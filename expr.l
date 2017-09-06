%option noyywrap
%option yylineno
%option nodefault

%x literal_string

%{
#include <cstdlib>
#include <cstring>
#include "ast.h"
#include "tokens.h"

#define YYERROR_VERBOSE 1
%}

DIGIT [0-9]
LETTER [a-zA-Z]

%%

"//".*	 	{ /* Comment */ }
[ \t\n]		{ /* Nothing */ }
";"			{ return ';'; }
"+"			{ return '+'; }
"-"			{ return '-'; }
"*"			{ return '*'; }
"/"			{ return '/'; }
"("			{ return '('; }
")"			{ return ')'; }
{DIGIT}+	{ yylval.id_t = (char*)malloc(yyleng+1); strcpy(yylval.id_t,yytext); return TK_NUM; }
{LETTER}+	{ yylval.id_t = (char*)malloc(yyleng+1); strcpy(yylval.id_t,yytext); return TK_ID; }
.			{ printf("Unexpected Token! %s ",yytext); return TK_ERROR; }

%%