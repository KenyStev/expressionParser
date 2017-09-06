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

E: 	  E '+' F	{ $$ = new AddExpr($1,$3); }
	| E '-' F 	{ $$ = new SubExpr($1,$3); }
	| F			{ $$ = $1; }
;

F:    F '*' T 	{ $$ = new MultExpr($1,$3); }
	| F '/' T 	{ $$ = new DivExpr($1,$3); }
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