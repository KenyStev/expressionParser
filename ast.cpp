#include "ast.h"

using namespace std;

static int countTemp=0;
string temps[] = {"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9"};
map<string, int> vars;

void releaseTemp(string tmp)
{
	if(vars[tmp]!=0)
	{
		vars[tmp]=0;
	}
}

string nextTemp()
{
	for (int i = 0; i < 10; ++i)
		if (vars.find(temps[i]) == vars.end() || vars[temps[i]] == 0)
		{
			vars[temps[i]] = 1;
			return temps[i];
		}
	return "";
}

void NumExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	string r = nextTemp();
	rv->place = (char*)malloc(r.size()+1);
	strcpy(rv->place, r.c_str());

	string code = "li " + r + ", " + lexeme;
	rv->code = (char*)malloc(code.size()+1);
	strcpy(rv->code,code.c_str());
}

void IdExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	string r = nextTemp();
	rv->place = (char*)malloc(r.size()+1);
	strcpy(rv->place,r.c_str());

	string code = "lw " + r + ", " + id;
	rv->code = (char*)malloc(code.size()+1);
	strcpy(rv->code,code.c_str());
}

void AddExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	returnValue_t *left_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	returnValue_t *right_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	expr1->generateCode(left_rv);
	expr2->generateCode(right_rv);

	string code = string(left_rv->code) + "\n" + string(right_rv->code) + "\n";
	releaseTemp(left_rv->place);
	releaseTemp(right_rv->place);

	string r = nextTemp();

	if (expr1->getKind() == NUM_EXPR)
	{
		code = string(right_rv->code) + "\n";
		code += "addi " + r + ", " + string(right_rv->place) + ", " + string(((NumExpr*)expr1)->lexeme);
	}else if(expr2->getKind() == NUM_EXPR)
	{
		code = string(left_rv->code) + "\n";
		code += "addi " + r + ", " + string(left_rv->place) + ", " + string(((NumExpr*)expr2)->lexeme);
	}else{
		code += "add " + r + ", " + string(left_rv->place) + ", " + string(right_rv->place);
	}


	rv->place = (char*)malloc(r.size());
	strcpy(rv->place,r.c_str());

	rv->code = (char*)malloc(code.size());
	strcpy(rv->code,code.c_str());

	free(left_rv->code);
	free(right_rv->place);
	free(left_rv);
	free(right_rv);
}

void SubExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	returnValue_t *left_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	returnValue_t *right_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	expr1->generateCode(left_rv);
	expr2->generateCode(right_rv);

	string code = string(left_rv->code) + "\n" + string(right_rv->code) + "\n";
	releaseTemp(left_rv->place);
	releaseTemp(right_rv->place);

	string r = nextTemp();

	// if (expr1->getKind() == NUM_EXPR)
	// {
	// 	code = string(right_rv->code) + "\n";
	// 	int val = ((NumExpr*)expr1)->value*-1;
	// 	char buf[10];
	// 	sprintf(buf,"%d",val);
	// 	code += "addi " + r + ", " + string(right_rv->place) + ", " + string(buf);
	// }else if(expr2->getKind() == NUM_EXPR)
	// {
	// 	code = string(left_rv->code) + "\n";
	// 	code += "addi " + r + ", " + string(left_rv->place) + ", " + string(((NumExpr*)expr2)->lexeme);
	// }else{
	// 	code += "sub " + r + ", " + string(left_rv->place) + ", " + string(right_rv->place);
	// }


	rv->place = (char*)malloc(r.size());
	strcpy(rv->place,r.c_str());

	rv->code = (char*)malloc(code.size());
	strcpy(rv->code,code.c_str());

	free(left_rv->code);
	free(right_rv->place); 
	free(left_rv);
	free(right_rv);
}

void MultExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	returnValue_t *left_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	returnValue_t *right_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	expr1->generateCode(left_rv);
	expr2->generateCode(right_rv);

	string code = string(left_rv->code) + "\n" + string(right_rv->code) + "\n";
	releaseTemp(left_rv->place);
	releaseTemp(right_rv->place);

	string r = nextTemp();

	code += "mult " + string(left_rv->place) + ", " + string(right_rv->place) + "\n";
	code += "mflo " + r;

	rv->place = (char*)malloc(r.size());
	strcpy(rv->place,r.c_str());

	rv->code = (char*)malloc(code.size());
	strcpy(rv->code,code.c_str());

	free(left_rv->code);
	free(right_rv->place);
	free(left_rv);
	free(right_rv);
}

void DivExpr::generateCode(returnValue_t * rv)
{
	if(!rv)return;

	returnValue_t *left_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	returnValue_t *right_rv = (returnValue_t*)malloc(sizeof(returnValue_t));
	expr1->generateCode(left_rv);
	expr2->generateCode(right_rv);

	string code = string(left_rv->code) + "\n" + string(right_rv->code) + "\n";
	releaseTemp(left_rv->place);
	releaseTemp(right_rv->place);

	string r = nextTemp();

	code += "div " + string(left_rv->place) + ", " + string(right_rv->place) + "\n";
	code += "mflo " + r;

	rv->place = (char*)malloc(r.size());
	strcpy(rv->place,r.c_str());

	rv->code = (char*)malloc(code.size());
	strcpy(rv->code,code.c_str());

	free(left_rv->code);
	free(right_rv->place);
	free(left_rv);
	free(right_rv);
}