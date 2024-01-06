#include "ssa.h"
#include <cassert>
#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "bg_llvm.h"
#include "graph.hpp"
#include "liveness.h"
#include "printLLVM.h"
#define INSERT() printf("%s:%d\n", __FILE__, __LINE__)
using namespace std;
using namespace LLVMIR;
using namespace GRAPH;

template <typename T>
std::unordered_set<T> make_diff(std::unordered_set<T> &tl1, std::unordered_set<T> &tl2)
{
    unordered_set<T> diffSet = unordered_set<T>();
    for (auto &it : tl1)
    {
        diffSet.emplace(it);
    }
    for (auto &it : tl2)
    {
        diffSet.erase(it);
    }
    return diffSet;
}
template <typename T>
std::unordered_set<T> make_union(std::unordered_set<T> &tl1, std::unordered_set<T> &tl2)
{

    unordered_set<T> diffSet = unordered_set<T>();
    for (auto &it : tl1)
    {
        diffSet.emplace(it);
    }
    for (auto &it : tl2)
    {
        diffSet.emplace(it);
    }
    return diffSet;
}
template <typename T>
std::unordered_set<T> make_intersection(std::unordered_set<T> &tl1, std::unordered_set<T> &tl2)
{
    unordered_set<T> intersection = unordered_set<T>();
    for (auto &it : tl1)
    {
        if (tl2.find(it) != tl2.end())
        {
            intersection.emplace(it);
        }
    }
    return intersection;
}
struct imm_Dominator
{
    LLVMIR::L_block *pred;
    unordered_set<LLVMIR::L_block *> succs;
};

unordered_map<L_block *, unordered_set<L_block *>> dominators;
unordered_map<L_block *, imm_Dominator> tree_dominators;
unordered_map<L_block *, unordered_set<L_block *>> DF_array;
unordered_map<L_block *, Node<LLVMIR::L_block *> *> revers_graph;
unordered_map<Temp_temp *, AS_operand *> temp2ASoper;

static void init_table()
{
    dominators.clear();
    tree_dominators.clear();
    DF_array.clear();
    revers_graph.clear();
    temp2ASoper.clear();
}

LLVMIR::L_prog *SSA(LLVMIR::L_prog *prog)
{
    for (auto &fun : prog->funcs)
    {
        init_table();

        combine_addr(fun);
        mem2reg(fun);

        auto RA_bg = Create_bg(fun->blocks);

        SingleSourceGraph(RA_bg.mynodes[0], RA_bg, fun);

        // Show_graph(stdout, RA_bg);
        Liveness(RA_bg.mynodes[0], RA_bg, fun->args);

        Dominators(RA_bg);
        // printf_domi();
        tree_Dominators(RA_bg);
        // printf_D_tree();
        // 默认0是入口block
        computeDF(RA_bg, RA_bg.mynodes[0]);
        // printf_DF();
        Place_phi_fu(RA_bg, fun);
        Rename(RA_bg);
        combine_addr(fun);
    }
    return prog;
}
bool is_scalar_ptr(AS_operand *as)
{
    return as->kind == OperandKind::TEMP && as->u.TEMP->type == TempType::INT_PTR && as->u.TEMP->len == 0;
}
bool is_scalar_temp(AS_operand *as)
{
    return as->kind == OperandKind::TEMP && as->u.TEMP->type == TempType::INT_TEMP;
}
static bool is_mem_variable(L_stm *stm)
{
    return stm->type == L_StmKind::T_ALLOCA && stm->u.ALLOCA->dst->kind == OperandKind::TEMP && stm->u.ALLOCA->dst->u.TEMP->type == TempType::INT_PTR && stm->u.ALLOCA->dst->u.TEMP->len == 0;
}

// 保证相同的AS_operand,地址一样 。常量除外
void combine_addr(LLVMIR::L_func *fun)
{
    unordered_map<Temp_temp *, unordered_set<AS_operand **>> temp_set;
    unordered_map<Name_name *, unordered_set<AS_operand **>> name_set;
    for (auto &block : fun->blocks)
    {
        for (auto &stm : block->instrs)
        {
            auto AS_operand_list = get_all_AS_operand(stm);
            for (auto AS_op : AS_operand_list)
            {
                if ((*AS_op)->kind == OperandKind::TEMP)
                {
                    temp_set[(*AS_op)->u.TEMP].insert(AS_op);
                }
                else if ((*AS_op)->kind == OperandKind::NAME)
                {
                    name_set[(*AS_op)->u.NAME].insert(AS_op);
                }
            }
        }
    }
    for (auto temp : temp_set)
    {
        AS_operand *fi_AS_op = **temp.second.begin();
        for (auto AS_op : temp.second)
        {
            *AS_op = fi_AS_op;
        }
    }
    for (auto name : name_set)
    {
        AS_operand *fi_AS_op = **name.second.begin();
        for (auto AS_op : name.second)
        {
            *AS_op = fi_AS_op;
        }
    }
}

