#include "printASM.h"
#include "asm_arm.h"
#include <iostream>

using namespace std;
using namespace ASM;

void ASM::printAS_global(std::ostream &os, ASM::AS_global *global)
{

    // Fixme: add here
    os << global->label->name << ":\n\n";
    if (global->init == 0)
    {
        os << "        .zero   " << global->len << endl;
    }
    else
    {
        os << "        .word   " << global->init << endl;
    }
}

void ASM::printAS_decl(std::ostream &os, ASM::AS_decl *decl)
{
    // Fixme: add here
    os << ".global " << decl->name << endl;
}

void ASM::printAS_stm(std::ostream &os, AS_stm *stm)
{
    // Fixme: add here
    switch (stm->type)
    {
    case AS_stmkind::BINOP:
    {
        string temp[] = {"add", "sub", "mul", "sdiv"};
        os << "        " << temp[static_cast<int>(stm->u.BINOP->op)] << "     ";
        os << printAS_reg(stm->u.BINOP->dst);
        os << ", ";
        os << printAS_reg(stm->u.BINOP->left);
        os << ", ";
        os << printAS_reg(stm->u.BINOP->right);
        os << endl;

        break;
    }
    case AS_stmkind::MOV:
    {
        os << "        mov     ";
        os << printAS_reg(stm->u.MOV->dst);
        os << ", ";
        os << printAS_reg(stm->u.MOV->src);
        os << endl;

        break;
    }
    case AS_stmkind::LDR:
    {
        os << "        ldr     ";
        os << printAS_reg(stm->u.LDR->dst);
        os << ", [";
        os << printAS_reg(stm->u.LDR->ptr, false);
        os << "]";
        os << endl;

        break;
    }
    case AS_stmkind::STR:
    {
        os << "        str     ";
        os << printAS_reg(stm->u.STR->ptr);
        os << ", [";
        os << printAS_reg(stm->u.STR->src, false);
        os << "]";
        os << endl;

        break;
    }
    case AS_stmkind::LABEL:
    {
        os << stm->u.LABEL->name << ":\n\n";
        break;
    }
    case AS_stmkind::B:
    {
        os << "        "
           << "b"
           << "     " << stm->u.B->jump->name;
        os << endl;

        break;
    }
    case AS_stmkind::BCOND:
    {
        string temp[] = {"eq", "ne", "lt", "gt", "le", "ge"};
        os << "        "
           << "b." << temp[static_cast<int>(stm->u.BCOND->op)] << "     " << stm->u.BCOND->jump->name;
        os << endl;

        break;
    }
    case AS_stmkind::BL:
    {
        os << "        "
           << "bl"
           << "     " << stm->u.BL->jump->name;
        os << endl;

        break;
    }
    case AS_stmkind::CMP:
    {
        os << "        cmp     ";
        os << printAS_reg(stm->u.CMP->left);
        os << ", ";
        os << printAS_reg(stm->u.CMP->right);
        os << endl;

        break;
    }
    case AS_stmkind::RET:
    {
        os << "        ret";
        os << endl;

        break;
    }
    case AS_stmkind::ADR:
    {
        os << "        adr     ";
        os << printAS_reg(stm->u.ADR->reg);
        os << ", " << stm->u.ADR->label->name;
        os << endl;

        break;
    }

    default:
        break;
    }
}

string ASM::printAS_reg(AS_reg *reg, bool integer)
{
    // Fixme: add here
    string res = "";
    if (reg->reg >= 0)
    {
        res += "x" + to_string(reg->reg);
    }
    else if (reg->reg == -3)
    {
        res += "#" + to_string(reg->offset);
    }
    else if (reg->reg == -1)
    {
        if (reg->offset != -1)
        {
            res += "sp, #" + to_string(reg->offset);
        }
        else if (reg->offset == -1)
        {
            res += "sp";
        }
    }
    if (integer)
    {
        if (res.substr(0, 2) == "sp" && res.length() > 2)
        {
            res = "[" + res + "]";
        }
    }
    return res;
}

void ASM::printAS_func(std::ostream &os, AS_func *func)
{
    for (const auto &stm : func->stms)
    {
        printAS_stm(os, stm);
    }
}

void ASM::printAS_prog(std::ostream &os, AS_prog *prog)
{

    os << ".section .data\n\n";
    for (const auto &global : prog->globals)
    {
        printAS_global(os, global);
    }

    os << ".section .text\n\n";
    // os << ".global _start\n\
// \n\
// _start:\n\
//     b main  \n\
//     mov x8, #93  \n\
//     mov x0, #0   \n\
//     svc #0       \n";
    for (const auto &decl : prog->decls)
    {
        printAS_decl(os, decl);
    }

    for (const auto &func : prog->funcs)
    {
        printAS_func(os, func);
    }
}