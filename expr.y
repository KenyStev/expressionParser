%{
#include <cstdio>
#include <string>
#include "ast.h"

using namespace std;

extern int yylineno;

int yylex();

void yyerror(const char *str)
{
    printf("Line %d: %s\n", yylineno, str);
}

Expr* getExpression(Expr*expr1,Expr*expr2,ExprKind kind)
{
	if (expr1->getKind() == NUM_EXPR && expr2->getKind() == NUM_EXPR)
	{
		int val = 0, v1 = ((NumExpr*)expr1)->value, v2 = ((NumExpr*)expr2)->value;

		switch(kind)
		{
			case ADD_EXPR: val = v1 + v2; break;
			case SUB_EXPR: val = v1 - v2; break;
			case MULT_EXPR: val = v1 * v2; break;
			case DIV_EXPR: val = v1 / v2; break;
			default: return NULL;
		}

		char buffer[10];
		sprintf(buffer,"%d",val);

		string s = string(buffer);
		char*ss = (char*)malloc(s.size()+1);
		strcpy(ss,s.c_str());
		return new NumExpr(ss);
	}else{
		switch(kind)
		{
			case ADD_EXPR: return new AddExpr(expr1,expr2);
			case SUB_EXPR: return new SubExpr(expr1,expr2);
			case MULT_EXPR: return new MultExpr(expr1,expr2);
			case DIV_EXPR: return new DivExpr(expr1,expr2);
			default: return NULL;
		}
	}
}

Statement *input;

#define YYERROR_VERBOSE 1

%}

%union {
	int num_t;
	char *id_t;
	Statement *statement_t;
	BlockStatement *blkstatement_t;
    Expr *expr_t;
    ExprList *exprlist_t;
}

%token<num_t> TK_ERROR TK_EOL
%token<id_t> TK_ID TK_NUM

%type<expr_t> E F T
%type<statement_t> input statement expression_statement
%type<blkstatement_t> statement_list

%%

input: statement_list 	{ input = $1; }
;

statement_list: statement_list statement 	{ $$ = $1; $$->add($2); }
              | statement 					{ $$ = new BlockStatement; $$->add($1); }
;

statement: expression_statement ';' { $$ = $1; }
;

expression_statement:	E 		{ 
	returnValue_t *rv = new returnValue_t(); 
	$$ = new ExpressionStatement($1); 
	//$$->generateCode(rv);
	//printf("code: \n\n%s\n", rv->code);
	//printf("\nplace: %s\n", rv->place); 
	}
;

E: 	  E '+' F	{ $$ = getExpression($1,$3,ADD_EXPR); }
	| E '-' F 	{ $$ = getExpression($1,$3,SUB_EXPR); }
	| F			{ $$ = $1; }
;

F:    F '*' T 	{ $$ = getExpression($1,$3,MULT_EXPR); }
	| F '/' T 	{ $$ = getExpression($1,$3,DIV_EXPR); }
	| T 		{ $$ = $1; }
;

T:	  TK_NUM	{ $$ = new NumExpr($1); }
	| TK_ID		{ 
		string text = $1;
		free($1);

		$$ = new IdExpr(text);
	 }
	| '(' E ')'	{ $$ = $2; }
;

%%