void mem2reg(LLVMIR::L_func *fun)
{
    // 一步一步处理，不会讲alloca去除
    //    Todo 只对标量处理
    // 一、优化Load
    // 二、优化store
    //
    for (auto x : fun->blocks)
    {
        list<L_stm *>::iterator it = x->instrs.begin();
        while (it != x->instrs.end())
        {
            if (is_mem_variable(*it))
            {
                auto xx = AS_Operand_Temp(Temp_newtemp_int());
                temp2ASoper[(*it)->u.ALLOCA->dst->u.TEMP] = xx;
                *it = L_Move(AS_Operand_Const(0), xx);
                // it = x->instrs.erase(it);
                it++;
            }
            else
                it++;
        }
    }
    // 如果load
    // getptr
    unordered_map<Temp_temp *, AS_operand *> mem_scalar_load;

    for (auto x : fun->blocks)
    {
        list<L_stm *>::iterator it = x->instrs.begin();
        while (it != x->instrs.end())
        {
            // 如果load，首先，如果是标量，不会有getptr。
            switch ((*it)->type)
            {
            case L_StmKind::T_STORE:
            {
                AS_operand *res = nullptr;
                AS_operand *src = nullptr;
                auto store = (*it)->u.STORE;
                if (is_scalar_ptr(store->ptr))
                // 将一个mem_variable存进数组
                // 将一个mem_variable给另一个mem_variable
                // 将一个东西给mem_variable
                {
                    if (temp2ASoper.find(store->ptr->u.TEMP) != temp2ASoper.end())
                        res = temp2ASoper[store->ptr->u.TEMP];
                    // 没有就是nullptr
                }
                if (store->src->kind == OperandKind::TEMP && store->src->u.TEMP->type == TempType::INT_TEMP)
                    src = mem_scalar_load[store->src->u.TEMP];
                if (res == nullptr)
                {
                    if (src != nullptr)
                    {
                        store->src = src;
                    }
                }
                else
                {
                    (*it) = L_Move(src == nullptr ? store->src : src, res);
                }
                break;
            }
            case L_StmKind::T_LOAD:
            {
                /*
                %r103 = load i32, ptr %r100, align 4
                */
                AS_operand *res = nullptr;
                AS_operand *src = nullptr;
                auto load = (*it)->u.LOAD;
                if (is_scalar_ptr(load->ptr))
                {
                    if (temp2ASoper.find(load->ptr->u.TEMP) != temp2ASoper.end())
                    {
                        mem_scalar_load[load->dst->u.TEMP] = temp2ASoper[load->ptr->u.TEMP];
                        it = x->instrs.erase(it);
                        continue;
                    }
                }
                break;
            }
            case L_StmKind::T_BINOP:
            case L_StmKind::T_CMP:
            case L_StmKind::T_MOVE:
            case L_StmKind::T_GEP:
            case L_StmKind::T_CALL:
            case L_StmKind::T_VOID_CALL:
            case L_StmKind::T_RETURN:
            default:
            {
                auto ass = get_all_AS_operand(*it);
                for (auto &as : ass)
                {
                    if ((*as)->kind == OperandKind::TEMP && (*as)->u.TEMP->type == TempType::INT_TEMP)
                    {
                        if (mem_scalar_load.find((*as)->u.TEMP) != mem_scalar_load.end())
                        {
                            *as = mem_scalar_load[(*as)->u.TEMP];
                        }
                    }
                }
                break;
            }
            break;
            }
            it++;
        }
    }
}

void Dominators(GRAPH::Graph<LLVMIR::L_block *> &bg)
{
    //   Todo
    auto sum = unordered_set<L_block *>();
    for (auto x : bg.mynodes)
    {
        if (x.second == nullptr)
        {
            continue;
        }
        sum.emplace(x.second->info);
    }
    for (auto x : bg.mynodes)
    {
        if (x.second == nullptr)
        {
            continue;
        }
        auto temp = unordered_set<L_block *>(sum.begin(), sum.end());
        dominators.emplace(x.second->info, temp);
    }

    sum.clear();
    auto aaa = unordered_set<L_block *>();
    aaa.emplace(bg.mynodes[0]->info);
    dominators.erase(bg.mynodes[0]->info);
    dominators.emplace(bg.mynodes[0]->info, aaa);
    for (auto xx : bg.mynodes)

        for (auto &x : bg.mynodes)
        {
            for (auto &y : bg.mynodes)
            {
                if (y.second != nullptr)
                {
                    std::unordered_set<L_block *> intersection;
                    for (auto key : y.second->preds)
                    {
                        auto &preds = dominators[bg.mynodes[key]->info];
                        if (intersection.size() > 0)
                            intersection = make_intersection(preds, intersection);
                        else
                        {
                            intersection = preds;
                        }
                    }
                    intersection.emplace(y.second->info);

                    dominators[y.second->info] = intersection;
                }
            }
        }
}

