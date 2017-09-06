#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <string.h>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>

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

class AddExpr: public BinaryExpr {
public:
    AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() + expr2->evaluate(); }
    int getKind() { return ADD_EXPR; }

    void generateCode(returnValue_t * rv);
};

class SubExpr: public BinaryExpr {
public:
    SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() - expr2->evaluate(); }
    int getKind() { return SUB_EXPR; }

    void generateCode(returnValue_t * rv);
};

class MultExpr: public BinaryExpr {
public:
    MultExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() * expr2->evaluate(); }
    int getKind() { return MULT_EXPR; }

    void generateCode(returnValue_t * rv);
};

class DivExpr: public BinaryExpr {
public:
    DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() / expr2->evaluate(); }
    int getKind() { return DIV_EXPR; }

    void generateCode(returnValue_t * rv);
};

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