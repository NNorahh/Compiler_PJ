#include "llvm_ir.h"
#include "asm_arm.h"
#include "temp.h"
#include "llvm2asm.h"

#include <queue>
#include <cassert>
#include <iostream>
using namespace std;
using namespace LLVMIR;
using namespace ASM;

#define INSERT1() printf("%s:%d\n", __FILE__, __LINE__);
static int stack_frame;
static bool alloc_frame = false;

static unordered_map<int, int> spOffsetMap;
static unordered_map<int, AS_relopkind> condMap;
static unordered_map<string, int> structLayout;
static AS_reg *SP = new AS_reg(-1, -1);
void structLayoutInit(vector<L_def *> &defs)
{
    for (const auto &def : defs)
    {
        switch (def->kind)
        {
        case L_DefKind::SRT:
        {

            // Fixme: add here
            // 所有结构的体的定义，每个域只有整形值，不考虑结构题中有数组或嵌套其他结构体。但我们允许声明一个变量为结构体数组（但不能在域内）。
            structLayout[def->u.SRT->name] = def->u.SRT->members.size() * 4;
        }
        case L_DefKind::GLOBAL:
        {
            break;
        }
        case L_DefKind::FUNC:
        {
            break;
        }
        }
    }
}

void set_stack(L_func &func)
{
    // Fixme: add here
    stack_frame = 0;
    for (auto &x : func.blocks)
    {
        for (auto &y : x->instrs)
        {
            if (y->type == L_StmKind::T_ALLOCA)
            {
                auto alloca = y->u.ALLOCA->dst->u.TEMP;
                // printf("%d,", alloca->num);
                switch (alloca->type)
                {
                case TempType::INT_PTR:

                    stack_frame += max(4, 4 * alloca->len);
                    spOffsetMap[alloca->num] = -stack_frame;
                    // printf("alloca->len:%d,INT_PTR,stack_frame:%d\n", alloca->len,stack_frame);

                    break;
                case TempType::INT_TEMP:
                    assert(0);
                    break;
                case TempType::STRUCT_PTR:
                {
                    int temp = structLayout[alloca->structname];
                    stack_frame += max(temp, temp * alloca->len);
                    spOffsetMap[alloca->num] = -stack_frame;
                    // printf("temp:%d,STRUCT_PTR.stack_frame:%d\n",temp, stack_frame);

                    break;
                }

                case TempType::STRUCT_TEMP:
                    stack_frame += structLayout[alloca->structname];
                    spOffsetMap[alloca->num] = -stack_frame;
                    // printf("STRUCT_TEMP.stack_frame:%d\n", stack_frame);

                    break;
                }
            }
        }
    }
    stack_frame = ((stack_frame + 15) >> 4) << 4;
    for (auto &x : spOffsetMap)
    {
        x.second += stack_frame;
        printf("%r%d [sp,#%d]\n", x.first, x.second);
    }
}

void new_frame(list<AS_stm *> &as_list)
{
    as_list.emplace_back(AS_Mov(new AS_reg(-3, stack_frame), new AS_reg(2, 0)));
    // Fixme: add here
    as_list.emplace_back(AS_Binop(AS_binopkind::SUB_, SP, new AS_reg(2, 0), SP));
}

