#include "bg_llvm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "graph.hpp"
#include "printLLVM.h"
#include "temp.h"
/* graph on AS_ basic blocks. This is useful to find dominance
   relations, etc. */
using namespace std;
using namespace LLVMIR;
using namespace GRAPH;
#define aaa() printf("%s:%d\n", __FILE__, __LINE__);
static Graph<L_block *> RA_bg;
static unordered_map<Temp_label *, L_block *> block_env;

Graph<L_block *> &Bg_graph()
{
    return RA_bg;
}
unordered_map<Temp_label *, L_block *> &Bg_block_env()
{
    return block_env;
}

Node<L_block *> *Look_bg(L_block *b)
{
    Node<L_block *> *n1 = nullptr;
    for (auto n : *RA_bg.nodes())
    {
        if (n.second->nodeInfo() == b)
        {
            n1 = n.second;
            break;
        }
    }
    if (n1 == nullptr)
        return RA_bg.addNode(b);
    else
        return n1;
}

static void Enter_bg(L_block *b1, L_block *b2)
{
    Node<L_block *> *n1 = Look_bg(b1);
    Node<L_block *> *n2 = Look_bg(b2);
    RA_bg.addEdge(n1, n2);
    return;
}

/* input LLVMIR::L_block* *List after instruction selection for each block,
    generate a graph on the basic blocks */

Graph<L_block *> &Create_bg(list<L_block *> &bl)
{
    RA_bg = Graph<L_block *>();
    block_env = unordered_map<Temp_label *, L_block *>();

    for (auto block : bl)
    {
        block_env.insert({block->label, block});
        RA_bg.addNode(block);
    }

    for (auto block : bl)
    {
        unordered_set<Temp_label *> succs = block->succs;
        for (auto label : succs)
        {
            Enter_bg(block, block_env[label]);
        }
    }
    return RA_bg;
}

// maybe useful
static void DFS(Node<L_block *> *r, Graph<L_block *> &bg)
{
}

void SingleSourceGraph(Node<L_block *> *r, Graph<L_block *> &bg, L_func *fun)
{
    //   Todo
    // 如何合并a和b。
    /*如果a的后继只有一个c，而且c只有a一个前驱，那么需要合并a和c。并将c的后继的前驱改成a
        a
        |
        b
        |
      - c
     |__|
    */
    bool stop = false;

    for (auto &block_node : bg.mynodes)
    {
        if (block_node.second && block_node.second->info)
            if (block_node.second->info != r->info)
            {
                if (block_node.second->inDegree() == 0)
                {

                    auto succs = block_node.second->succs;
                    for (auto succ : succs)
                    {

                        if (bg.mynodes[succ] && bg.mynodes[succ]->info)
                        {

                            bg.rmEdge(block_node.second, bg.mynodes[succ]);
                        }
                    }
                    block_node.second->info->instrs.clear();
                    block_node.second->info->succs.clear();
                    bg.rmNode(block_node.second);
                }
            }
    }

    while (!stop)
    {
        stop = true;
        for (auto block_node : bg.mynodes)
        {

            if (block_node.second && block_node.second->info)

                if (block_node.second->inDegree() == 1)
                {
                    auto iter = block_node.second->preds.begin();
                    auto merge = bg.mynodes[*iter];
                    if (merge->outDegree() != 1)
                    {
                        continue;
                    }

                    stop = false;
                    NodeSet temp = block_node.second->succs;
                    for (auto x : temp)
                    {
                        bg.rmEdge(block_node.second, bg.mynodes[x]);
                        bg.addEdge(merge, bg.mynodes[x]);
                    }
                    for (auto llstm : block_node.second->info->instrs)
                    {
                        merge->info->instrs.push_back(llstm);
                    }
                    for (auto x : block_node.second->info->succs)
                    {
                        merge->info->succs.emplace(x);
                    }
                    auto it = merge->info->instrs.begin();
                    while (it != merge->info->instrs.end())
                    {
                        if ((*it)->type == L_StmKind::T_JUMP)
                        {
                            if ((*it)->u.JUMP->jump->name.c_str() == block_node.second->info->label->name.c_str())
                            {
                                it = merge->info->instrs.erase(it);
                                continue;
                            }
                        }
                        else if ((*it)->type == L_StmKind::T_LABEL)
                        {
                            if ((*it)->u.LABEL->label->name.c_str() == block_node.second->info->label->name.c_str())
                            {
                                it = merge->info->instrs.erase(it);
                                continue;
                            }
                        }
                        it++;
                    }

                    block_node.second->info->instrs.clear();
                    block_node.second->info->succs.clear();
                    bg.rmEdge(merge, block_node.second);
                    bg.rmNode(block_node.second);
                    break;
                }
        }
    }
}

void Show_graph(FILE *out, GRAPH::Graph<LLVMIR::L_block *> &bg)
{
    for (auto block_node : bg.mynodes)
    {
        auto block = block_node.second->info;
        fprintf(out, "%s \n", block->label->name.c_str());
        fprintf(out, "pred  %zu  ", block_node.second->preds.size());
        for (auto pred : block_node.second->preds)
        {
            fprintf(out, "%s  ", bg.mynodes[pred]->info->label->name.c_str());
        }
        fprintf(out, "\n");
        fprintf(out, "succ  %zu  ", block_node.second->succs.size());
        for (auto succ : block_node.second->succs)
        {
            fprintf(out, "%s  ", bg.mynodes[succ]->info->label->name.c_str());
        }
        fprintf(out, "\n");
    }
}