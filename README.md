# compiler_homework
This is the compiler project release repo for the SOFT130061 @ Fudan University.
## Task1
Primitive Types 和One-level Array 定义
1. 完成 $<type>programElements$生成
2. 完成$<token>programElement$，其中一个类别为$<token> varDeclStmt varDecl$
3. 完成Primitive Types 定义
4. 和One-level Array中,存在初始化列表。比如"let a[5]:int={1,2,3}",需要抽象出$<token>my_rightValList$ 
实现方法：
    a. 使用yacc:$<type>my_rightValListItem$和
    $<token>my_rightVal$
    b. 使用lex:不行
5. 为此需要先实现右值。比如$$let a[5]:int={1+3,2-b,3*c}$$
6. 右值先不考虑函数调用$$ let a[5]:int={1+3,2-b,3*c,f(1)}$$Compiler/Compiler/src/TeaplAst.h:73

7. 测试文件书写
将原先的测试文件更改路径至'./src/testsF',在'./src/tests'下书写自己的测试文件。
'''c
let a;//这种语法先不考虑
'''
## Task 2-3
### 函数定义
### 结构体定义

## Task4-5

### 右值

注意布尔运算和算术运算的区别。可能有问题，需要参考Compiler/Compiler/Spec/teapl.md修改

### 位置信息重构

# Homework4
``` c++
struct Func_local
{
    std::string name;
    LLVMIR::FuncType ret;
    std::vector<Temp_temp*> args;
    std::list<LLVMIR::L_stm*> irs;
    Func_local(const std::string _name,LLVMIR::FuncType _ret,const std::vector<Temp_temp*> &_args,const std::list<LLVMIR::L_stm*> &_irs)
        : name(_name), ret(_ret), args(_args), irs(_irs) {}
};
struct L_func
{
    std::string name;
    FuncType ret;
    std::vector<Temp_temp*> args;
    std::list<L_block*> blocks;
    L_func(const std::string &_name,FuncType _ret,const std::vector<Temp_temp*> _args,const std::list<L_block*> &_blocks);
};
struct L_block
{
    Temp_label *label;
    std::unordered_set<Temp_label*> succs;
    std::list<L_stm*> instrs;
    L_block(Temp_label *_label,const std::unordered_set<Temp_label*> &_succs,const std::list<L_stm*> &_instrs);
};
```

## Task5
teat_add_or: 

exit
llvm-link-14: /root/usr/Compiler/Compiler/src/tests/teat_add_or.ll:112:22: error: use of undefined value '%r137'
  call void @re(i32* %r137, i32* %r138)

test08: 

exit
llvm-link-14: /root/usr/Compiler/Compiler/src/tests/test08.ll:80:22: error: use of undefined value '%r124'
  call void @re(i32* %r124, i32* %r125)
                     ^
llvm-link-14: error:  loading file '/root/usr/Compiler/Compiler/src/tests/test08.ll'