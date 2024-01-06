%{
#include <stdio.h>
#include "TeaplAst.h"

//extern A_pos pos;
extern int line, col;
extern A_program root;
A_pos my_pos();
extern int yylex(void);
extern "C"{
extern void yyerror(char *s); 
extern int  yywrap();
}

%}

// TODO:
// your parser
// you should define the return value in lexer here
%union {
	//int val;
	//char *id;
	A_pos pos;
	A_tokenId tokenId;
	A_tokenNum tokenNum;

	A_type type;
	A_programElementList programElements;
	A_programElement programElement;

	A_varDeclStmt varDeclStmt;
	A_varDecl varDecl;
	A_varDef varDef;

	A_rightValList rightValList;

	A_varDeclList paramDecl;
	A_codeBlockStmtList stmts;

	A_structDef structDef;


	A_fnDeclStmt fnDeclStmt;
	A_fnDecl fnDecl;

	A_fnDef fnDef;

	A_codeBlockStmt codeBlockStmt;

	A_assignStmt assignStmt;
	A_callStmt callStmt;
	A_ifStmt ifStmt;
	A_whileStmt whileStmt;
	A_returnStmt returnStmt;

	//rightVal
	A_rightVal rightVal;
	//leftVal
	A_leftVal leftVal;
	A_arithExpr arithExpr;
	A_boolExpr boolExpr;
	A_arithBiOpExpr arithBiOpExpr;
	A_exprUnit exprUnit;
	A_arithUExpr arithUExpr;
	A_arithBiOp op;
	A_fnCall fnCall;
	A_indexExpr indexExpr;
	A_memberExpr memberExpr;
	A_boolBiOpExpr boolBiOpExpr;
	A_boolUnit boolUnit;
	A_comExpr comExpr;
	A_boolUOpExpr boolUOpExpr;


}

// you should declare tokens and their value type here

%token <pos> NOT LP RP FnRetTypeDecl LET FN IF ELSE WHILE RET_ STRUCT BREAK CONTINUE TypeAssign StmtEnd EQ MLP MRP BLP BRP Comma POINT
%token <tokenId> term

%token LT LE GT GE EEQ NE

// + - * /
%left OR 
%left AND
%left <op> SUB ADD
%left <op> MUL DIV
%token <tokenNum> NUMBER 

%type <programElements> my_program
%type <programElement> my_programElement
%type <rightValList> my_rightValList
%type <rightValList> my_NotNullRightValList
%type <codeBlockStmt> my_codeBlockStmt
%type <stmts> my_codeBlockStmtList


%type <varDeclStmt> my_varDeclStmt
%type <assignStmt> my_assignStmt
%type <whileStmt> my_whileStmt
%type <ifStmt>my_ifStmt

%type <varDecl> my_varDecl
%type <varDef> my_varDef
%type <fnDeclStmt> my_fnDeclStmt
%type <structDef> my_structDef
%type <fnDecl> my_fnDecl
%type <fnDef>my_fnDef
%type <paramDecl>my_ParamDecl
%type <paramDecl>my_NotNullParaDecl
%type <type> my_fnRetType
%type <type> Type
%token <type> NativeType
//rightVal'
%type <rightVal> my_rightVal
%type <leftVal> my_leftVal
%type <arithExpr> my_arithExpr
%type <boolExpr> my_boolExpr
%type <boolExpr> my_ifWhileBoolExpr
%type <arithBiOpExpr> my_arithBiOpExpr
%type <exprUnit> my_exprUnit 
%type <arithUExpr> my_arithUExpr
%type <fnCall> my_fnCall
%type <indexExpr> my_indexExpr
%type <memberExpr> my_memberExpr
%type <boolBiOpExpr> my_boolBiOpExpr
%type <boolUnit> my_boolUnit
%type <comExpr> my_comExpr
%type <boolUOpExpr>my_boolUOpExpr
// you should declare rules for parser between the first and the second %%

%%
my_program: {$$=0; }
	|my_programElement my_program {
		root = A_Program(A_ProgramElementList($1,$2));
		$$=root->programElements; 
	}
	;
//顺序 86 87 87
my_programElement : my_varDeclStmt { 
		$$= A_ProgramVarDeclStmt($1->pos,$1);
	}
	|  my_structDef {
		$$= A_ProgramStructDef($1->pos,$1);
	}
	|  my_fnDeclStmt {
		$$= A_ProgramFnDeclStmt($1->pos,$1);
	}
	|  my_fnDef {
		$$= A_ProgramFnDef($1->pos,$1);
	}
	|  StmtEnd {
		$$= A_ProgramNullStmt($1);
	}
	;
my_varDeclStmt:LET my_varDecl StmtEnd{$$ =A_VarDeclStmt($1,$2); }
	|LET my_varDef StmtEnd{$$ =A_VarDefStmt($1,$2); }
	;
