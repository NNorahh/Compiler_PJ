#include "ast2llvm.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <list>

using namespace std;
using namespace LLVMIR;
#define INSERT() printf("%s:%d\n", __FILE__, __LINE__);
static unordered_map<string, FuncType> funcReturnMap;
static unordered_map<string, StructInfo> structInfoMap;
static unordered_map<string, Name_name *> globalVarMap;
static unordered_map<string, Temp_temp *> localVarMap;
static list<pair<Temp_label *, Temp_label *>> blockstack;
static unordered_map<string, L_funcdecl *> funcalargs;
static list<L_stm *> emit_irs;

LLVMIR::L_prog *ast2llvm(aA_program p)
{

    auto defs = ast2llvmProg_first(p);

    auto funcs = ast2llvmProg_second(p);

    vector<L_func *> funcs_block;
    for (const auto &f : funcs)
    {
        funcs_block.push_back(ast2llvmFuncBlock(f));
    }
    for (auto &f : funcs_block)
    {
        ast2llvm_moveAlloca(f);
    }
    return new L_prog(defs, funcs_block);
}

int ast2llvmRightVal_first(aA_rightVal r)
{
    if (r == nullptr)
    {
        return 0;
    }
    switch (r->kind)
    {
    case A_arithExprValKind:
    {
        return ast2llvmArithExpr_first(r->u.arithExpr);
        break;
    }
    case A_boolExprValKind:
    {
        return ast2llvmBoolExpr_first(r->u.boolExpr);
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmBoolExpr_first(aA_boolExpr b)
{
    switch (b->kind)
    {
    case A_boolBiOpExprKind:
    {
        return ast2llvmBoolBiOpExpr_first(b->u.boolBiOpExpr);
        break;
    }
    case A_boolUnitKind:
    {
        return ast2llvmBoolUnit_first(b->u.boolUnit);
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmBoolBiOpExpr_first(aA_boolBiOpExpr b)
{
    int l = ast2llvmBoolExpr_first(b->left);
    int r = ast2llvmBoolExpr_first(b->right);
    if (b->op == A_and)
    {
        return l && r;
    }
    else
    {
        return l || r;
    }
}

int ast2llvmBoolUOpExpr_first(aA_boolUOpExpr b)
{
    if (b->op == A_not)
    {
        return !ast2llvmBoolUnit_first(b->cond);
    }
    return 0;
}

int ast2llvmBoolUnit_first(aA_boolUnit b)
{
    switch (b->kind)
    {
    case A_comOpExprKind:
    {
        return ast2llvmComOpExpr_first(b->u.comExpr);
        break;
    }
    case A_boolExprKind:
    {
        return ast2llvmBoolExpr_first(b->u.boolExpr);
        break;
    }
    case A_boolUOpExprKind:
    {
        return ast2llvmBoolUOpExpr_first(b->u.boolUOpExpr);
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmComOpExpr_first(aA_comExpr c)
{
    auto l = ast2llvmExprUnit_first(c->left);
    auto r = ast2llvmExprUnit_first(c->right);
    switch (c->op)
    {
    case A_lt:
    {
        return l < r;
        break;
    }
    case A_le:
    {
        return l <= r;
        break;
    }
    case A_gt:
    {
        return l > r;
        break;
    }
    case A_ge:
    {
        return l >= r;
        break;
    }
    case A_eq:
    {
        return l == r;
        break;
    }
    case A_ne:
    {
        return l != r;
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmArithBiOpExpr_first(aA_arithBiOpExpr a)
{
    auto l = ast2llvmArithExpr_first(a->left);
    auto r = ast2llvmArithExpr_first(a->right);
    switch (a->op)
    {
    case A_add:
    {
        return l + r;
        break;
    }
    case A_sub:
    {
        return l - r;
        break;
    }
    case A_mul:
    {
        return l * r;
        break;
    }
    case A_div:
    {
        return l / r;
        break;
    }
    default:
        break;
    }
    return 0;
}

int ast2llvmArithUExpr_first(aA_arithUExpr a)
{
    if (a->op == A_neg)
    {
        return -ast2llvmExprUnit_first(a->expr);
    }
    return 0;
}

int ast2llvmArithExpr_first(aA_arithExpr a)
{
    switch (a->kind)
    {
    case A_arithBiOpExprKind:
    {
        return ast2llvmArithBiOpExpr_first(a->u.arithBiOpExpr);
        break;
    }
    case A_exprUnitKind:
    {
        return ast2llvmExprUnit_first(a->u.exprUnit);
        break;
    }
    default:
        assert(0);
        break;
    }
    return 0;
}

int ast2llvmExprUnit_first(aA_exprUnit e)
{
    if (e->kind == A_numExprKind)
    {
        return e->u.num;
    }
    else if (e->kind == A_arithExprKind)
    {
        return ast2llvmArithExpr_first(e->u.arithExpr);
    }
    else if (e->kind == A_arithUExprKind)
    {
        return ast2llvmArithUExpr_first(e->u.arithUExpr);
    }
    else
    {
        assert(0);
    }
    return 0;
}

std::vector<LLVMIR::L_def *> ast2llvmProg_first(aA_program p)
{
    vector<L_def *> defs;
    // 函数声明
    defs.push_back(L_Funcdecl("getch", vector<TempDef>(), FuncType(ReturnType::INT_TYPE)));
    defs.push_back(L_Funcdecl("getint", vector<TempDef>(), FuncType(ReturnType::INT_TYPE)));
    defs.push_back(L_Funcdecl("putch", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("putint", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("putarray", vector<TempDef>{TempDef(TempType::INT_TEMP), TempDef(TempType::INT_PTR, -1)}, FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("_sysy_starttime", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    defs.push_back(L_Funcdecl("_sysy_stoptime", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    funcReturnMap.emplace("getch", FuncType(ReturnType::INT_TYPE));
    funcReturnMap.emplace("getint", FuncType(ReturnType::INT_TYPE));
    funcReturnMap.emplace("putch", FuncType(ReturnType::VOID_TYPE));
    funcReturnMap.emplace("putint", FuncType(ReturnType::VOID_TYPE));
    funcReturnMap.emplace("putarray", FuncType(ReturnType::VOID_TYPE));
    funcReturnMap.emplace("_sysy_starttime", FuncType(ReturnType::VOID_TYPE));
    funcReturnMap.emplace("_sysy_stoptime", FuncType(ReturnType::VOID_TYPE));
    funcalargs.emplace("getch", new L_funcdecl("getch", vector<TempDef>(), FuncType(ReturnType::INT_TYPE)));
    funcalargs.emplace("getint", new L_funcdecl("getint", vector<TempDef>(), FuncType(ReturnType::INT_TYPE)));
    funcalargs.emplace("putch", new L_funcdecl("putch", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    funcalargs.emplace("putint", new L_funcdecl("putint", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    funcalargs.emplace("putarray", new L_funcdecl("putarray", vector<TempDef>{TempDef(TempType::INT_TEMP), TempDef(TempType::INT_PTR, -1)}, FuncType(ReturnType::VOID_TYPE)));
    funcalargs.emplace("_sysy_starttime", new L_funcdecl("_sysy_starttime", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    funcalargs.emplace("_sysy_stoptime", new L_funcdecl("_sysy_stoptime", vector<TempDef>{TempDef(TempType::INT_TEMP)}, FuncType(ReturnType::VOID_TYPE)));
    for (const auto &v : p->programElements)
    {

        switch (v->kind)
        {
        case A_programNullStmtKind:
        {
            break;
        }
        case A_programVarDeclStmtKind:
        {
            if (v->u.varDeclStmt->kind == A_varDeclKind) // 变量声明
            {
                if (v->u.varDeclStmt->u.varDecl->kind == A_varDeclScalarKind) // 标量
                {
                    if (v->u.varDeclStmt->u.varDecl->u.declScalar->type->type == A_structTypeKind) // 自定义结构体
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,
                                             Name_newname_struct(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declScalar->id), *v->u.varDeclStmt->u.varDecl->u.declScalar->type->u.structType));
                        TempDef def(TempType::STRUCT_TEMP, 0, *v->u.varDeclStmt->u.varDecl->u.declScalar->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declScalar->id, def, vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declScalar->id,
                                             Name_newname_int(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declScalar->id)));
                        TempDef def(TempType::INT_TEMP, 0);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declScalar->id, def, vector<int>()));
                    }
                }
                else if (v->u.varDeclStmt->u.varDecl->kind == A_varDeclArrayKind)
                {
                    if (v->u.varDeclStmt->u.varDecl->u.declArray->type->type == A_structTypeKind)
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declArray->id,
                                             Name_newname_struct_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declArray->id), v->u.varDeclStmt->u.varDecl->u.declArray->len, *v->u.varDeclStmt->u.varDecl->u.declArray->type->u.structType));
                        TempDef def(TempType::STRUCT_PTR, v->u.varDeclStmt->u.varDecl->u.declArray->len, *v->u.varDeclStmt->u.varDecl->u.declArray->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declArray->id, def, vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDecl->u.declArray->id,
                                             Name_newname_int_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDecl->u.declArray->id), v->u.varDeclStmt->u.varDecl->u.declArray->len));
                        TempDef def(TempType::INT_PTR, v->u.varDeclStmt->u.varDecl->u.declArray->len);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDecl->u.declArray->id, def, vector<int>()));
                    }
                }
                else
                {
                    assert(0);
                }
            }
            else if (v->u.varDeclStmt->kind == A_varDefKind)
            {
                if (v->u.varDeclStmt->u.varDef->kind == A_varDefScalarKind)
                {
                    if (v->u.varDeclStmt->u.varDef->u.defScalar->type->type == A_structTypeKind)
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defScalar->id,
                                             Name_newname_struct(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defScalar->id), *v->u.varDeclStmt->u.varDef->u.defScalar->type->u.structType));
                        TempDef def(TempType::STRUCT_TEMP, 0, *v->u.varDeclStmt->u.varDef->u.defScalar->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defScalar->id, def, vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defScalar->id,
                                             Name_newname_int(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defScalar->id)));
                        TempDef def(TempType::INT_TEMP, 0);
                        vector<int> init;
                        init.push_back(ast2llvmRightVal_first(v->u.varDeclStmt->u.varDef->u.defScalar->val));
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defScalar->id, def, init));
                    }
                }
                else if (v->u.varDeclStmt->u.varDef->kind == A_varDefArrayKind)
                {
                    if (v->u.varDeclStmt->u.varDef->u.defArray->type->type == A_structTypeKind)
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defArray->id,
                                             Name_newname_struct_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defArray->id), v->u.varDeclStmt->u.varDef->u.defArray->len, *v->u.varDeclStmt->u.varDef->u.defArray->type->u.structType));
                        TempDef def(TempType::STRUCT_PTR, v->u.varDeclStmt->u.varDef->u.defArray->len, *v->u.varDeclStmt->u.varDef->u.defArray->type->u.structType);
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defArray->id, def, vector<int>()));
                    }
                    else
                    {
                        globalVarMap.emplace(*v->u.varDeclStmt->u.varDef->u.defArray->id,
                                             Name_newname_int_ptr(Temp_newlabel_named(*v->u.varDeclStmt->u.varDef->u.defArray->id), v->u.varDeclStmt->u.varDef->u.defArray->len));
                        TempDef def(TempType::INT_PTR, v->u.varDeclStmt->u.varDef->u.defArray->len);
                        vector<int> init;
                        for (auto &el : v->u.varDeclStmt->u.varDef->u.defArray->vals)
                        {
                            // 右值转换，厉害，尽管我不会
                            init.push_back(ast2llvmRightVal_first(el));
                        }
                        defs.push_back(L_Globaldef(*v->u.varDeclStmt->u.varDef->u.defArray->id, def, init));
                    }
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                assert(0);
            }
            break;
        }
        case A_programStructDefKind:
        {
            StructInfo si;
            int off = 0;
            vector<TempDef> members;
            for (const auto &decl : v->u.structDef->varDecls)
            {
                if (decl->kind == A_varDeclScalarKind)
                {
                    if (decl->u.declScalar->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_TEMP, 0, *decl->u.declScalar->type->u.structType);
                        MemberInfo info(off++, def);
                        si.memberinfos.emplace(*decl->u.declScalar->id, info);
                        members.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_TEMP, 0);
                        MemberInfo info(off++, def);
                        si.memberinfos.emplace(*decl->u.declScalar->id, info);
                        members.push_back(def);
                    }
                }
                else if (decl->kind == A_varDeclArrayKind)
                {
                    if (decl->u.declArray->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR, decl->u.declArray->len, *decl->u.declArray->type->u.structType);
                        MemberInfo info(off++, def);
                        si.memberinfos.emplace(*decl->u.declArray->id, info);
                        members.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_PTR, decl->u.declArray->len);
                        MemberInfo info(off++, def);
                        si.memberinfos.emplace(*decl->u.declArray->id, info);
                        members.push_back(def);
                    }
                }
                else
                {
                    assert(0);
                }
            }
            structInfoMap.emplace(*v->u.structDef->id, std::move(si));
            defs.push_back(L_Structdef(*v->u.structDef->id, members));
            break;
        }
        case A_programFnDeclStmtKind:
        {

            FuncType type;
            if (v->u.fnDeclStmt->fnDecl->type == nullptr)
            {
                type.type = ReturnType::VOID_TYPE;
            }
            else if (v->u.fnDeclStmt->fnDecl->type->type == A_nativeTypeKind)
            {
                type.type = ReturnType::INT_TYPE;
            }
            else if (v->u.fnDeclStmt->fnDecl->type->type == A_structTypeKind)
            {
                type.type = ReturnType::STRUCT_TYPE;
                type.structname = *v->u.fnDeclStmt->fnDecl->type->u.structType;
            }
            else
            {
                assert(0);
            }
            if (funcReturnMap.find(*v->u.fnDeclStmt->fnDecl->id) == funcReturnMap.end())
                funcReturnMap.emplace(*v->u.fnDeclStmt->fnDecl->id, std::move(type));
            vector<TempDef> args;

            for (const auto &decl : v->u.fnDeclStmt->fnDecl->paramDecl->varDecls)
            {
                if (decl->kind == A_varDeclScalarKind)
                {
                    if (decl->u.declScalar->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR, 0, *decl->u.declScalar->type->u.structType);
                        args.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_TEMP, 0);
                        args.push_back(def);
                    }
                }
                else if (decl->kind == A_varDeclArrayKind)
                {
                    if (decl->u.declArray->type->type == A_structTypeKind)
                    {
                        TempDef def(TempType::STRUCT_PTR, -1, *decl->u.declArray->type->u.structType);
                        args.push_back(def);
                    }
                    else
                    {
                        TempDef def(TempType::INT_PTR, -1);
                        args.push_back(def);
                    }
                }
                else
                {
                    assert(0);
                }
            }
            // defs.push_back(L_Funcdecl(*v->u.fnDeclStmt->fnDecl->id, args, type));
            funcalargs.emplace(*v->u.fnDeclStmt->fnDecl->id, new L_funcdecl(*v->u.fnDeclStmt->fnDecl->id, args, type));

            break;
        }
        case A_programFnDefKind:
        {
            if (funcReturnMap.find(*v->u.fnDef->fnDecl->id) == funcReturnMap.end())
            {
                FuncType type;
                if (v->u.fnDef->fnDecl->type == nullptr)
                {
                    type.type = ReturnType::VOID_TYPE;
                }
                else if (v->u.fnDef->fnDecl->type->type == A_nativeTypeKind)
                {
                    type.type = ReturnType::INT_TYPE;
                }
                else if (v->u.fnDef->fnDecl->type->type == A_structTypeKind)
                {
                    type.type = ReturnType::STRUCT_TYPE;
                    type.structname = *v->u.fnDef->fnDecl->type->u.structType;
                }
                else
                {
                    assert(0);
                }
                funcReturnMap.emplace(*v->u.fnDef->fnDecl->id, std::move(type));
            }
            break;
        }
        default:
            assert(0);
            break;
        }
    }
    return defs;
}

