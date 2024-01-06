#include "TypeCheck.h"

// maps to store the type information. Feel free to design new data structures if you need.
typeMap g_token2Type; // global token ids to type
/*
-1 scala
0 id
>0 num
*/
#define INSERT(out) *out << __FILE__ << ":" << __LINE__ << std::endl;
typeMap funcparam_token2Type; // func params token ids to type
vector<string> globalName;
paramMemberMap func2Param;
typeMap func2ret;
FunctionParameterList *digInTempFunct = nullptr;
paramMemberMap struct2Members;
string* currentFunct;
struct DefDecl
{
    bool Def;
    bool Decl;
};

std::unordered_map<string, DefDecl *> func2WheatherDefDecl;

// private util functions. You can use these functions to help you debug.
void error_print(std::ostream *out, A_pos p, string info)
{
    *out << "\nTypecheck error in line " << p->line << ", col " << p->col << ": " << info << std::endl
         << std::endl;
    exit(0);
}
// 全局变量中查找这个id是否使用过
void avoid_id2_global(std::ostream *out, A_pos p, string &name, const string &error = "a global varies have the same name have been def or decl")
{
    auto it = g_token2Type.find(name);
    if (it != g_token2Type.end())
    {
        error_print(out, p, error);
    }
}

// 在struct的变量中查找这个id是否使用过
void avoid_id2_struct(std::ostream *out, A_pos p, string &name)
{
    auto it = struct2Members.find(name);
    if (it != struct2Members.end())
    {
        error_print(out, p, "a struct have the same id have been defined");
    }
}
void avoid_id2_fun(std::ostream *out, A_pos p, string &name)
{
    auto it = func2WheatherDefDecl.find(name);
    if (it != func2WheatherDefDecl.end())
    {
        error_print(out, p, "a function have the same id have been defined");
    }
}
// 如果这个struct没有定义就是用，报错
void check_struct_defined(std::ostream *out, A_pos p, const string &name)
{
    auto it = struct2Members.find(name);
    if (it == struct2Members.end())
    {
        error_print(out, p, "struct not defined");
    }
}
string *get_name_from_varDecls(const aA_varDecl varDecls)
{
    if (!varDecls)
    {
        return nullptr;
    }
    switch (varDecls->kind)
    {
    case A_varDeclScalarKind:
    {
        auto y = varDecls->u.declScalar;
        return y->id;
        break;
    }

    case A_varDeclArrayKind:
    {
        auto y = varDecls->u.declArray;
        return y->id;
        break;
    }

    default:
        break;
    }
    return nullptr;
}
void print_token_map(typeMap *map)
{
    for (auto it = map->begin(); it != map->end(); it++)
    {
        std::cout << it->first << " : ";
        switch (it->second->type->type)
        {
        case A_dataType::A_nativeTypeKind:
            switch (it->second->type->u.nativeType)
            {
            case A_nativeType::A_intTypeKind:
                std::cout << "int";
                break;
            default:
                break;
            }
            break;
        case A_dataType::A_structTypeKind:
            std::cout << *(it->second->type->u.structType);
            break;
        default:
            break;
        }
        std::cout << std::endl;
    }
}

// public functions
// This is the entrace of this file.
void check_Prog(std::ostream *out, aA_program p)
{
    // p is the root of AST tree.
    for (auto ele : p->programElements)
    {
        switch (ele->kind)
        {
        case A_programVarDeclStmtKind:
            check_VarDecl(out, ele->u.varDeclStmt, globalName);
            break;

        case A_programStructDefKind:
            check_StructDef(out, ele->u.structDef);
            break;
        default:
            break;
        }

        /*
            Write your code here.

            Hint:
            1. Design the order of checking the program elements to meet the requirements that funtion declaration and global variable declaration can be used anywhere in the program.

            2. Many types of statements indeed collapse to some same units, so a good abstract design will help you reduce the amount of your code.
        */
    }
    for (auto ele : p->programElements)
    {
        switch (ele->kind)
        {
        case A_programFnDefKind:
            check_FnDef(out, ele->u.fnDef);
            break;

        case A_programFnDeclStmtKind:
            check_FnDeclStmt(out, ele->u.fnDeclStmt);
            break;
        default:
            break;
        }
    }

    for (auto ele : p->programElements)
    {
        switch (ele->kind)
        {
        case A_programFnDefKind:
            check_FnDefBlock(out, ele->u.fnDef);
            break;
        default:
            break;
        }
    }

    (*out) << "\nTypecheck passed!" << std::endl
           << std::endl;
    return;
}

