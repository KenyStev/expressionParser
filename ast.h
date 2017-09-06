#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <string.h>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#define DEF_BINARY_EXPR(name,pref,op) class name##Expr: public BinaryExpr { \
public: \
    name##Expr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {} \
    int evaluate() { return expr1->evaluate() op expr2->evaluate(); } \
    int getKind() { return pref##_EXPR; } \
    void generateCode(returnValue_t * rv); }; \

using namespace std;

extern map<string, int> vars;

typedef struct returnValue
{
	char *code;
	char *place;
} returnValue_t;

enum ExprKind {
  ADD_EXPR,
  SUB_EXPR,
  MULT_EXPR,
  DIV_EXPR,
  NUM_EXPR,
  ID_EXPR
};

class Expr;
typedef list<Expr*> ExprList;

class Expr {
public:
	virtual void generateCode(returnValue_t *) = 0;
    virtual int evaluate() = 0;
    virtual int getKind() = 0;
    bool isA(int kind) { return (getKind() == kind); }
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr *expr1, Expr *expr2) {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }

    Expr *expr1;
    Expr *expr2;
};

DEF_BINARY_EXPR(Add,ADD,+);
DEF_BINARY_EXPR(Sub,SUB,-);
DEF_BINARY_EXPR(Mult,MULT,*);
DEF_BINARY_EXPR(Div,DIV,/);

class NumExpr: public Expr {
public:
    NumExpr(char* lexeme) { this->lexeme = lexeme; value = atoi(lexeme); }
    int evaluate() { return value; }
    int getKind() { return NUM_EXPR; }
    void generateCode(returnValue_t * rv);
    char *lexeme;
    int value;
};

class IdExpr: public Expr {
public:
    IdExpr(string id) { this->id = id; }
    int evaluate() { return vars[id]; }
    int getKind() { return ID_EXPR; }

    void generateCode(returnValue_t * rv);

    string id;
};

#endif