std::vector<Func_local *> ast2llvmProg_second(aA_program p)
{
    vector<Func_local *> f_loc;
    for (const auto &v : p->programElements)
    {
        if (v->kind == A_programFnDefKind)
        {

            f_loc.push_back(ast2llvmFunc(v->u.fnDef));
        }
    }

    return f_loc;
}

Func_local *ast2llvmFunc(aA_fnDef f)
{
    /*
    struct Func_local
    {
        std::string name;
        LLVMIR::FuncType ret;
        std::vector<Temp_temp*> args;
        std::list<LLVMIR::L_stm*> irs;
        Func_local(const std::string _name,LLVMIR::FuncType _ret,const std::vector<Temp_temp*> &_args,const std::list<LLVMIR::L_stm*> &_irs)
            : name(_name), ret(_ret), args(_args), irs(_irs) {}
    };
    */
    string name = *f->fnDecl->id;
    FuncType ret = funcReturnMap[name];
    vector<Temp_temp *> args;
    vector<TempDef> args__;
    for (auto &x : f->fnDecl->paramDecl->varDecls)
    {
        switch (x->kind)
        {
        case A_varDeclType::A_varDeclScalarKind:
        {
            if (x->u.declScalar->type->type == A_dataType::A_nativeTypeKind)
            {

                Temp_temp *temp = Temp_newtemp_int();
                Temp_temp *temp_temp = Temp_newtemp_int_ptr(0);
                AS_operand *res = AS_Operand_Temp(temp_temp);
                emit_irs.push_back(L_Alloca(res));
                ast2llvmAssignment(res, AS_Operand_Temp(temp));

                args.push_back(temp);
                args__.push_back(TempDef(TempType::INT_TEMP, 0, string()));
                localVarMap.emplace(*x->u.declScalar->id, temp_temp);
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_struct_ptr(0, *x->u.declScalar->type->u.structType);
                args.push_back(temp);
                args__.push_back(TempDef(TempType::STRUCT_TEMP, 0, *x->u.declScalar->type->u.structType));

                localVarMap.emplace(*x->u.declScalar->id, temp);
            }
            break;
        }
        case A_varDeclType::A_varDeclArrayKind:
        {
            if (x->u.declArray->type->type == A_dataType::A_nativeTypeKind)
            {
                Temp_temp *temp = Temp_newtemp_int_ptr(-1);
                args.push_back(temp);
                args__.push_back(TempDef(TempType::INT_PTR, -1, string()));
                localVarMap.emplace(*x->u.declArray->id, temp);
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_struct_ptr(-1, *x->u.declArray->type->u.structType);
                args.push_back(temp);
                args__.push_back(TempDef(TempType::STRUCT_PTR, -1, *x->u.declArray->type->u.structType));
                localVarMap.emplace(*x->u.declArray->id, temp);
            }
            break;
        }
        default:
            break;
        }
    }
    if (funcalargs.find(name) == funcalargs.end())
        funcalargs.emplace(name, new L_funcdecl(name, args__, ret));
    std::vector<string> fnvector;
    for (auto &x : f->stmts)
    {

        // printf("line:%d,col:%d\n", x->pos->line, x->pos->col);
        ast2llvmBlock(x, nullptr, nullptr, fnvector);
        if (x->kind == A_codeBlockStmtType::A_returnStmtKind)
        {
            break;
        }
    }
    if (ret.type == ReturnType::VOID_TYPE)
    {
        emit_irs.push_back(L_Ret(nullptr));
    }
    else
    {
        emit_irs.push_back(L_Ret(AS_Operand_Const(-1)));
    }
    Func_local *res = new Func_local(name, ret, args, emit_irs);
    emit_irs.clear();
    localVarMap.clear();
    return res;
}
void ast2llvmDefDecl(aA_varDeclStmt varDeclStmt, std::vector<string> &tempVector)
{
    if (varDeclStmt->kind == A_varDeclKind) // 变量声明
    {
        if (varDeclStmt->u.varDecl->kind == A_varDeclScalarKind) // 标量
        {
            if (varDeclStmt->u.varDecl->u.declScalar->type->type == A_structTypeKind) // 自定义结构体
            {
                Temp_temp *temp = Temp_newtemp_struct_ptr(0, *varDeclStmt->u.varDecl->u.declScalar->type->u.structType);
                localVarMap.emplace(*varDeclStmt->u.varDecl->u.declScalar->id,
                                    temp);
                tempVector.push_back(*varDeclStmt->u.varDecl->u.declScalar->id);
                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_int_ptr(0);
                localVarMap.emplace(*varDeclStmt->u.varDecl->u.declScalar->id,
                                    temp);
                tempVector.push_back(*varDeclStmt->u.varDecl->u.declScalar->id);

                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
            }
        }
        else if (varDeclStmt->u.varDecl->kind == A_varDeclArrayKind) // 数组
        {
            if (varDeclStmt->u.varDecl->u.declArray->type->type == A_structTypeKind)
            {
                int len = varDeclStmt->u.varDecl->u.declArray->len;
                string structname = *varDeclStmt->u.varDecl->u.declArray->type->u.structType;
                string id = *varDeclStmt->u.varDecl->u.declArray->id;
                Temp_temp *temp = Temp_newtemp_struct_ptr(len, structname);
                localVarMap.emplace(id,
                                    temp);
                tempVector.push_back(id);

                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_int_ptr(varDeclStmt->u.varDecl->u.declArray->len);
                localVarMap.emplace(*varDeclStmt->u.varDecl->u.declArray->id,
                                    temp);
                tempVector.push_back(*varDeclStmt->u.varDecl->u.declArray->id);

                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
            }
        }
        else
        {
            assert(0);
        }
    }
    else if (varDeclStmt->kind == A_varDefKind)
    {
        if (varDeclStmt->u.varDef->kind == A_varDefScalarKind)
        {
            if (varDeclStmt->u.varDef->u.defScalar->type->type == A_structTypeKind)
            {
                assert(0);
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_int_ptr(0);
                localVarMap.emplace(*varDeclStmt->u.varDef->u.defScalar->id,
                                    temp);
                tempVector.push_back(*varDeclStmt->u.varDef->u.defScalar->id);

                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
                AS_operand *right = ast2llvmRightVal(varDeclStmt->u.varDef->u.defScalar->val);
                ast2llvmAssignment(AS_Operand_Temp(temp), right);
            }
        }
        else if (varDeclStmt->u.varDef->kind == A_varDefArrayKind)
        {
            if (varDeclStmt->u.varDef->u.defArray->type->type == A_structTypeKind)
            {
                assert(0);
            }
            else
            {
                Temp_temp *temp = Temp_newtemp_int_ptr(varDeclStmt->u.varDef->u.defArray->len);
                localVarMap.emplace(*varDeclStmt->u.varDef->u.defArray->id,
                                    temp);
                tempVector.push_back(*varDeclStmt->u.varDef->u.defArray->id);

                emit_irs.push_back(L_Alloca(AS_Operand_Temp(temp)));
                int i = 0;
                AS_operand *base_ptr = AS_Operand_Temp(temp);
                for (auto rightval_ : varDeclStmt->u.varDef->u.defArray->vals)
                {
                    AS_operand *index = AS_Operand_Const(i);
                    AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
                    emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
                    AS_operand *right__ = ast2llvmRightVal(rightval_);
                    ast2llvmAssignment(new_ptr, right__);
                    i++;
                }
                for (; i < varDeclStmt->u.varDef->u.defArray->len; i++)
                {
                    AS_operand *index = AS_Operand_Const(i);
                    AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
                    emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
                    AS_operand *right__ = AS_Operand_Const(0);
                    ast2llvmAssignment(new_ptr, right__);
                    i++;
                }
            }
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
    }
}
void ast2llvmBlock(aA_codeBlockStmt b, Temp_label *con_label, Temp_label *bre_label, std::vector<string> &tempVector)
{

    aA_codeBlockStmt x = b;
    switch (x->kind)
    {
    case A_codeBlockStmtType::A_nullStmtKind:
    {
        break;
    }
    case A_codeBlockStmtType::A_varDeclStmtKind:
    {
        ast2llvmDefDecl(x->u.varDeclStmt, tempVector);
        break;
    }
    case A_codeBlockStmtType::A_assignStmtKind:
    {

        AS_operand *left = ast2llvmLeftVal(x->u.assignStmt->leftVal);
        AS_operand *right = ast2llvmRightVal(x->u.assignStmt->rightVal);
        ast2llvmAssignment(left, right);
        break;
    }
    case A_codeBlockStmtType::A_callStmtKind:
    {

        ast2llvmFnCall(x->u.callStmt->fnCall);
        break;
    }
    case A_codeBlockStmtType::A_ifStmtKind:
    {

        ast2llvmBlockIf(x->u.ifStmt, con_label, bre_label);
        break;
    }
    case A_codeBlockStmtType::A_whileStmtKind:
    {

        string name1 = "bb_while_" + to_string(x->pos->col) + "_" + to_string(x->pos->line);
        Temp_label *new_con = Temp_newlabel_named(name1 + "_cond");
        Temp_label *new_bre = Temp_newlabel_named(name1 + "_bre");
        Temp_label *true_label = Temp_newlabel_named(name1 + "_whilestms");
        emit_irs.push_back(L_Jump(new_con));
        emit_irs.push_back(L_Label(new_con));
        std::vector<string> whileVector;
        ast2llvmBoolExpr(x->u.whileStmt->boolExpr, true_label, new_bre);
        emit_irs.push_back(L_Label(true_label));

        for (auto &xx : x->u.whileStmt->whileStmts)
        {
            ast2llvmBlock(xx, new_con, new_bre, whileVector);
            if (xx->kind == A_codeBlockStmtType::A_returnStmtKind)
            {
                break;
            }
        }
        for (auto &x : whileVector)
        {
            localVarMap.erase(x);
        }
        emit_irs.push_back(L_Jump(new_con));
        emit_irs.push_back(L_Label(new_bre));

        break;
    }
    case A_codeBlockStmtType::A_returnStmtKind:
    {

        if (x->u.returnStmt->retVal == nullptr)
        {
            emit_irs.push_back(L_Ret(nullptr));
        }
        else
        {
            emit_irs.push_back(L_Ret(LOAD(ast2llvmRightVal(x->u.returnStmt->retVal))));
        }
        break;
    }
    case A_codeBlockStmtType::A_continueStmtKind:
    {
        if (con_label == nullptr)
        {
            assert(0);
        }
        emit_irs.push_back(L_Jump(con_label));
        break;
    }
    case A_codeBlockStmtType::A_breakStmtKind:
    {
        if (bre_label == nullptr)
        {
            assert(0);
        }
        emit_irs.push_back(L_Jump(bre_label));
        break;
    }
    default:
        break;
    }
}
void ast2llvmBlockIf(aA_ifStmt b, Temp_label *con_label, Temp_label *bre_label)
{
    string name1 = "bb_if_" + to_string(b->pos->col) + "_" + to_string(b->pos->line);
    Temp_label *true_label = Temp_newlabel_named(name1 + "_true");
    Temp_label *false_label = Temp_newlabel_named(name1 + "_false");
    Temp_label *end_label = Temp_newlabel_named(name1 + "_end");
    ast2llvmBoolExpr(b->boolExpr, true_label, false_label);
    emit_irs.push_back(L_Label(true_label));
    std::vector<string> ifVector;
    for (auto x : b->ifStmts)
    {
        ast2llvmBlock(x, con_label, bre_label, ifVector);
        if (x->kind == A_codeBlockStmtType::A_returnStmtKind)
        {
            break;
        }
    }
    for (auto &x : ifVector)
    {
        localVarMap.erase(x);
    }
    ifVector.clear();
    emit_irs.push_back(L_Jump(end_label));
    emit_irs.push_back(L_Label(false_label));
    for (auto x : b->elseStmts)
    {
        ast2llvmBlock(x, con_label, bre_label, ifVector);
        if (x->kind == A_codeBlockStmtType::A_returnStmtKind)
        {
            break;
        }
    }
    for (auto &x : ifVector)
    {
        localVarMap.erase(x);
    }
    ifVector.clear();
    emit_irs.push_back(L_Jump(end_label));
    emit_irs.push_back(L_Label(end_label));
}
AS_operand *ast2llvmRightVal(aA_rightVal r)
{

    switch (r->kind)
    {
    case A_rightValType::A_arithExprValKind:
        return ast2llvmArithExpr(r->u.arithExpr);
        /* code */
        break;
    case A_rightValType::A_boolExprValKind:
    {
        string name1 = "bb" + to_string(r->pos->col) + "_" + to_string(r->pos->line);
        Temp_label *my_true_label = Temp_newlabel_named(name1 + "true");
        Temp_label *my_false_label = Temp_newlabel_named(name1 + "false");
        Temp_label *my_end_label = Temp_newlabel_named(name1 + "end");
        AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        emit_irs.push_back(L_Alloca(dst));
        ast2llvmBoolExpr(r->u.boolExpr, my_true_label, my_false_label);
        emit_irs.push_back(L_Label(my_true_label));
        emit_irs.push_back(L_Store(AS_Operand_Const(1), dst));
        emit_irs.push_back(L_Jump(my_end_label));
        emit_irs.push_back(L_Label(my_false_label));
        emit_irs.push_back(L_Store(AS_Operand_Const(0), dst));
        emit_irs.push_back(L_Jump(my_end_label));

        emit_irs.push_back(L_Label(my_end_label));
        AS_operand *res = AS_Operand_Temp(Temp_newtemp_int());
        emit_irs.push_back(L_Load(res, dst));

        return res;
        break;
    }

    default:
        break;
    }
}

AS_operand *ast2llvmLeftVal(aA_leftVal l) // 说明左值返回的一定是ptr
{
    switch (l->kind)
    {
    case A_leftValType::A_arrValKind:
        return ast2llvmArray(l->u.arrExpr);
    case A_leftValType::A_memberValKind:
        return ast2llvmMember(l->u.memberExpr);
    case A_leftValType::A_varValKind:
        return ast2llvmId(*l->u.id);
    default:
        break;
    }
    return nullptr;
}
AS_operand *ast2llvmArray(aA_arrayExpr arrExpr)
{

    AS_operand *base_ptr = ast2llvmLeftVal(arrExpr->arr);
    AS_operand *index = LOAD(ast2llvmIndexExpr(arrExpr->idx));

    // 不知道返回值的类型 a[6].b[7]

    AS_operand *new_ptr = nullptr;
    if (base_ptr->kind == OperandKind::NAME)
    {
        Name_name *NAME = base_ptr->u.NAME;
        if (NAME->type == TempType::INT_PTR)
        {
            new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }
        else
        {
            new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(0, NAME->structname));
        }
    }
    else
    {
        Temp_temp *TEMP = base_ptr->u.TEMP;
        if (TEMP->type == TempType::INT_PTR)
        {
            // 当len为0时代表单个元素的指针，当len为-1时代表数组头指针，这种情况只在数组作为函数参数时存在，当len大于0时代表数组的长度。
            new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        }
        else
        {
            new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(0, TEMP->structname));
        }
    }

    emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
    return new_ptr;
}
AS_operand *ast2llvmMember(aA_memberExpr memberExpr)
{

    AS_operand *base_ptr = ast2llvmLeftVal(memberExpr->structId);
    string structName = std::string();
    if (base_ptr->kind == OperandKind::TEMP)
    {
        Temp_temp *TEMP = base_ptr->u.TEMP;
        structName = TEMP->structname;
    }
    else
    {
        Name_name *NAME = base_ptr->u.NAME;
        structName = NAME->structname;
    }
    StructInfo structInfo = structInfoMap[structName];
    MemberInfo memberInfo = structInfo.memberinfos[*memberExpr->memberId];
    AS_operand *index = AS_Operand_Const(memberInfo.offset);

    AS_operand *new_ptr = nullptr;
    // 根据TempDef生成new_ptr
    switch (memberInfo.def.kind)
    {
    case TempType::INT_TEMP:
        new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(0));
        break;
    case TempType::INT_PTR:
        new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(memberInfo.def.len));
        break;
    case TempType::STRUCT_PTR:
        new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(memberInfo.def.len, memberInfo.def.structname));
        break;
    case TempType::STRUCT_TEMP:
        new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(0, memberInfo.def.structname));
        break;

    default:
        break;
    }

    emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
    return new_ptr;
}
void ast2llvmAssignment(AS_operand *left, AS_operand *right)
{

    AS_operand *right_ = LOAD(right);

    emit_irs.push_back(L_Store(right_, left));
}
AS_operand *ast2llvmIndexExpr(aA_indexExpr index)
{
    if (index->kind == A_numIndexKind)
    {
        return AS_Operand_Const(index->u.num);
    }
    else
    {
        return ast2llvmId(*index->u.id);
    }
}