void check_VarDecl(std::ostream *out, aA_varDeclStmt vd, vector<string> &tempname)
{
    // variable declaration statement
    if (!vd)
        return;
    string name;
    if (vd->kind == A_varDeclStmtType::A_varDeclKind)
    {
        // if declaration only
        // Example:
        //   let a:int;
        //   let a[5]:int;

        /* write your code here*/
        // 变量声明，复用
        string *name = get_name_from_varDecls(vd->u.varDecl);
        My_type *my_type = new My_type(vd->u.varDecl);
        // 名称和
        avoid_id2_global(out, vd->pos, *name);
        avoid_id2_struct(out, vd->pos, *name);
        avoid_id2_fun(out, vd->pos, *name);
        if (digInTempFunct != nullptr)
        {
            if (digInTempFunct->getParameterTypeByName(*name) != nullptr)
            {
                error_print(out, vd->pos, "Duplicated with function para");
            }
        }

        if (my_type->type&& my_type->type->type == A_structTypeKind) // 如果是结构体
        {
            check_struct_defined(out, my_type->type->pos, *my_type->type->u.structType);
        }
        g_token2Type[*name] = my_type;
        tempname.push_back(*name);
    }
    else if (vd->kind == A_varDeclStmtType::A_varDefKind)
    {
        // if both declaration and initialization
        // Example:
        //   let a:int = 5;

        /* write your code here */
        switch (vd->u.varDef->kind)
        {
        case A_varDefScalarKind:
        {
            auto x = vd->u.varDef->u.defScalar;
            name = *x->id;
            avoid_id2_global(out, x->pos, name);
            avoid_id2_struct(out, x->pos, name);
            avoid_id2_fun(out, x->pos, name);
            if (digInTempFunct != nullptr)
            {
                if (digInTempFunct->getParameterTypeByName(name) != nullptr)
                {
                    error_print(out, vd->pos, "Duplicated with function para");
                }
            }
            auto type = x->type;
            if (type&&type->type == A_structTypeKind)
            {
                check_struct_defined(out, x->pos, *x->type->u.structType);
            }
            g_token2Type[name] = new My_type(type, Scalar);
            /* code about rightVal*/
            My_type *rightTye = check_RightVal(out, x->val);
            if (!g_token2Type[name]->com_type(*rightTye))
            {
                error_print(out, x->val->pos, "Right value type mismatch");
            }
            tempname.push_back(name);
            break;
        }

        case A_varDefArrayKind:
        {
            auto x = vd->u.varDef->u.defArray;
            name = *x->id;
            avoid_id2_global(out, x->pos, name);
            avoid_id2_struct(out, x->pos, name);
            avoid_id2_fun(out, x->pos, name);
            if (digInTempFunct != nullptr)
            {
                if (digInTempFunct->getParameterTypeByName(name) != nullptr)
                {
                    error_print(out, vd->pos, "Duplicated with function para");
                }
            }
            auto type = x->type;
            if (type&& type->type== A_structTypeKind)
            {
                check_struct_defined(out, x->pos, *x->type->u.structType);
            }
            if (vd->u.varDecl->u.declArray->len <= 0)
            {
                error_print(out, vd->u.varDecl->u.declArray->pos, "array length not valid");
            }
            g_token2Type[name] = new My_type(type, Array, vd->u.varDecl->u.declArray->len);
            /* code about rightValList*/
            My_type *innerType = new My_type(*g_token2Type[name] );
            innerType->kind=Scalar;
            if (x->vals.size() > x->len)
            {
                error_print(out, x->pos, "The initialization list of the array is too long");
            }
            for (auto rv : x->vals)
            {
                My_type *rightTye = check_RightVal(out, rv);
                if (!innerType->com_type(*rightTye))
                {
                    error_print(out, rv->pos, "Right value type mismatch");
                }
            }
            tempname.push_back(name);
            break;
        }
        default:
            break;
        }
    }
    return;
}

