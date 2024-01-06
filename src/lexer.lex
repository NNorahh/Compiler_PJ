%{
#include <stdio.h>
#include <string.h>
#include "TeaplAst.h"
#include "y.tab.hpp"
extern int line, col ;
A_pos my_pos();
%}
%s COMMENT1
%s COMMENT2

%%
<COMMENT1>"\n" { line+=1;col = 1;BEGIN INITIAL; }
<COMMENT2>"\n" { line+=1; col = 1;}
<COMMENT2>"*/" {col += yyleng; BEGIN INITIAL ; }
<INITIAL>"//" { col += yyleng;BEGIN COMMENT1;}
<INITIAL>"/*" { col += yyleng;BEGIN COMMENT2;}



<INITIAL>"\n" {line+=yyleng; col=1;}
<INITIAL>"let " {yylval.pos = my_pos();col+=yyleng; return LET;}
<INITIAL>"if" {yylval.pos = my_pos();col+=yyleng;return IF;}
<INITIAL>"else" {yylval.pos = my_pos();col+=yyleng;return ELSE;}
<INITIAL>"while" {yylval.pos = my_pos();col+=yyleng;return WHILE;}
<INITIAL>"fn" {yylval.pos = my_pos();col+=yyleng;return FN;}
<INITIAL>"ret" {yylval.pos = my_pos();col+=yyleng;return RET_;}
<INITIAL>"struct" {yylval.pos = my_pos();col+=yyleng;return STRUCT;}
<INITIAL>"break" {yylval.pos = my_pos();col+=yyleng;return BREAK;}
<INITIAL>"continue" {yylval.pos = my_pos();col+=yyleng;return CONTINUE;}
<INITIAL>"->" {yylval.pos = my_pos();col +=yyleng;return FnRetTypeDecl;}
<INITIAL>"int" {yylval.pos = my_pos();col+=yyleng;yylval.type= A_NativeType(A_Pos(line,col),A_intTypeKind) ;return NativeType;}
<INITIAL>"<=" {col+=yyleng;return LE;}
<INITIAL>"<" {col+=yyleng;return LT;}
<INITIAL>">=" {col+=yyleng;return GE;}
<INITIAL>">" {col+=yyleng;return GT;}
<INITIAL>"==" {col+=yyleng;return EEQ;}
<INITIAL>"!=" {col+=yyleng; return NE;}
<INITIAL>"=" { col+=yyleng;return EQ;}
<INITIAL>"!"	{yylval.pos = my_pos(); col+=yyleng; return NOT ;}
<INITIAL>"\t" {col +=4;}
<INITIAL>" " {col +=1;}
<INITIAL>"*" { col+=yyleng; yylval.op = A_mul; return MUL ;}
<INITIAL>"/" { col+=yyleng; yylval.op = A_div; return DIV  ;}
<INITIAL>"+"	{ col+=yyleng; yylval.op = A_add; return ADD ;}
<INITIAL>"-"	{ col+=yyleng; yylval.op = A_sub; return SUB ;}
<INITIAL>"&&" {col+=yyleng;return AND;}
<INITIAL>"||" {col+=yyleng;return OR;}
<INITIAL>"\(" {yylval.pos = my_pos();col+=yyleng; return LP ;}
<INITIAL>")" {yylval.pos = my_pos();yylval.pos = my_pos(); col+=yyleng; return RP ;}
<INITIAL>"[" { yylval.pos = my_pos();col+=yyleng; return MLP ;}
<INITIAL>"]" { yylval.pos = my_pos();col+=yyleng; return MRP ;}
<INITIAL>"{" { yylval.pos = my_pos();col+=yyleng; return BLP ;}
<INITIAL>"}" { yylval.pos = my_pos();col+=yyleng; return BRP ;}
<INITIAL>":" { yylval.pos = my_pos();col +=yyleng;return TypeAssign;}
<INITIAL>";" {yylval.pos = my_pos();col +=yyleng;return StmtEnd;}
<INITIAL>"," {yylval.pos = my_pos();col +=yyleng;return Comma;}
<INITIAL>(0|([1-9][0-9]*)) { 
	yylval.tokenNum= A_TokenNum(my_pos(),atoi(yytext));
	col +=yyleng;
	return NUMBER; 
}
<INITIAL>"." {yylval.pos = my_pos();col +=yyleng;return POINT;}

<INITIAL>[a-zA-Z_][a-zA-Z0-9_]* {
    //it is important
    char *res = (char *)calloc(sizeof(char),yyleng+1); 
    strncpy(res,yytext,yyleng);
    yylval.tokenId=A_TokenId(my_pos(),res);
    col +=yyleng;
	return term;
}

%%