AS_operand *ast2llvmBoolExpr(aA_boolExpr b, Temp_label *true_label, Temp_label *false_label)
{
    // let a:int = b<=c;

    switch (b->kind)
    {
    case A_boolExprType::A_boolBiOpExprKind:

        ast2llvmBoolBiOpExpr(b->u.boolBiOpExpr, true_label, false_label);

        /* code */
        break;
    case A_boolExprType::A_boolUnitKind:
        ast2llvmBoolUnit(b->u.boolUnit, true_label, false_label);

    default:
        break;
    }
    return nullptr;
}

void ast2llvmBoolBiOpExpr(aA_boolBiOpExpr b, Temp_label *true_label, Temp_label *false_label)
{
    // 带有中断 a||b&&c
    aA_boolExpr left = b->left;
    string name1 = "bb" + to_string(b->right->pos->col) + "_" + to_string(b->right->pos->line);
    Temp_label *my_true_label = Temp_newlabel_named(name1 + "true");
    Temp_label *my_false_label = Temp_newlabel_named(name1 + "false");
    Temp_label *checksecond = Temp_newlabel_named(name1 + "checksecond");

    switch (b->op)
    {
    case A_boolBiOp::A_and:
        ast2llvmBoolExpr(b->left, checksecond, false_label);
        emit_irs.push_back(L_Label(checksecond));
        ast2llvmBoolExpr(b->right, true_label, false_label);

        /* code */
        break;
    case A_boolBiOp::A_or:
        ast2llvmBoolExpr(b->left, true_label, checksecond);
        emit_irs.push_back(L_Label(checksecond));
        ast2llvmBoolExpr(b->right, true_label, false_label);
        /* code */
        break;
    default:
        break;
    }
}