void check_StructDef(std::ostream *out, aA_structDef sd)
{
    if (!sd)
        return;
    // 查查这个id是否使用过
    // 和全局变量、结构体、函数比较
    avoid_id2_global(out, sd->pos, *sd->id);
    avoid_id2_struct(out, sd->pos, *sd->id);
    avoid_id2_fun(out, sd->pos, *sd->id);

    // id 合法，检查内部变量是否重名

    FunctionParameterList *functionParameterList = new FunctionParameterList();
    for (auto x : sd->varDecls)
    {
        // 检查内部id是否出现问题
        string *name = get_name_from_varDecls(x);
        bool res = functionParameterList->addParameter(*name, new My_type(x));
        if (res == false)
        {
            error_print(out, x->pos, "duplicated id in structDef");
        }
        // 检查类型是否合法
        My_type *my_type = functionParameterList->getParameterTypeByName(*name);
        if (my_type->type->type == A_structTypeKind)
        {
            check_struct_defined(out, my_type->type->pos, *my_type->type->u.structType);
        }
    }
    struct2Members[*sd->id] = functionParameterList;
    // structure definition
    // Example:
    //      struct A{
    //          a:int;
    //          b:int;
    //      }

    /* write your code here */
    return;
}

void check_FnDecl(std::ostream *out, aA_fnDecl fd)
{
    // Example:
    //      fn main(a:int, b:int)->int
    if (!fd)
        return;
    /*
        write your code here
        Hint: you may need to check if the function is already declared
    */
    // 查查这个id是否使用过
    // 和全局变量、结构体比较
    avoid_id2_global(out, fd->pos, *fd->id);
    avoid_id2_struct(out, fd->pos, *fd->id);
    // avoid_id2_fun(out, fd->pos, *fd->id);函数比较在进入这里之间已经处理
    FunctionParameterList *functionParameterList = new FunctionParameterList();
    for (auto x : fd->paramDecl->varDecls)
    {
        // 检查内部id是否出现问题
        string *name = get_name_from_varDecls(x);
        bool res = functionParameterList->addParameter(*name, new My_type(x));
        if (res == false)
        {
            error_print(out, x->pos, "duplicated id in structDef");
        }
        // 检查类型是否合法
        My_type *my_type = functionParameterList->getParameterTypeByName(*name);
        if (my_type->type->type == A_structTypeKind)
        {
            check_struct_defined(out, my_type->type->pos,* my_type->type->u.structType);
        }
    }
    func2Param[*fd->id] = functionParameterList;
    return;
}

void check_FnDeclStmt(std::ostream *out, aA_fnDeclStmt fd)
{
    // Example:
    //      fn main(a:int, b:int)->int;
    if (!fd)
        return;
    string name = *fd->fnDecl->id;
    avoid_id2_fun(out, fd->pos, name);
    check_FnDecl(out, fd->fnDecl);
    DefDecl *defDecl = new DefDecl();
    defDecl->Decl = true;
    defDecl->Def = false;
    func2WheatherDefDecl[name] = defDecl;
    // 函数只能返回标量
    My_type *my_type = nullptr;
    if (fd->fnDecl->type != nullptr)
    {
        my_type = new My_type(fd->fnDecl->type, Scalar);
        if (my_type->type->type == A_structTypeKind)
        {
            check_struct_defined(out, my_type->type->pos, *my_type->type->u.structType);
        }
    }
    func2ret[name] = my_type;

    return;
}