void printf_domi()
{
    printf("Dominator:\n");
    for (auto x : dominators)
    {
        printf("%s :\n", x.first->label->name.c_str());
        for (auto t : x.second)
        {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}

void printf_D_tree()
{
    printf("dominator tree:\n");
    for (auto x : tree_dominators)
    {

        printf("%s :\n", x.first->label->name.c_str());

        for (auto t : x.second.succs)
        {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}
void printf_DF()
{
    printf("DF:\n");
    for (auto x : DF_array)
    {
        printf("%s :\n", x.first->label->name.c_str());
        for (auto t : x.second)
        {
            printf("%s ", t->label->name.c_str());
        }
        printf("\n\n");
    }
}

void tree_Dominators(GRAPH::Graph<LLVMIR::L_block *> &bg)
{
    //   Todo
    for (auto &x : dominators)
    {
        // idom(n)是n的必经结点
        for (auto &y : x.second)
        {
            if (y != x.first)
            { // idom(n)和n不是同一个结点
                bool imm = true;
                for (auto &z : x.second)
                {
                    // idom(n)不是 n的其他必经结点的必经结点。
                    if (z == y || z == x.first)
                    {
                        continue;
                    }
                    if (dominators[z].find(y) != dominators[z].end())
                    {
                        imm = false;
                        break;
                    }
                }
                if (imm)
                {
                    imm_Dominator imm_Dominator_temp;
                    // printf("%s:%d,%s\n", __FILE__, __LINE__, y->label->name.c_str());

                    imm_Dominator_temp.pred = y;
                    imm_Dominator_temp.succs = unordered_set<LLVMIR::L_block *>();
                    // printf("%s:%d,%s\n", __FILE__, __LINE__, x.first->label->name.c_str());
                    tree_dominators[x.first] = imm_Dominator_temp;
                }
            }
        }
    }
    for (auto &xx : dominators)
    {
        auto x = tree_dominators[xx.first];
        if (x.pred == nullptr)
            continue;
        // printf("%s:%d,%s\n", __FILE__, __LINE__, x.pred->label->name.c_str());
        if (x.pred != nullptr)
            tree_dominators[x.pred].succs.emplace(xx.first);
    }
}

void computeDF(GRAPH::Graph<LLVMIR::L_block *> &bg, GRAPH::Node<LLVMIR::L_block *> *r)
{
    //   Todo
    for (auto x : r->succs)
    {
        if (tree_dominators[bg.mynodes[x]->info].pred != r->info)
        {
            DF_array[r->info].emplace(bg.mynodes[x]->info);
        }
    }
    for (auto &x : tree_dominators[r->info].succs)
    {
        for (auto &node : bg.mynodes)
        {
            if (node.second->info == x)
            {
                computeDF(bg, node.second);
            }
        }
        for (auto &w : DF_array[x])
        {
            if (dominators[w].find(r->info) == dominators[w].end() || r->info == w)
            {
                DF_array[r->info].emplace(w);
            }
        }
    }
}

// 只对标量做
void Place_phi_fu(GRAPH::Graph<LLVMIR::L_block *> &bg, L_func *fun)
{
    //   Todo
    std::unordered_map<L_block *, std::unordered_set<AS_operand *>> A_orig;
    std::unordered_map<AS_operand *, std::unordered_set<L_block *>> defsites;
    std::unordered_map<L_block *, std::unordered_set<AS_operand *>> A_phi;

    // 计算A_orig
    for (auto &node : bg.mynodes)
    {
        auto x = node.second->info;
        for (auto &y : x->instrs)
        {
            auto ass = get_def_operand(y);
            for (auto as : ass)
            {
                auto it = std::find_if(temp2ASoper.begin(), temp2ASoper.end(), [&](const auto &pair)
                                       { return pair.second == *as; });

                if (it != temp2ASoper.end())
                {
                    A_orig[x].emplace(*as);
                }
            }
        }
    }
    for (auto &x : A_orig)
    {
        for (auto &y : x.second)
        {
            defsites[y].emplace(x.first);
        }
    }
    for (auto &x : defsites)
    {
        auto W = x.second;
        while (W.size())
        {

            L_block *n = W.extract(W.begin()).value();
            W.erase(n);

            for (auto &y : DF_array[n])
            {

                if (A_phi[y].find(x.first) == A_phi[y].end())
                {
                    A_phi[y].emplace(x.first);
                    auto node = std::find_if(bg.mynodes.begin(), bg.mynodes.end(), [&](const auto &pair)
                                             { return pair.second->info == y; });
                    std::vector<std::pair<AS_operand *, Temp_label *>> phis;
                    for (auto pred : node->second->preds)
                    {
                        phis.push_back({x.first, bg.mynodes[pred]->info->label});
                    }
                    auto it = node->second->info->instrs.front();
                    if (it->type == L_StmKind::T_LABEL)
                        node->second->info->instrs.pop_front();
                    node->second->info->instrs.emplace_front(L_Phi(x.first, phis));
                    // printf("bb:%s,chaPi:\%r%d\n", node->second->info->label->name.c_str(), x.first->u.TEMP->num);
                    if (it->type == L_StmKind::T_LABEL)

                        node->second->info->instrs.emplace_front(it);
                    if (A_orig[y].find(x.first) == A_orig[y].end())
                    {
                        W.emplace(y);
                    }
                }
            }
        }
    }
}

static list<AS_operand **> get_def_int_operand(LLVMIR::L_stm *stm)
{
    list<AS_operand **> ret1 = get_def_operand(stm), ret2;
    for (auto AS_op : ret1)
    {
        if ((**AS_op).kind == OperandKind::TEMP && (**AS_op).u.TEMP->type == TempType::INT_TEMP)
        {
            ret2.push_back(AS_op);
        }
    }
    return ret2;
}

static list<AS_operand **> get_use_int_operand(LLVMIR::L_stm *stm)
{
    list<AS_operand **> ret1 = get_use_operand(stm), ret2;
    for (auto AS_op : ret1)
    {
        if ((**AS_op).kind == OperandKind::TEMP && (**AS_op).u.TEMP->type == TempType::INT_TEMP)
        {
            ret2.push_back(AS_op);
        }
    }
    return ret2;
}

static void Rename_temp(GRAPH::Graph<LLVMIR::L_block *> &bg, GRAPH::Node<LLVMIR::L_block *> *n, unordered_map<Temp_temp *, stack<Temp_temp *>> Stack)
{
    //   Todo
    for (auto S : n->info->instrs)
    {
        if (S->type != L_StmKind::T_PHI)
        {
            auto ass = get_use_int_operand(S);
            for (auto as : ass)
            {
                if (Stack.find((*as)->u.TEMP) != Stack.end())
                {
                    *as = AS_Operand_Temp(Stack[(*as)->u.TEMP].top());
                }
            }
        }
        auto ass = get_def_int_operand(S);
        for (auto as : ass)
        {
            if (Stack.find((*as)->u.TEMP) != Stack.end())
            {
                auto xx = Temp_newtemp_int();
                Stack[(*as)->u.TEMP].push(xx);
                *as = AS_Operand_Temp(xx);
            }
        }
    }
    for (auto yy : n->succs)
    {
        auto y = bg.mynodes[yy];
        int num = 0;

        auto iter = y->preds.begin();
        while (iter != y->preds.end())
        {
            if (bg.mynodes[*iter]->info->label->name == n->info->label->name)
            {
                break;
            }
            iter++;
            num++;
        }
        for (auto S : y->info->instrs)
        {
            if (S->type == L_StmKind::T_PHI)
            {
                auto xx = S->u.PHI->phis[num].first->u.TEMP;
                S->u.PHI->phis[num].first = AS_Operand_Temp(Stack[xx].top());
            }
        }
    }

    for (auto XX : tree_dominators[n->info].succs)
    {

        auto node = std::find_if(bg.mynodes.begin(), bg.mynodes.end(), [&](const auto &pair)
                                 { return pair.second->info == XX; });

        Rename_temp(bg, bg.mynodes[node->first], Stack);
    }
}

void Rename(GRAPH::Graph<LLVMIR::L_block *> &bg)
{
    //   Todo
    unordered_map<Temp_temp *, stack<Temp_temp *>> Stack;
    for (auto &iter : temp2ASoper)
    {
        // printf("\%r%d:", iter.second->u.TEMP->num);
        // Stack[iter.second->u.TEMP].push(Temp_newtemp_int());
        Stack[iter.second->u.TEMP].push(iter.second->u.TEMP);
        // printf("\%r%d\n", Stack[iter.second->u.TEMP].top()->num);
    }
    Rename_temp(bg, bg.mynodes[0], Stack);
}