void free_frame(list<AS_stm *> &as_list)
{
    as_list.emplace_back(AS_Mov(new AS_reg(-3, stack_frame), new AS_reg(2, 0)));

    as_list.emplace_back(AS_Binop(AS_binopkind::ADD_, SP, new AS_reg(2, 0), SP));
}
void allignLeftvRight2(AS_reg *&op_reg, AS_operand *as_operand)
{
    switch (as_operand->kind)
    {
    case OperandKind::ICONST:
    {
        // store from the const: str #1, ...
        // do not need to move the instant into reg, use #1 directly
        int instant = as_operand->u.ICONST;
        op_reg = new AS_reg(-3, instant);
        break;
    }
    case OperandKind::TEMP:
    {
        // store from the reg: str x, ...
        int src_num = as_operand->u.TEMP->num;
        op_reg = new AS_reg(src_num, 0);
        break;
    }
    case OperandKind::NAME:
    {
        assert(0);
    }
    }
}
void allignLeftvRight(AS_reg *&op_reg, AS_operand *as_operand, list<AS_stm *> &as_list, int n = 2)
{
    if (n != 2 && n != 3)
    {
        /* LAB6 预留了 x2 x3 两个物理寄存器来暂存立即数。*/
        assert(0);
    }
    switch (as_operand->kind)
    {
    case OperandKind::ICONST:
    {

        // store from the const: str #1, ...
        // move the instant into x2: mov x2, #1
        int instant = as_operand->u.ICONST;
        AS_reg *src_mov = new AS_reg(-3, instant);

        AS_reg *dst_mov = new AS_reg(n, 0);
        as_list.push_back(AS_Mov(src_mov, dst_mov));
        op_reg = dst_mov;
        break;
    }
    case OperandKind::TEMP:
    {
        // store from the reg directly: str x, ...
        int src_num = as_operand->u.TEMP->num;
        op_reg = new AS_reg(src_num, 0);
        break;
    }
    case OperandKind::NAME:
    {
        assert(0);
    }
    }
}
void llvm2asmBinop(list<AS_stm *> &as_list, L_stm *binop_stm)
{
    AS_reg *left;
    AS_reg *right;
    AS_reg *dst;
    AS_binopkind op;
    AS_binopkind temp[] = {AS_binopkind::ADD_, AS_binopkind::SUB_, AS_binopkind::MUL_, AS_binopkind::SDIV_};
    op = temp[static_cast<int>(binop_stm->u.BINOP->op)];
    switch (binop_stm->u.BINOP->op)
    {
    case L_binopKind::T_plus:
    case L_binopKind::T_minus:
    {
        allignLeftvRight(left, binop_stm->u.BINOP->left, as_list, 2);
        allignLeftvRight2(right, binop_stm->u.BINOP->right);

        break;
    }

    case L_binopKind::T_mul:
    case L_binopKind::T_div:
    {
        allignLeftvRight(left, binop_stm->u.BINOP->left, as_list, 2);
        allignLeftvRight(right, binop_stm->u.BINOP->right, as_list, 3);
        break;
    }
    }

    // Fixme: add here
    dst = new AS_reg(binop_stm->u.BINOP->dst->u.TEMP->num, 0);
    as_list.push_back(AS_Binop(op, left, right, dst));
}
void allignPtr(AS_reg *&op_reg, AS_operand *as_operand, list<AS_stm *> &as_list, int n = 2)
{
    switch (as_operand->kind)
    {
    case OperandKind::TEMP:
    {
        if (spOffsetMap.find(as_operand->u.TEMP->num) != spOffsetMap.end())
        {
            op_reg = new AS_reg(-1, spOffsetMap[as_operand->u.TEMP->num]);
        }
        else
        {
            op_reg = new AS_reg(as_operand->u.TEMP->num, 0);
        }
        break;
    }
    case OperandKind::NAME:
    {
        op_reg = new AS_reg(n, 0);
        as_list.push_back(AS_Adr(new AS_label(as_operand->u.NAME->name->name), op_reg));
        break;
    }
    case OperandKind::ICONST:
    {
        assert(0);
        break;
    }

        /* code */

    default:
        break;
    }
}
void llvm2asmLoad(list<AS_stm *> &as_list, L_stm *load_stm)
{
    AS_reg *dst;
    AS_reg *ptr;
    dst = new AS_reg(load_stm->u.LOAD->dst->u.TEMP->num, 0);
    // Fixme: add here
    allignPtr(ptr, load_stm->u.LOAD->ptr, as_list);

    as_list.push_back(AS_Ldr(dst, ptr));
}

void llvm2asmStore(list<AS_stm *> &as_list, L_stm *store_stm)
{
    AS_reg *src;
    AS_reg *ptr;

    allignLeftvRight(src, store_stm->u.STORE->src, as_list);
    // Fixme: add here
    allignPtr(ptr, store_stm->u.STORE->ptr, as_list);
    as_list.push_back(AS_Str(ptr, src));
}

void llvm2asmCmp(list<AS_stm *> &as_list, L_stm *cmp_stm)
{
    // Fixme: add here
    AS_reg *left;
    AS_reg *right;
    AS_relopkind temp[] = {AS_relopkind::EQ_,
                           AS_relopkind::NE_,
                           AS_relopkind::LT_,
                           AS_relopkind::GT_,
                           AS_relopkind::LE_,
                           AS_relopkind::GE_};
    allignLeftvRight(left, cmp_stm->u.CMP->left, as_list, 2);

    allignLeftvRight(right, cmp_stm->u.CMP->right, as_list, 3);
    condMap[cmp_stm->u.CMP->dst->u.TEMP->num] = temp[static_cast<int>(cmp_stm->u.CMP->op)];
    as_list.push_back(AS_Cmp(left, right));
}