void check_FnDef(std::ostream *out, aA_fnDef fd)
{
    // Example:
    //      fn main(a:int, b:int)->int{
    //          let c:int;
    //          c = a + b;
    //          return c;
    //      }
    if (!fd)
        return;
    string name = *fd->fnDecl->id;
    auto it = func2WheatherDefDecl.find(name);
    FunctionParameterList *functionParameterListDecl = nullptr;
    if (it != func2WheatherDefDecl.end())
    {
        if (it->second->Def == true) // 已经定义
            error_print(out, fd->pos, "a function have the same id have been defined");
        
        if (it->second->Decl == true)
        { // 检查函数声明和定义时类型以及个数是否匹配 1
            functionParameterListDecl = func2Param[name];
            // 检查返回值类型是否一致
            My_type *my_type = nullptr;
            if (fd->fnDecl->type != nullptr)
            {
                my_type = new My_type(fd->fnDecl->type, Scalar);
                if (my_type->type->type == A_structTypeKind)
                {
                    check_struct_defined(out, my_type->type->pos, *my_type->type->u.structType);
                }
            }
            if (my_type == nullptr && func2ret[name] != nullptr)
            {
                error_print(out, fd->fnDecl->pos, "Declaration and definition return values are different");
            }
            if (my_type != nullptr && func2ret[name] == nullptr)
            {
                error_print(out, fd->fnDecl->pos, "Declaration and definition return values are different");
            }
            if(my_type==func2ret[name]&&my_type==nullptr)
            {
                error_print(out, fd->fnDecl->pos, "Declaration and definition return values are different");
            }else 
            if (!my_type->com_type(*func2ret[name]))
            {
                error_print(out, fd->fnDecl->pos, "Declaration and definition return values are different");
            }
            func2ret[name] = my_type;
        }
    }
    else
    {
        // 函数只能返回标量
        My_type *my_type = nullptr;
        if (fd->fnDecl->type != nullptr)
        {
            my_type = new My_type(fd->fnDecl->type, Scalar);
            if (my_type->type->type == A_structTypeKind)
            {
                check_struct_defined(out, my_type->type->pos, *my_type->type->u.structType);
            }
        }
        func2ret[name] = my_type;
    }
    check_FnDecl(out, fd->fnDecl);
    if (functionParameterListDecl)
    {
        // 检查函数声明和定义时类型以及个数是否匹配 2
        if (functionParameterListDecl->getParameterLength() != func2Param[name]->getParameterLength())
        {
            error_print(out, fd->fnDecl->pos, "The parameter list is different when declaring and defining");
        }
        int n = functionParameterListDecl->getParameterLength();
        for (int i = 0; i < n; i++)
        {
            if (!functionParameterListDecl->getParameterTypeByIndex(i)->com_type(*func2Param[name]->getParameterTypeByIndex(i)))
            {
                error_print(out, fd->fnDecl->pos, "The parameter list is different when declaring and defining");
            }
        }
    }
    if (it != func2WheatherDefDecl.end())
    {
        it->second->Def = true;
    }
    else
    {
        DefDecl *defDecl = new DefDecl();
        defDecl->Decl = false;
        defDecl->Def = true;
        func2WheatherDefDecl[name] = defDecl;
    }

    /*
        write your code here
        Hint: you may pay attention to the function parameters, local variables and global variables.
    */
    // first,
    return;
}
void check_FnDefBlock(std::ostream *out, aA_fnDef fd)
{
    digInTempFunct = func2Param[*fd->fnDecl->id];
    currentFunct = fd->fnDecl->id;

    vector<string> blockScope;
    for (auto x : fd->stmts)
    {
        check_CodeblockStmt(out, x, blockScope);
    }
    for (auto x : blockScope)
    {
        g_token2Type.erase(x);
    }
    currentFunct = nullptr;
    digInTempFunct = nullptr;
}
void check_CodeblockStmt(std::ostream *out, aA_codeBlockStmt cs, vector<string> &blockScope)
{
    if (!cs)
        return;
    switch (cs->kind)
    {
    case A_codeBlockStmtType::A_varDeclStmtKind:
        check_VarDecl(out, cs->u.varDeclStmt, blockScope); // ？？
        break;
    case A_codeBlockStmtType::A_assignStmtKind:
        check_AssignStmt(out, cs->u.assignStmt);
        break;
    case A_codeBlockStmtType::A_ifStmtKind:
        check_IfStmt(out, cs->u.ifStmt);
        break;
    case A_codeBlockStmtType::A_whileStmtKind:
        check_WhileStmt(out, cs->u.whileStmt);
        break;
    case A_codeBlockStmtType::A_callStmtKind:
        check_CallStmt(out, cs->u.callStmt);
        break;
    case A_codeBlockStmtType::A_returnStmtKind:
        check_ReturnStmt(out, cs->u.returnStmt);
        break;
    case A_codeBlockStmtType::A_continueStmtKind:{
        // os << "continue;";
        break;
    }
    case A_codeBlockStmtType::A_breakStmtKind:{
        // os << "break;";
        break;
    }
    default:
        break;
    }
    return;
}