my_varDecl: term TypeAssign Type {
	// let a:int;
		$$ = A_VarDecl_Scalar($1->pos,A_VarDeclScalar($1->pos,$1->id,$3));
	}
	|  term {
		// let a;
		$$ = A_VarDecl_Scalar($1->pos,A_VarDeclScalar($1->pos,$1->id,0));
	}
	| term MLP NUMBER MRP TypeAssign Type  {
		//let a[4]:int;
		$$ = A_VarDecl_Array($1->pos,A_VarDeclArray($1->pos,$1->id,$3->num,$6));
	}
	| term MLP NUMBER MRP {
		//let a[4];
		$$ = A_VarDecl_Array($1->pos,A_VarDeclArray($1->pos,$1->id,$3->num,0));
	}
	;
my_varDef: term TypeAssign Type EQ my_rightVal {
	// let a:int=1;
		$$ = A_VarDef_Scalar($1->pos,A_VarDefScalar($1->pos,$1->id,$3,$5));
	}
	|  term EQ my_rightVal {
		// let a=1;
		$$ = A_VarDef_Scalar($1->pos,A_VarDefScalar($1->pos,$1->id,0,$3));
	}
	| term MLP NUMBER MRP TypeAssign Type EQ BLP my_rightValList BRP {
		$$ = A_VarDef_Array($1->pos,A_VarDefArray($1->pos,$1->id,$3->num,$6,$9));
	}
	| term MLP NUMBER MRP EQ BLP my_rightValList BRP {
		$$ = A_VarDef_Array($1->pos,A_VarDefArray($1->pos,$1->id,$3->num,0,$7));
	}
	;
my_rightValList: { $$ = 0;}
	|my_NotNullRightValList { 
		$$ = $1;
	}
	;
my_NotNullRightValList:
	my_rightVal {
		$$ = A_RightValList($1,0);
	}
	| my_rightVal Comma my_NotNullRightValList {
		$$ = A_RightValList($1,$3);
	}
	;
my_fnDeclStmt:FN my_fnDecl StmtEnd{
		$$ = A_FnDeclStmt($1,$2);
	}
	;
my_structDef: 
	STRUCT term BLP my_NotNullParaDecl BRP {
		$$ = A_StructDef($1, $2->id, $4);
	}
	;
my_fnDef: FN my_fnDecl BLP my_codeBlockStmtList BRP {
		$$ = A_FnDef($1,$2,$4);
	}

	;
my_codeBlockStmtList:
	{ $$ = 0 ;}
	| my_codeBlockStmt my_codeBlockStmtList{
		$$ = A_CodeBlockStmtList($1, $2);
	}
	;
my_leftVal:
	term {$$= A_IdExprLVal($1->pos, $1->id);}
	|my_leftVal MLP my_indexExpr MRP {$$ = A_ArrExprLVal($1->pos,A_ArrayExpr($1->pos,$1,$3));}
	|my_memberExpr {$$ =A_MemberExprLVal($1->pos, $1);}
	;
my_assignStmt:
	my_leftVal EQ my_rightVal StmtEnd {
		$$ = A_AssignStmt($1->pos, $1, $3);
	}
	;
my_whileStmt:
	WHILE LP my_ifWhileBoolExpr RP BLP my_codeBlockStmtList BRP
	{
		$$ = A_WhileStmt($1,$3,$6);
	}
	;
my_ifStmt:
	IF LP my_ifWhileBoolExpr RP BLP my_codeBlockStmtList BRP
	{
		$$ = A_IfStmt($1,$3,$6,0);
	}
	|IF LP my_ifWhileBoolExpr RP BLP my_codeBlockStmtList BRP 
	ELSE BLP my_codeBlockStmtList BRP 
	{
		$$ = A_IfStmt($1,$3,$6,$10);
	}
	;
my_codeBlockStmt:
	StmtEnd { $$ = A_BlockNullStmt($1);}
	| my_varDeclStmt { $$ = A_BlockVarDeclStmt($1->pos, $1);}
	| my_assignStmt { $$ = A_BlockAssignStmt($1->pos, $1);}
	| my_fnCall StmtEnd { $$ = A_BlockCallStmt($1->pos, A_CallStmt($1->pos, $1));} 
	| my_ifStmt {$$ = A_BlockIfStmt($1->pos,$1);}
	| my_whileStmt {$$ = A_BlockWhileStmt($1->pos,$1);}
	| RET_ my_rightVal StmtEnd {$$ = A_BlockReturnStmt($1,A_ReturnStmt($1,$2));}
	| RET_ StmtEnd {$$ = A_BlockReturnStmt($1,A_ReturnStmt($1,0));}
	| CONTINUE StmtEnd {$$ = A_BlockContinueStmt($1);} 
	| BREAK StmtEnd {$$ = A_BlockBreakStmt($1);} 
	;

my_ifWhileBoolExpr:my_boolExpr {$$=$1;}
	;