void llvm2asmCJmp(list<AS_stm *> &as_list, L_stm *cjmp_stm)
{
    // Fixme: add here
    as_list.push_back(AS_BCond(condMap[cjmp_stm->u.CJUMP->dst->u.TEMP->num], new AS_label(cjmp_stm->u.CJUMP->true_label->name)));
    as_list.push_back(AS_B(new AS_label(cjmp_stm->u.CJUMP->false_label->name)));
}

void llvm2asmRet(list<AS_stm *> &as_list, L_stm *ret_stm)
{
    // Fixme: add here
    as_list.push_back(AS_Ret());
}

void llvm2asmGep(list<AS_stm *> &as_list, L_stm *gep_stm)
{
    // Fixme: add here
    AS_reg *new_ptr;
    AS_reg *base_ptr;
    AS_reg *index_;
    auto gep = gep_stm->u.GEP;
    allignPtr(base_ptr, gep->base_ptr, as_list, 2);
    int bits = 0;
    switch (gep->base_ptr->kind)
    {
    case OperandKind::TEMP:
    {
        auto ttemp = gep->base_ptr->u.TEMP;
        switch (ttemp->type)
        {
        case TempType::INT_PTR:
        case TempType::STRUCT_TEMP:
        {
            bits = 4;
            break;
        }
        case TempType::STRUCT_PTR:
        {
            if (ttemp->len > 0)
            {
                bits = structLayout[ttemp->structname];
            }
            else
            {
                bits = 4;
            }
            break;
        }

        default:
            assert(0);
            break;
        }
        break;
    }
    case OperandKind::NAME:
    {
        auto nname = gep->base_ptr->u.NAME;
        switch (nname->type)
        {
        case TempType::INT_PTR:
        case TempType::STRUCT_TEMP:
        {
            bits = 4;
            break;
        }
        case TempType::STRUCT_PTR:
        {
            if (nname->len > 0)
            {
                bits = structLayout[nname->structname];
            }
            else
            {
                bits = 4;
            }
            break;
        }

        default:
            assert(0);
            break;
        }
        break;
    }

    default:
        assert(0);
        break;
    }
    if (gep_stm->u.GEP->index->kind != OperandKind::ICONST)
    {
        assert(0);
    }
    if (gep->base_ptr->kind == OperandKind::TEMP)
    {
        if (spOffsetMap.find(gep->base_ptr->u.TEMP->num) != spOffsetMap.end())
        {
            spOffsetMap[gep->new_ptr->u.TEMP->num] = base_ptr->offset - bits * gep->index->u.ICONST;
            return;
        }
    }
    index_ = new AS_reg(-3, bits * gep->index->u.ICONST);
    new_ptr = new AS_reg(gep->new_ptr->u.TEMP->num, 0);
    as_list.push_back(AS_Binop(AS_binopkind::ADD_, base_ptr, index_, new_ptr));
}

void llvm2asmStm(list<AS_stm *> &as_list, L_stm &stm)
{

    if (!alloc_frame && stm.type != L_StmKind::T_LABEL)
    {
        new_frame(as_list);
        alloc_frame = true;
    }

    switch (stm.type)
    {
    case L_StmKind::T_BINOP:
    {
        llvm2asmBinop(as_list, &stm);
        break;
    }
    case L_StmKind::T_LOAD:
    {
        llvm2asmLoad(as_list, &stm);
        break;
    }
    case L_StmKind::T_STORE:
    {
        llvm2asmStore(as_list, &stm);
        break;
    }
    case L_StmKind::T_LABEL:
    {
        auto label = new AS_label(stm.u.LABEL->label->name);
        as_list.push_back(AS_Label(label));
        break;
    }
    case L_StmKind::T_JUMP:
    {
        auto label = new AS_label(stm.u.JUMP->jump->name);
        as_list.push_back(AS_B(label));
        break;
    }
    case L_StmKind::T_CMP:
    {
        llvm2asmCmp(as_list, &stm);
        break;
    }
    case L_StmKind::T_CJUMP:
    {
        llvm2asmCJmp(as_list, &stm);
        break;
    }
    case L_StmKind::T_MOVE:
    {
        // Do nothing
        break;
    }
    case L_StmKind::T_CALL:
    {
        // Do nothing
        break;
    }
    case L_StmKind::T_VOID_CALL:
    {
        // Do nothing
        break;
    }
    case L_StmKind::T_RETURN:
    {
        llvm2asmRet(as_list, &stm);
        alloc_frame = false;
        break;
    }
    case L_StmKind::T_ALLOCA:
    {
        // Do nothing
        break;
    }
    case L_StmKind::T_GEP:
    {
        llvm2asmGep(as_list, &stm);
        break;
    }
    case L_StmKind::T_PHI:
    {
        // Do nothing
        break;
    }
    case L_StmKind::T_NULL:
    {
        // Do nothing
        break;
    }
    }
}