void check_AssignStmt(std::ostream *out, aA_assignStmt as)
{
    if (!as)
        return;
    string name;
    My_type *my_type = nullptr;
    switch (as->leftVal->kind)
    {
    case A_leftValType::A_varValKind:
    {
        /* write your code here */
        my_type = check_IdExpr(out, as->leftVal->pos, *as->leftVal->u.id);
    }
    break;
    case A_leftValType::A_arrValKind:
    {
        my_type = check_ArrayExpr(out, as->leftVal->u.arrExpr);
        /* write your code here */
    }
    break;
    case A_leftValType::A_memberValKind:
    {
        my_type = check_MemberExpr(out, as->leftVal->u.memberExpr);
        /* write your code here */
    }
    break;
    }
    if (my_type == nullptr)
    {
        error_print(out, as->leftVal->pos, "leftVal type error");
    }
    My_type *rightTye = check_RightVal(out, as->rightVal);
    if (rightTye == nullptr)
    {
        error_print(out, as->rightVal->pos, "rightVal type error");
    }
    if (!my_type->com_type(*rightTye))
    {
        error_print(out, as->pos, "Left value right value type mismatch");
    }
    if(my_type->kind==Array)
    {
        error_print(out, as->pos, "Array assignment error!");
    }
    return;
}
My_type *check_IdExpr(std::ostream *out, A_pos pos, string id)
{
    if (digInTempFunct == nullptr) // 函数外部
    {
        My_type *my_type = nullptr;
        my_type = g_token2Type[id];
        if (my_type == nullptr)
        {
            error_print(out, pos, "id not defined");
        }
        return my_type;
    }
    else
    {
        My_type *my_type = nullptr;
        my_type = digInTempFunct->getParameterTypeByName(id);
        if (my_type == nullptr) // 在函数中没有找到
        {
            my_type = g_token2Type[id];
            if (my_type == nullptr)
            {
                error_print(out, pos, "id not defined");
            }
        }
        return my_type;
    }
    /* write your code here */
    return nullptr;
}
My_type *check_ArrayExpr(std::ostream *out, aA_arrayExpr ae)
{
    if (!ae)
        return nullptr;
    /*
        Example:
            a[1] = 0;
        Hint:
            check the validity of the array index
    */
    if (digInTempFunct == nullptr) // 函数外部
    {
        My_type *my_type = nullptr;
        my_type = g_token2Type[*ae->arr];
        if (my_type == nullptr)
        {
            error_print(out, ae->pos, "struct id not defined");
        }
        if (my_type->kind == A_Scalar_Array::Scalar)
        {
            error_print(out, ae->pos, "this is not an array");
        }
        My_type *arr_type = new My_type(*my_type);
        arr_type->kind = Scalar;
        if(ae->idx->kind==A_idIndexKind)
        {
            // A_type a_type = A_NativeType(0,A_intTypeKind);
            aA_type a_type = new aA_type_;
            a_type->pos = ae->idx->pos;
            a_type->type = A_nativeTypeKind;
            a_type->u.nativeType = A_intTypeKind;
            My_type *type = new My_type(a_type ,Scalar);
            string* idname= ae->idx->u.id;
            if(!type->com_type(*check_IdExpr(out,ae->idx->pos ,*idname)))
            {
                error_print(out, ae->pos, "id index error");
            }
        }
        return arr_type;
    }
    else
    {
        My_type *my_type = nullptr;
        my_type = digInTempFunct->getParameterTypeByName(*ae->arr);
        if (my_type == nullptr) // 在函数中没有找到
        {
            my_type = g_token2Type[*ae->arr];
            if (my_type == nullptr)
            {
                error_print(out, ae->pos, "id not defined");
            }
        }
        if (my_type->kind == A_Scalar_Array::Scalar)
        {
            error_print(out, ae->pos, "this is not an array");
        }
        My_type *arr_type = new My_type(*my_type);
        arr_type->kind = Scalar;
        if(ae->idx->kind==A_idIndexKind)
        {
            // A_type a_type = A_NativeType(0,A_intTypeKind);
            aA_type a_type = new aA_type_;
            a_type->pos = 0;
            a_type->type = A_nativeTypeKind;
            a_type->u.nativeType = A_intTypeKind;
            My_type *type = new My_type(a_type ,Scalar);
            string* idname= ae->idx->u.id;
            if(!type->com_type(*check_IdExpr(out,ae->idx->pos ,*idname)))
            {
                error_print(out, ae->pos, "id index error");
            }
        }
        return arr_type;
    }
    return nullptr;
}