void ast2llvmBoolUnit(aA_boolUnit b, Temp_label *true_label, Temp_label *false_label)
{
    switch (b->kind)
    {
    case A_boolUnitType::A_boolExprKind:
        ast2llvmBoolExpr(b->u.boolExpr, true_label, false_label);
        /* code */
        break;
    case A_boolUnitType::A_boolUOpExprKind:
        ast2llvmBoolUnit(b->u.boolUOpExpr->cond, false_label, true_label);
        /* code */
        break;
    case A_boolUnitType::A_comOpExprKind:
        ast2llvmComOpExpr(b->u.comExpr, true_label, false_label);
        /* code */
        break;
    default:
        break;
    }
}

void ast2llvmComOpExpr(aA_comExpr c, Temp_label *true_label, Temp_label *false_label)
{
    AS_operand *left = LOAD(ast2llvmExprUnit(c->left));
    AS_operand *right = LOAD(ast2llvmExprUnit(c->right));
    AS_operand *temp = AS_Operand_Temp(Temp_newtemp_int());

    switch (c->op)
    {
    case A_comOp::A_eq:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_eq, left, right, temp));
    }
    /* code */
    break;
    case A_comOp::A_ge:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_ge, left, right, temp));
    }
    /* code */
    break;
    case A_comOp::A_gt:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_gt, left, right, temp));
    }
    /* code */
    break;
    case A_comOp::A_le:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_le, left, right, temp));
    }
    /* code */
    break;
    case A_comOp::A_lt:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_lt, left, right, temp));
    }
    /* code */
    break;
    case A_comOp::A_ne:
    {
        emit_irs.push_back(L_Cmp(L_relopKind::T_ne, left, right, temp));
    }
    /* code */
    break;
    default:
        break;
    }
    emit_irs.push_back(L_Cjump(temp, true_label, false_label));
}
AS_operand *LOAD(AS_operand *left)
{
    switch (left->kind)
    {
    case OperandKind::TEMP:
    {
        switch (left->u.TEMP->type)
        {
        case TempType::INT_PTR:
        {

            AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.push_back(L_Load(dst, left));
            return dst;
        }
        case TempType::INT_TEMP:
        {
            return left;
            break;
        }
        case TempType::STRUCT_TEMP:
        {
            return left;
            break;
        }
        case TempType::STRUCT_PTR:
        {

            return left;
            break;
        }
        default:
            return left;
            break;
        }
    }
    break;
    case OperandKind::NAME:
    {
        switch (left->u.NAME->type)
        {
        case TempType::INT_PTR:
        {

            AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.push_back(L_Load(dst, left));
            return dst;
        }
        case TempType::INT_TEMP:
        {
            AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.push_back(L_Load(dst, left));
            return dst;
            break;
        }
        case TempType::STRUCT_PTR:
        {
            return left;
            break;
        }
        case TempType::STRUCT_TEMP:
        {
            return left;
            break;
        }
        default:
            return left;
            break;
        }
    }
    /* code */
    break;
    case OperandKind::ICONST:
        return left;
        break;

    default:
        break;
    }
}
AS_operand *fnarg(AS_operand *left, TempDef &tempdef)
{

    switch (left->kind)
    {
    case OperandKind::TEMP:
    {
        switch (left->u.TEMP->type)
        {
        case TempType::INT_PTR:
        {
            if (tempdef.kind == TempType::INT_TEMP)
            {
                AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
                emit_irs.push_back(L_Load(dst, left));

                return dst;
            }
            else if (tempdef.kind == TempType::INT_PTR)
            {
                if (left->u.TEMP->len > 0)
                {
                    AS_operand *index = AS_Operand_Const(0);
                    AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(-1));
                    emit_irs.push_back(L_Gep(new_ptr, left, index));

                    return new_ptr;
                }
                else
                {

                    return left;
                }

                break;
            }
        }
        case TempType::INT_TEMP:
        {
            return left;
            break;
        }
        case TempType::STRUCT_TEMP:
        {
            return left;
        }
        case TempType::STRUCT_PTR:
        {
            if (left->u.TEMP->len > 0)
            {
                AS_operand *base_ptr = left;
                AS_operand *index = AS_Operand_Const(0);
                AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(-1, left->u.TEMP->structname));

                emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
                return new_ptr;
            }
            else
            {
                return left;
            }
            break;
        }
        default:
            return left;
            break;
        }
    }
    break;
    case OperandKind::NAME:
    {
        switch (left->u.NAME->type)
        {
        case TempType::INT_PTR:
        {
            if (tempdef.kind == TempType::INT_TEMP)
            {
                AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
                emit_irs.push_back(L_Load(dst, left));
                return dst;
                break;
            }
            else
            {
                if (left->u.NAME->len > 0)
                {
                    AS_operand *index = AS_Operand_Const(0);
                    AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_int_ptr(-1));
                    emit_irs.push_back(L_Gep(new_ptr, left, index));

                    return new_ptr;
                }
                else
                {
                    return left;
                }

                break;
            }
        }
        case TempType::INT_TEMP:
        {
            AS_operand *dst = AS_Operand_Temp(Temp_newtemp_int());
            emit_irs.push_back(L_Load(dst, left));
            return dst;
            break;
        }
        case TempType::STRUCT_TEMP:
        {
            return left;
            break;
        }
        case TempType::STRUCT_PTR:
        {

            if (left->u.NAME->len > 0)
            {
                AS_operand *base_ptr = left;
                AS_operand *index = AS_Operand_Const(0);
                AS_operand *new_ptr = AS_Operand_Temp(Temp_newtemp_struct_ptr(-1, left->u.NAME->structname));

                emit_irs.push_back(L_Gep(new_ptr, base_ptr, index));
                return new_ptr;
            }
            else
            {
                return left;
            }

            break;
        }

        default:
            return left;
            break;
        }
    }
    /* code */
    break;
    case OperandKind::ICONST:
    {
        if (tempdef.kind == TempType::INT_PTR)
        {
            AS_operand *res = AS_Operand_Temp(Temp_newtemp_int_ptr(0));

            emit_irs.push_back(L_Alloca(res));
            emit_irs.push_back(L_Store(left, res));
            return res;
        }
        else
        {
            return left;
        }
    }
    break;

    default:
        break;
    }
}
AS_operand *ast2llvmArithBiOpExpr(aA_arithBiOpExpr a)
{
    // 如果有一个是bool变量怎么办？
    // bool表达式到int类型的转换。暂时不会出现。但是左值和右值之间可能出现
    // a.c+b.d+c
    AS_operand *left = LOAD(ast2llvmArithExpr(a->left));
    AS_operand *right = LOAD(ast2llvmArithExpr(a->right));
    AS_operand *res = AS_Operand_Temp(Temp_newtemp_int());
    switch (a->op)
    {
    case A_arithBiOp::A_add:
        emit_irs.push_back(L_Binop(L_binopKind::T_plus, left, right, res));
        /* code */
        break;
    case A_arithBiOp::A_sub:
        emit_irs.push_back(L_Binop(L_binopKind::T_minus, left, right, res));
        /* code */
        break;
    case A_arithBiOp::A_mul:
        emit_irs.push_back(L_Binop(L_binopKind::T_mul, left, right, res));
        /* code */
        break;
    case A_arithBiOp::A_div:
        emit_irs.push_back(L_Binop(L_binopKind::T_div, left, right, res));
        /* code */
        break;
    default:
        break;
    }
    return res;
}

