#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include "TeaplAst.h"
#include "TeaplaAst.h"
#include <unordered_map>
#include <vector>
typedef enum
{
    Scalar,
    Array
} A_Scalar_Array;
struct My_type
{
    aA_type type;
    A_Scalar_Array kind;
    int len;
    My_type(const aA_type t, const A_Scalar_Array k, int l = -1)
    {
        type = t;
        kind = k;
        len = l;
    }
    My_type(const My_type &t)
    {
        type = t.type;
        kind = t.kind;
        len = t.len;
    }
    My_type(const aA_varDecl varDecls)
    {
        switch (varDecls->kind)
        {
        case A_varDeclScalarKind:
        {
            auto y = varDecls->u.declScalar;
            type = y->type;
            kind = Scalar;
            len = 0;
            break;
        }

        case A_varDeclArrayKind:
        {
            auto y = varDecls->u.declArray;
            type = y->type;
            kind = Array;
            len = y->len;
            break;
        }

        default:
            break;
        }
    }
    bool com_type(const My_type&t)
    {
        if(t.kind!=kind)
        {
            return false;
        }
        if(t.type->type!=type->type)
        {
            return false;
        }
        //要么都是native，要么都是struct
        if(t.type->type==A_nativeTypeKind)
        {
            return true;
        }else{
            if(*t.type->u.structType!=*type->u.structType)
            {
                return false;
            }
        }
        return true;
    }
};
class FunctionParameterList
{
public:
    bool addParameter(const std::string &name, My_type *type)
    {
        // 检查是否存在具有相同名称的参数
        for (const auto &param : parameters)
        {
            if (param.first == name)
            {
                return false; // 参数名称重复，不合法
            }
        }
        // 添加参数到列表
        parameters.push_back(std::make_pair(name, type));
        return true; // 参数添加成功
    }

    My_type *getParameterTypeByIndex(int index)
    {
        if (index >= 0 && index < parameters.size())
        {
            return parameters[index].second;
        }
        return nullptr;
    }

    std::string getParameterNameByIndex(int index)
    {
        if (index >= 0 && index < parameters.size())
        {
            return parameters[index].first;
        }
        return "";
    }

    My_type *getParameterTypeByName(const std::string &name)
    {
        for (const auto &param : parameters)
        {
            if (param.first == name)
            {
                return param.second;
            }
        }
        return nullptr;
    }
    int getParameterLength()
    {
        return parameters.size();
    }
    std::string toString()
    {
        std::string result;
        for (const auto &param : parameters)
        {
            result += "My_type* " + param.first + ", ";
        }
        if (!result.empty())
        {
            result = result.substr(0, result.length() - 2); // Remove the trailing comma and space
        }
        return result;
    }

private:
    std::vector<std::pair<std::string, My_type *>> parameters;
};
// you can use this type to store the type a token.
typedef std::unordered_map<string, My_type *> typeMap;

// you can use this map to store the members of a struct or params of a function.
typedef std::unordered_map<string, FunctionParameterList *> paramMemberMap;

void check_Prog(std::ostream *out, aA_program p);
void check_VarDecl(std::ostream *out, aA_varDeclStmt vd,vector<string>& tempname);
void check_StructDef(std::ostream *out, aA_structDef sd);
void check_FnDecl(std::ostream *out, aA_fnDecl fd);
void check_FnDeclStmt(std::ostream *out, aA_fnDeclStmt fd);
void check_FnDef(std::ostream *out, aA_fnDef fd);
void check_FnDefBlock(std::ostream *out, aA_fnDef fd);
void check_CodeblockStmt(std::ostream *out, aA_codeBlockStmt cs,vector<string>& blockScope);
void check_AssignStmt(std::ostream *out, aA_assignStmt as);
My_type *check_ArrayExpr(std::ostream *out, aA_arrayExpr ae);
My_type *check_MemberExpr(std::ostream *out, aA_memberExpr me);
My_type *check_IdExpr(std::ostream *out,A_pos pos, string id);
void check_IfStmt(std::ostream *out, aA_ifStmt is);
// void check_BoolExpr(std::ostream* out, aA_boolExpr be);
// void check_BoolUnit(std::ostream* out, aA_boolUnit bu);
// aA_type check_ExprUnit(std::ostream* out, aA_exprUnit eu);
My_type *check_FuncCall(std::ostream *out, aA_fnCall fc);
void check_WhileStmt(std::ostream *out, aA_whileStmt ws);
void check_CallStmt(std::ostream *out, aA_callStmt cs);
void check_ReturnStmt(std::ostream *out, aA_returnStmt rs);
// rightVal Check
My_type *check_RightVal(std::ostream *out, aA_rightVal rv);
My_type *check_ArithExpr(std::ostream *out, aA_arithExpr ae);
My_type *check_ArithBiOpExpr(std::ostream *out, aA_arithBiOpExpr abe);
My_type *check_ExprUnit(std::ostream *out, aA_exprUnit eu);
My_type *check_ArithUExpr(std::ostream *out, aA_arithUExpr eue);

My_type *check_BoolExpr(std::ostream *out, aA_boolExpr be);
My_type *check_BoolBiOpExpr(std::ostream *out, aA_boolBiOpExpr bbe);
My_type *check_BoolUnit(std::ostream *out, aA_boolUnit bu);
My_type *check_ComExpr(std::ostream *out, aA_comExpr ce);
My_type *check_BoolUOpExpr(std::ostream *out, aA_boolUOpExpr bue);