My_type *check_MemberExpr(std::ostream *out, aA_memberExpr me)
{
    // check if the member exists and return the tyep of the member
    // you may need to check if the type of this expression matches with its
    // leftvalue or rightvalue, so return its aA_type would be a good way. Feel
    // free to change the design pattern if you need.
    if (!me)
        return nullptr;
    /*
        Example:
            a.b
    */
    if (digInTempFunct == nullptr) // 函数外部
    {
        My_type *my_type = nullptr;
        my_type = g_token2Type[*me->structId];
        if (my_type == nullptr)
        {
            error_print(out, me->pos, "struct id not defined");
        }
        if (my_type->type->type == A_nativeTypeKind)
        {
            error_print(out, me->pos, "this id is not struct");
        }
        string structTypeName = *my_type->type->u.structType;
        My_type *memberExpr = struct2Members[structTypeName]->getParameterTypeByName(*me->memberId);
        if (memberExpr == nullptr)
        {
            error_print(out, me->pos, "struct member not defined");
        }
        return memberExpr;
    }
    else
    {
        My_type *my_type = nullptr;
        my_type = digInTempFunct->getParameterTypeByName(*me->structId);
        if (my_type == nullptr) // 在函数中没有找到
        {
            my_type = g_token2Type[*me->structId];
            if (my_type == nullptr)
            {
                error_print(out, me->pos, "id not defined");
            }
        }
        if (my_type->type->type == A_nativeTypeKind)
        {
            error_print(out, me->pos, "this id is not struct");
        }
        string structTypeName = *my_type->type->u.structType;
        My_type *memberExpr = struct2Members[structTypeName]->getParameterTypeByName(*me->memberId);
        if (memberExpr == nullptr)
        {
            error_print(out, me->pos, "struct member not defined");
        }
        return memberExpr;
    }
    return nullptr;
}

void check_IfStmt(std::ostream *out, aA_ifStmt is)
{
    if (!is)
        return;
    check_BoolExpr(out, is->boolExpr);
    vector<string> blockScope;
    for (aA_codeBlockStmt s : is->ifStmts)
    {
        check_CodeblockStmt(out, s, blockScope);
    }
    for (auto x : blockScope)
    {
        g_token2Type.erase(x);
    }
    blockScope.clear();
    for (aA_codeBlockStmt s : is->elseStmts)
    {
        check_CodeblockStmt(out, s, blockScope);
    }
    for (auto x : blockScope)
    {
        g_token2Type.erase(x);
    }
    return;
}