AS_operand *ast2llvmArithUExpr(aA_arithUExpr a)
{
    // 只能是int，希望是
    AS_operand *zero = AS_Operand_Const(0);
    AS_operand *temp = LOAD(ast2llvmExprUnit(a->expr));
    AS_operand *neg = AS_Operand_Temp(Temp_newtemp_int());

    emit_irs.push_back(L_Binop(L_binopKind::T_minus, zero, temp, neg));
    return neg;
}

AS_operand *ast2llvmArithExpr(aA_arithExpr a)
{

    switch (a->kind)
    {
    case A_arithExprType::A_arithBiOpExprKind:
        return ast2llvmArithBiOpExpr(a->u.arithBiOpExpr);
        /* code */
        break;
    case A_arithExprType::A_exprUnitKind:
        return ast2llvmExprUnit(a->u.exprUnit);
        break;

    default:
        break;
    }
}
AS_operand *ast2llvmId(string id) // 返回指针
{

    if (localVarMap.find(id) != localVarMap.end())
    {
        // if a.b.c
        // 返回a的ptr

        return AS_Operand_Temp(localVarMap[id]);
    }
    else
    { // llvm-as: test.ll:20:16: error: global variable reference must have pointer type

        return AS_Operand_Name(globalVarMap[id]);
    }
}
AS_operand *ast2llvmExprUnit(aA_exprUnit e)
{

    switch (e->kind)
    {
    case A_numExprKind:
        return AS_Operand_Const(e->u.num);
        /* code */
        break;
    case A_idExprKind:
    {

        return ast2llvmId(*e->u.id);
    }
    break;
    case A_arithExprKind:
    {
        return ast2llvmArithExpr(e->u.arithExpr);
    }
    break;
    case A_fnCallKind:
    {
        return ast2llvmFnCall(e->u.callExpr);
    }
    break;
    case A_arrayExprKind:
    {

        return ast2llvmArray(e->u.arrayExpr);
    }
    break;
    case A_memberExprKind:
    {

        return ast2llvmMember(e->u.memberExpr);
    }
    break;
    case A_arithUExprKind:
    {

        return ast2llvmArithUExpr(e->u.arithUExpr);
    }

    break;

    default:
        break;
    }
}
AS_operand *ast2llvmFnCall(aA_fnCall callExpr)
{

    vector<AS_operand *> args;
    string name = *callExpr->fn;
    for (int i = 0; i < callExpr->vals.size(); i++)
    {

        AS_operand *left = ast2llvmRightVal(callExpr->vals[i]);

        TempDef &tempdef = funcalargs[name]->args[i];

        AS_operand *res = fnarg(left, tempdef);

        args.push_back(res);
    }

    AS_operand *res = nullptr;
    FuncType funcType = funcReturnMap[name];

    if (funcType.type == ReturnType::VOID_TYPE)
    {
        emit_irs.push_back(L_Voidcall(name, args));
    }
    else if (funcType.type == ReturnType::INT_TYPE)
    {
        res = AS_Operand_Temp(Temp_newtemp_int());
        emit_irs.push_back(L_Call(name, res, args));
    }
    else
    {
        // 如果返回的是struct，那么拿到的是指针吗？
        res = AS_Operand_Temp(Temp_newtemp_struct_ptr(0, funcType.structname));
        emit_irs.push_back(L_Call(name, res, args));
    }

    return res;
}
LLVMIR::L_func *ast2llvmFuncBlock(Func_local *f)
{

    list<L_block *> blocks;

    std::list<L_stm *> instrs;
    std::unordered_set<Temp_label *> succs;
    Temp_label *label;
    L_block *lblock = new L_block(Temp_newlabel(), succs, instrs);
    lblock->instrs.push_back(L_Label(lblock->label));
    bool hungry = true;
    bool before_ret = true;
    for (auto x : f->irs)
    {
        switch (x->type)
        {
        case L_StmKind::T_LABEL:
        {
            blocks.push_back(lblock);
            if (hungry)
            {
                lblock->succs.emplace(x->u.LABEL->label);
            }
            lblock = new L_block(x->u.LABEL->label, succs, instrs);
            hungry = true;
            before_ret = true;
            lblock->instrs.push_back(x);
            break;
        }
        case L_StmKind::T_JUMP:
        {
            if (hungry && before_ret)
            {
                lblock->succs.emplace(x->u.JUMP->jump);
                lblock->instrs.push_back(x);
            }

            hungry = false;
            break;
        }
        case L_StmKind::T_CJUMP:
        {
            if (hungry && before_ret)
            {
                lblock->succs.emplace(x->u.CJUMP->true_label);
                lblock->succs.emplace(x->u.CJUMP->false_label);
                lblock->instrs.push_back(x);
            }
            hungry = false;

            break;
        }
        case L_StmKind::T_RETURN:
        {
            if (before_ret)
            {
                lblock->instrs.push_back(x);
            }
            before_ret = false;
            break;
        }
        default:
        {
            if (before_ret)
                lblock->instrs.push_back(x);
            break;
        }
        }
    }
    blocks.push_back(lblock);

    return new L_func(f->name, f->ret, f->args, blocks);
}

void ast2llvm_moveAlloca(LLVMIR::L_func *f)
{

    std::list<L_stm *> instrs;
    std::unordered_set<Temp_label *> succs;
    Temp_label *label;
    auto it = f->blocks.begin();
    if (*it != nullptr)
    {
        succs.emplace((*it)->label);
    }
    L_block *lblock = new L_block(Temp_newlabel(), succs, instrs);
    lblock->instrs.emplace_front(L_Label(lblock->label));
    int i = 0;
    // 新建一个空的block
    for (auto &x : f->blocks)
    {

        list<L_stm *>::iterator it = x->instrs.begin();
        for (; it != x->instrs.end();)
        {
            if ((*it)->type == L_StmKind::T_ALLOCA)
            {
                lblock->instrs.emplace_back(*it);
                it = x->instrs.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
    lblock->instrs.emplace_back(L_Jump(f->blocks.front()->label));
    f->blocks.emplace_front(lblock);
    return;
}