void allocReg(list<AS_stm *> &as_list)
{

    unordered_map<int, int> vregStart;
    unordered_map<int, int> vregEnd;
    auto setDef = [&](AS_reg *reg, int lineNo)
    {
        int regNo = reg->reg;
        if (regNo < 100)
            return;
        if (vregStart.find(regNo) == vregStart.end())
        {
            vregStart.insert({regNo, lineNo});
        }
    };
    auto setUse = [&](AS_reg *reg, int lineNo)
    {
        int regNo = reg->reg;
        if (regNo < 100)
            return;
        vregEnd.insert({regNo, lineNo});
    };
    int lineNo = 0;
    for (const auto &stm : as_list)
    {
        switch (stm->type)
        {
        case AS_stmkind::BINOP:
            setDef(stm->u.BINOP->dst, lineNo);
            setUse(stm->u.BINOP->left, lineNo);
            setUse(stm->u.BINOP->right, lineNo);
            break;
        case AS_stmkind::MOV:
            setDef(stm->u.MOV->dst, lineNo);
            setUse(stm->u.MOV->src, lineNo);
            break;
        case AS_stmkind::LDR:
            setDef(stm->u.LDR->dst, lineNo);
            setUse(stm->u.LDR->ptr, lineNo);
            break;
        case AS_stmkind::STR:
            setUse(stm->u.STR->src, lineNo);
            setUse(stm->u.STR->ptr, lineNo);
            break;
        case AS_stmkind::CMP:
            setUse(stm->u.CMP->left, lineNo);
            setUse(stm->u.CMP->right, lineNo);
            break;
        case AS_stmkind::ADR:
            setDef(stm->u.ADR->reg, lineNo);
            break;
        default:
            break;
        }
        lineNo += 1;
    }

    // workaround for undef vreg
    for (const auto &iter : vregEnd)
    {
        auto pos = vregStart.find(iter.first);
        if (pos == vregStart.end())
        {
            vregStart.insert(iter);
        }
    }

    /* cout<<"Live interval:\n";
    for (auto iter: vregStart){
        cout<<iter.first<<": ["<<iter.second<<", "<<vregEnd[iter.first]<<"]\n";
    } */

    // -1 invalid for allocation, 0 unallocated, >100 registerNo
    // x9-x15 x20-x28 is available
    vector<int> allocateRegs{9, 10, 11, 12, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 28};
    vector<int> allocateTable;
    unordered_map<int, int> v2pMapping;
    allocateTable.resize(32);
    for (int i = 0; i < 32; ++i)
    {
        allocateTable[i] = -1;
    }
    for (auto ind : allocateRegs)
    {
        allocateTable[ind] = 0;
    }

    auto get_mapping = [&](int regNo, int lineNo)
    {
        auto pos = v2pMapping.find(regNo);
        if (pos != v2pMapping.end())
            return pos->second;

        // find available reg
        for (int i = 0; i < 32; ++i)
        {
            int allocNo = allocateTable[i];
            if ((allocNo == 0) || (allocNo > 0 && vregEnd[allocNo] < lineNo))
            {
                v2pMapping[regNo] = i;
                allocateTable[i] = regNo;
                // cout<<regNo<<" -> "<<i<<"\n";
                return i;
            }
        }
        // throw runtime_error("allocate register fail");
        exit(-1);
    };

    auto vreg_map = [&](AS_reg *reg, int lineNo)
    {
        int regNo = reg->reg;
        if (regNo < 100)
            return;
        reg->reg = get_mapping(regNo, lineNo);
    };

    lineNo = 0;
    for (const auto &stm : as_list)
    {
        switch (stm->type)
        {
        case AS_stmkind::BINOP:
            vreg_map(stm->u.BINOP->dst, lineNo);
            vreg_map(stm->u.BINOP->left, lineNo);
            vreg_map(stm->u.BINOP->right, lineNo);
            break;
        case AS_stmkind::MOV:
            vreg_map(stm->u.MOV->dst, lineNo);
            vreg_map(stm->u.MOV->src, lineNo);
            break;
        case AS_stmkind::LDR:
            vreg_map(stm->u.LDR->dst, lineNo);
            vreg_map(stm->u.LDR->ptr, lineNo);
            break;
        case AS_stmkind::STR:
            vreg_map(stm->u.STR->src, lineNo);
            vreg_map(stm->u.STR->ptr, lineNo);
            break;
        case AS_stmkind::CMP:
            vreg_map(stm->u.CMP->left, lineNo);
            vreg_map(stm->u.CMP->right, lineNo);
            break;
        case AS_stmkind::ADR:
            vreg_map(stm->u.ADR->reg, lineNo);
            break;
        default:
            break;
        }
        lineNo += 1;
    }

    /* cout<<"regAlloc:\n";
    for (const auto& iter:v2pMapping){
        cout<<"x"<<iter.first<<" -> x"<<iter.second<<"\n";
    } */
}