My_type *check_FuncCall(std::ostream *out, aA_fnCall fc)
{
    if (!fc)
        return nullptr;
    // Example:
    //      foo(1, 2);

    /* write your code here */
    DefDecl *defdecl = func2WheatherDefDecl[*fc->fn];
    if (defdecl == nullptr)
    {
        error_print(out, fc->pos, "function id not defined");
    }
    FunctionParameterList *fcList = func2Param[*fc->fn];
    if (fcList->getParameterLength() != fc->vals.size())
    {
        error_print(out, fc->pos, "function para error");
    }
    int len = fcList->getParameterLength();
    for (int i = 0; i < len; i++)
    {
        if (!fcList->getParameterTypeByIndex(i)->com_type(*check_RightVal(out, fc->vals[i])))
        {
            error_print(out, fc->pos, "function para error");
        }
    }
    return func2ret[*fc->fn];
}

void check_WhileStmt(std::ostream *out, aA_whileStmt ws)
{
    if (!ws)
        return;
    check_BoolExpr(out, ws->boolExpr);
    vector<string> block_scope;
    for (aA_codeBlockStmt s : ws->whileStmts)
    {
        check_CodeblockStmt(out, s, block_scope);
    }
    for (auto x : block_scope)
    {
        g_token2Type.erase(x);
    }
    return;
}

void check_CallStmt(std::ostream *out, aA_callStmt cs)
{
    if (!cs)
        return;
    check_FuncCall(out, cs->fnCall);
    return;
}

void check_ReturnStmt(std::ostream *out, aA_returnStmt rs)
{
    if (!rs)
        return;
    My_type* my_type = nullptr;
    My_type* retType = func2ret[*currentFunct];
    my_type = check_RightVal(out, rs->retVal);
    if (my_type->kind == Array){
        error_print(out, rs->pos, "function cannot return array");
    }

    if (my_type == nullptr && retType == nullptr){
        return;
    }
    else if (my_type == nullptr || retType == nullptr){
        error_print(out, rs->pos, "return type not correct");
    }
    else if (!my_type->com_type(*retType)){
        error_print(out, rs->pos, "return type not correct");
    }
    return;
}