my_fnDecl: term LP my_ParamDecl RP my_fnRetType{
		$$ = A_FnDecl($1->pos,$1->id,A_ParamDecl($3),$5);
	}
	|term LP my_ParamDecl RP{
		$$ = A_FnDecl($1->pos,$1->id,A_ParamDecl($3),0);
	}
	;
my_ParamDecl: {	$$ = 0;}
	| my_NotNullParaDecl{
		$$=$1;
	}
	;
my_NotNullParaDecl: 
	my_varDecl {
		$$=A_VarDeclList($1,0);
	}
	| my_varDecl Comma my_NotNullParaDecl{
		$$=A_VarDeclList($1,$3);
	}
	;
my_fnRetType:FnRetTypeDecl Type {
		$$ = $2;
	}
	;

Type: term {
		$$ = A_StructType($1->pos, $1->id);
	} 
	| NativeType {
		$$ = $1;
	}
	;

my_rightVal:my_arithExpr {$$=A_ArithExprRVal($1->pos, $1);}
	|my_boolExpr {$$=A_BoolExprRVal($1->pos, $1);}
	;
my_arithExpr:my_arithBiOpExpr {$$=A_ArithBiOp_Expr($1->pos,$1);}
	|my_exprUnit {$$=A_ExprUnit($1->pos,$1);}
	;
my_arithBiOpExpr:my_arithExpr SUB my_arithExpr {$$=A_ArithBiOpExpr($1->pos,$2,$1,$3);}
	| my_arithExpr ADD my_arithExpr {$$=A_ArithBiOpExpr($1->pos,$2,$1,$3);}
	| my_arithExpr MUL my_arithExpr {$$=A_ArithBiOpExpr($1->pos,$2,$1,$3);}
	| my_arithExpr DIV my_arithExpr {$$=A_ArithBiOpExpr($1->pos,$2,$1,$3);}
	;
my_exprUnit:NUMBER {$$=A_NumExprUnit($1->pos, $1->num);}
	|term {$$= A_IdExprUnit($1->pos, $1->id);}
    |LP my_arithExpr RP {$$ = A_ArithExprUnit($1,$2);}
	|my_fnCall {$$=A_CallExprUnit($1->pos,$1);}
	|my_leftVal MLP my_indexExpr MRP {$$ = A_ArrayExprUnit($1->pos,A_ArrayExpr($1->pos,$1,$3));}
	|my_memberExpr {$$ =A_MemberExprUnit($1->pos, $1);}
	|my_arithUExpr {$$=A_ArithUExprUnit($1->pos, $1);} //-1
	;
my_fnCall: term LP my_rightValList RP{$$ = A_FnCall($1->pos,$1->id,$3);}
	;
my_arithUExpr:SUB my_exprUnit  %prec NUMBER{$$=A_ArithUExpr($2->pos,A_neg,$2);}
	;

my_memberExpr:my_leftVal POINT term {$$=A_MemberExpr($1->pos,$1,$3->id );}
	;
my_indexExpr:NUMBER {$$=A_NumIndexExpr($1->pos,$1->num);}
	|term {$$=A_IdIndexExpr($1->pos,$1->id);}
	;
my_comExpr: my_exprUnit LT my_exprUnit {$$=A_ComExpr($1->pos,A_lt,$1,$3);}
	|my_exprUnit LE my_exprUnit {$$=A_ComExpr($1->pos,A_le,$1,$3);}
	|my_exprUnit GT my_exprUnit {$$=A_ComExpr($1->pos,A_gt,$1,$3);}
	|my_exprUnit GE my_exprUnit {$$=A_ComExpr($1->pos,A_ge,$1,$3);}
	|my_exprUnit EEQ my_exprUnit {$$=A_ComExpr($1->pos,A_eq,$1,$3);}
	|my_exprUnit NE my_exprUnit {$$=A_ComExpr($1->pos,A_ne,$1,$3);}
	;
my_boolExpr:my_boolBiOpExpr {$$=A_BoolBiOp_Expr($1->pos,$1);}
	| my_boolUnit  {$$=A_BoolExpr($1->pos,$1);}
	;
my_boolBiOpExpr:my_boolExpr AND my_boolExpr {$$=A_BoolBiOpExpr($1->pos,A_and,$1,$3);}
	|my_boolExpr OR my_boolExpr {$$=A_BoolBiOpExpr($1->pos,A_or,$1,$3);}
	;
my_boolUnit: my_comExpr {$$ = A_ComExprUnit($1->pos, $1);}
	|LP my_boolExpr RP {$$=A_BoolExprUnit($1,$2);}
	|my_boolUOpExpr {$$=A_BoolUOpExprUnit($1->pos,$1);}//!
	;
my_boolUOpExpr:NOT my_boolUnit {$$=A_BoolUOpExpr($1,A_not,$2);}
	;


%%
A_pos my_pos()
{
	return A_Pos(line,col);
}
extern "C"{
void yyerror(char * s)
{
  fprintf(stderr, "%s\n",s);
}
int yywrap()
{
  return(1);
}

}