AS_func *llvm2asmFunc(L_func &func)
{
    list<AS_stm *> stms;

    auto p = new AS_func(stms);
    auto func_label = new AS_label(func.name);
    p->stms.push_back(AS_Label(func_label));

    for (const auto &block : func.blocks)
    {
        for (const auto &instr : block->instrs)
        {
            llvm2asmStm(p->stms, *instr);
        }
    }

    allocReg(p->stms);

    return p;
}

void llvm2asmDecl(vector<AS_decl *> &decls, L_def &def)
{
    switch (def.kind)
    {
    case L_DefKind::GLOBAL:
    {
        return;
    }
    case L_DefKind::FUNC:
    {
        AS_decl *decl = new AS_decl(def.u.FUNC->name);
        decls.push_back(decl);
        break;
    }
    case L_DefKind::SRT:
    {
        return;
    }
    }
}

void llvm2asmGlobal(vector<AS_global *> &globals, L_def &def)
{
    switch (def.kind)
    {
    case L_DefKind::GLOBAL:
    {
        // Fixme: add here
        /*
        AS_global 是汇编代码内全局变量 DATA 的抽象，在全局有一个 label 作为标签。

        对于 int 类型，init 存储初始化的整型值（缺省为 0 ）。
        对于数组或结构体，全部初始化为 0 , len 记录全局变量需要多少字节，注意不是数组长度。*/
        int _len = 4;
        if (def.u.GLOBAL->def.kind == TempType::INT_PTR)
        {
            _len = max(4 * def.u.GLOBAL->def.len, 4);
        }
        else if (def.u.GLOBAL->def.kind == TempType::STRUCT_PTR || def.u.GLOBAL->def.kind == TempType::STRUCT_TEMP)
        {
            int temp = structLayout[def.u.GLOBAL->def.structname];
            _len = max(temp * def.u.GLOBAL->def.len, temp);
        }
        int _init = 0;
        if (def.u.GLOBAL->init.size() == 0 ||
            def.u.GLOBAL->def.kind == TempType::STRUCT_PTR ||
            def.u.GLOBAL->def.kind == TempType::STRUCT_TEMP)
            ;
        else
        {
            if (def.u.GLOBAL->def.kind == TempType::INT_PTR && def.u.GLOBAL->def.len > 0)
                ;
            _init = def.u.GLOBAL->init[0];
        }

        globals.push_back(new AS_global(new AS_label(def.u.GLOBAL->name), _init, _len));
        break;
    }
    case L_DefKind::FUNC:
    {
        return;
    }
    case L_DefKind::SRT:
    {
        return;
    }
    }
}

AS_prog *llvm2asm(L_prog &prog)
{
    std::vector<AS_global *> globals;
    std::vector<AS_decl *> decls;
    std::vector<AS_func *> func_list;

    auto as_prog = new AS_prog(globals, decls, func_list);

    structLayoutInit(prog.defs);

    // translate function definition
    for (const auto &def : prog.defs)
    {
        llvm2asmDecl(as_prog->decls, *def);
    }

    for (const auto &func : prog.funcs)
    {
        AS_decl *decl = new AS_decl(func->name);
        as_prog->decls.push_back(decl);
    }

    // translate global data
    for (const auto &def : prog.defs)
    {
        llvm2asmGlobal(as_prog->globals, *def);
    }

    // translate each llvm function

    for (const auto &func : prog.funcs)
    {
        alloc_frame = false;

        set_stack(*func);

        as_prog->funcs.push_back(llvm2asmFunc(*func));
    }

    return as_prog;
}