My_type *check_RightVal(std::ostream *out, aA_rightVal rv)
{
    switch (rv->kind)
    {
    case A_arithExprValKind:
        return check_ArithExpr(out, rv->u.arithExpr);
        /* code */
        break;
    case A_boolExprValKind:
        return check_BoolExpr(out, rv->u.boolExpr);
        /* code */
        break;
    default:
        break;
    }
    return nullptr;
}
My_type *check_ArithExpr(std::ostream *out, aA_arithExpr ae)
{
    switch (ae->kind)
    {
    case A_arithBiOpExprKind:
        return check_ArithBiOpExpr(out, ae->u.arithBiOpExpr);
        /* code */
        break;
    case A_exprUnitKind:
        return check_ExprUnit(out, ae->u.exprUnit);
        break;
    default:
        break;
    }
    return nullptr;
}
My_type *check_ArithBiOpExpr(std::ostream *out, aA_arithBiOpExpr abe)
{
    My_type *left = check_ArithExpr(out, abe->left);
    My_type *right = check_ArithExpr(out, abe->right);
    if (!left->com_type(*right))
    {
        error_print(out, abe->pos, "ArithBiOpExpr left and right have different type");
    }
    if (left->kind != Scalar)
    {
        error_print(out, abe->left->pos, "The element of a binary expression cannot be an array");
    }
    if (right->kind != Scalar)
    {
        error_print(out, abe->left->pos, "The element of a binary expression cannot be an array");
    }
    if (left->type->type != A_nativeTypeKind)
    {
        error_print(out, abe->left->pos, "The element of a binary expression cannot be a structure");
    }
    if (right->type->type != A_nativeTypeKind)
    {
        error_print(out, abe->left->pos, "The element of a binary expression cannot be a structure");
    }
    return left;
}
My_type *check_ExprUnit(std::ostream *out, aA_exprUnit eu)
{
    // validate the expression unit and return the aA_type of it
    // you may need to check if the type of this expression matches with its
    // leftvalue or rightvalue, so return its aA_type would be a good way.
    // Feel free to change the design pattern if you need.
    if (!eu)
        return nullptr;
    aA_type ret;
    switch (eu->kind)
    {
    case A_exprUnitType::A_idExprKind:
    {
        return check_IdExpr(out, eu->pos, *eu->u.id);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_numExprKind:
    {
        aA_type t = new aA_type_;
        t->pos = eu->pos;
        t->type = A_nativeTypeKind;
        t->u.nativeType = A_intTypeKind;

        return new My_type(t, Scalar);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_fnCallKind:
    {
        return check_FuncCall(out, eu->u.callExpr);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_arrayExprKind:
    {
        return check_ArrayExpr(out, eu->u.arrayExpr);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_memberExprKind:
    {
        return check_MemberExpr(out, eu->u.memberExpr);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_arithExprKind:
    {
        return check_ArithExpr(out, eu->u.arithExpr);
        /* write your code here */
    }
    break;
    case A_exprUnitType::A_arithUExprKind:
    {
        return check_ArithUExpr(out, eu->u.arithUExpr);
        /* write your code here */
    }
    break;
    }
    // return ret;
    return nullptr;
}
My_type *check_ArithUExpr(std::ostream *out, aA_arithUExpr eue)
{
    if (!eue)
    {
        return nullptr;
    }
    return check_ExprUnit(out, eue->expr);
}
My_type *check_BoolExpr(std::ostream *out, aA_boolExpr be)
{
    if (!be)
        return nullptr;
    switch (be->kind)
    {
    case A_boolExprType::A_boolBiOpExprKind:
        /* write your code here */
        break;
    case A_boolExprType::A_boolUnitKind:
        check_BoolUnit(out, be->u.boolUnit);
        break;
    default:
        break;
    }
    return nullptr;
}
My_type *check_BoolUnit(std::ostream *out, aA_boolUnit bu)
{
    if (!bu)
        return nullptr;
    switch (bu->kind)
    {
    case A_boolUnitType::A_comOpExprKind:
    {
        return check_ComExpr(out, bu->u.comExpr);
        /* write your code here */
    }
    break;
    case A_boolUnitType::A_boolExprKind:
        /* write your code here */
        return check_BoolExpr(out, bu->u.boolExpr);
    case A_boolUnitType::A_boolUOpExprKind:
        return check_BoolUOpExpr(out, bu->u.boolUOpExpr);
        /* write your code here */
        break;
    default:
        break;
    }
    return nullptr;
}
My_type *check_ComExpr(std::ostream *out, aA_comExpr ce)
{
    if (!ce)
    {
        return nullptr;
    }
    My_type *left = check_ExprUnit(out, ce->left);
    My_type *right = check_ExprUnit(out, ce->right);
    if (!left->com_type(*right))
    {
        error_print(out, ce->pos, "Different types on both sides of the comparison operator");
    }
    if (left->kind != Scalar)
    {
        error_print(out, ce->left->pos, "The element of a binary expression cannot be an array");
    }
    if (right->kind != Scalar)
    {
        error_print(out, ce->left->pos, "The element of a binary expression cannot be an array");
    }
    if (left->type->type != A_nativeTypeKind)
    {
        error_print(out, ce->left->pos, "The element of a binary expression cannot be a structure");
    }
    if (right->type->type != A_nativeTypeKind)
    {
        error_print(out, ce->left->pos, "The element of a binary expression cannot be a structure");
    }
    return left;
}
My_type *check_BoolUOpExpr(std::ostream *out, aA_boolUOpExpr bue)
{
    if (!bue)
    {
        return nullptr;
    }
    return check_BoolUnit(out, bue->cond);
}
