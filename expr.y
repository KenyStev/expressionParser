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

Expr* getExpression(Expr*expr1,Expr*expr2)
{
	if (expr1->getKind() == NUM_EXPR && expr2->getKind() == NUM_EXPR)
	{
		int val = ((NumExpr*)expr1)->value + ((NumExpr*)expr2)->value;
		string s = ""+val;
		char*ss = (char*)malloc(s.size()+1);
		strcpy(ss,s.c_str());
		return new NumExpr(ss);
	}
}

#define YYERROR_VERBOSE 1

//Statement *input;
%}

%union {
	int num_t;
	char *id_t;
	Expr *expr_t;
}

%token<num_t> TK_ERROR TK_EOL
%token<id_t> TK_ID TK_NUM

%type<expr_t> E F T input

%%

input:	E 		{ 
	returnValue_t *rv = new returnValue_t(); 
	$$ = $1; $$->generateCode(rv);
	printf("code: %s\n", rv->code);
	printf("place: %s\n", rv->place); }
;

E: 	  E '+' F	{ $$ = getExpression($1,$3); }
	| E '-' F 	{ $$ = getExpression($1,$3); }
	| F			{ $$ = $1; }
;

F:    F '*' T 	{ $$ = getExpression($1,$3); }
	| F '/' T 	{ $$ = getExpression($1,$3); }
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