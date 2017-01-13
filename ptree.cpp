#include "ptree.h"
#include <cstring>
#include <cstdio>

int serialNumberCounter = 0;
const char *nodeLabels[] = { "NT_NUMBER", "NT_IDENTIFIER",
"NT_ADDITION", "NT_SUBTRACTION", "NT_MULTIPLICATION", "NT_DIVISION",
"NT_ASSIGNMENT", "NT_UNARYPLUS", "NT_UNARYMINUS", "NT_STATEMENT", "NT_COMPILATIONUNIT",
"NT_FUNCTIONDEF", "NT_FORMALARGS", "NT_COMPOUND", "NT_STATEMENTLIST", "NT_IF", "NT_WHILE", "NT_RETURN", "NT_EQUALS",
"NT_NEQUAL", "NT_LESSTHAN", "NT_GREATERTHAN", "NT_LTE", "NT_GTE", "NT_AND", "NT_OR", "NT_NOT", "NT_PARENTHESIS",
"NT_FUNCTIONCALL", "NT_EXP", "NT_SQRT", "NT_POW", "NT_PARAMLIST" };
struct ptnode *root;
struct ptnode *symbols[50] = { NULL };
double returnValue;

struct ptnode *NewNode(NODETYPE type,
	struct ptnode *left, struct ptnode *right, char *yytext)
{
	char serial[30];
	struct ptnode *newNode;
	newNode = (struct ptnode *)malloc(sizeof(struct ptnode));

	newNode->m_type = type;
	newNode->m_serialNumber = serialNumberCounter++;

	newNode->m_leftChild = left;
	newNode->m_rightChild = right;
	newNode->m_extraChild = NULL;

	if (type == NT_IDENTIFIER) {
		newNode->m_identifier = strdup(yytext);
	}
	else {
		newNode->m_identifier = NULL;
	}

	newNode->m_label = (char *)malloc(50);
	strcpy(newNode->m_label, nodeLabels[type]);
	strcat(newNode->m_label, "_");
	itoa(newNode->m_serialNumber, serial, 10);
	strcat(newNode->m_label, serial);

	if (yytext != NULL)
	{
		newNode->m_semanticValue = atoi(yytext);
	}
	else
	{
		newNode->m_semanticValue = -1;
	}

	newNode->m_functionRoot = NULL;

	return newNode;
}

struct ptnode *NewNode(NODETYPE type,
	struct ptnode *left, struct ptnode *right, struct ptnode* extra, char *yytext)
{
	char serial[30];
	struct ptnode *newNode;
	newNode = (struct ptnode *)malloc(sizeof(struct ptnode));

	newNode->m_type = type;
	newNode->m_serialNumber = serialNumberCounter++;

	newNode->m_leftChild = left;
	newNode->m_rightChild = right;
	newNode->m_extraChild = extra;

	if (type == NT_IDENTIFIER) {
		newNode->m_identifier = strdup(yytext);
	}
	else {
		newNode->m_identifier = NULL;
	}

	newNode->m_label = (char *)malloc(50);
	strcpy(newNode->m_label, nodeLabels[type]);
	strcat(newNode->m_label, "_");
	itoa(newNode->m_serialNumber, serial, 10);
	strcat(newNode->m_label, serial);

	if (yytext != NULL)
	{
		newNode->m_semanticValue = atoi(yytext);
	}
	else
	{
		newNode->m_semanticValue = -1;
	}

	newNode->m_functionRoot = NULL;

	return newNode;
}

void printGraphViz(FILE *fp, struct ptnode *current)
{
	if (current->m_leftChild != NULL)
	{
		fprintf(fp, "\"%s\" -> \"%s\";\n", current->m_label, current->m_leftChild->m_label);
		printGraphViz(fp, current->m_leftChild);
	}
	if (current->m_rightChild != NULL)
	{
		fprintf(fp, "\"%s\" -> \"%s\";\n", current->m_label, current->m_rightChild->m_label);
		printGraphViz(fp, current->m_rightChild);
	}
	if (current->m_extraChild != NULL)
	{
		fprintf(fp, "\"%s\" -> \"%s\";\n", current->m_label, current->m_extraChild->m_label);
		printGraphViz(fp, current->m_extraChild);
	}
}

struct ptnode *lookup(char *identifier, bool query)
{
	int index = 0;
	struct ptnode *newnode;
	struct ptnode *p = symbols[0];

	while (p != NULL) {
		if (!strcmp(identifier, p->m_identifier)) {
			return p;
		}
		// I swapped the two following statements and worked
		index++;
		p = symbols[index];
	}
	if (query) {
		return NULL;
	}
	else {
		newnode = NewNode(NT_IDENTIFIER, NULL, NULL, identifier);
		symbols[index--] = newnode;
		return newnode;
	}

}

double eval(struct ptnode *current)
{
	double result;
	struct ptnode *lhs;

	if (current->m_type == NT_COMPILATIONUNIT) {
		eval(current->m_leftChild);
		if (current->m_rightChild != NULL) {
			eval(current->m_rightChild);
		}

		return 0;
	}
	else if (current->m_type == NT_STATEMENT) {
		if (current->m_leftChild != NULL) {
			//printf("Result = %d\n", eval(current->m_leftChild));
			result = eval(current->m_leftChild);
		}

		return 0;
	}
	else if (current->m_type == NT_ADDITION) {
		return eval(current->m_leftChild) + eval(current->m_rightChild);
	}
	else if (current->m_type == NT_SUBTRACTION) {
		return eval(current->m_leftChild) - eval(current->m_rightChild);
	}
	else if (current->m_type == NT_MULTIPLICATION) {
		return eval(current->m_leftChild) * eval(current->m_rightChild);
	}
	else if (current->m_type == NT_DIVISION) {
		if (result = eval(current->m_rightChild) != 0) {
			return eval(current->m_leftChild) / result;
		}
		else {
			printf("kara....!!#!$@#$!@#@");
		}
	}
	else if (current->m_type == NT_UNARYMINUS) {
		return -eval(current->m_leftChild);
	}
	else if (current->m_type == NT_UNARYPLUS) {
		return eval(current->m_leftChild);
	}
	else if (current->m_type == NT_NUMBER) {
		return current->m_semanticValue;
	}
	else if (current->m_type == NT_IDENTIFIER) {
		return current->m_semanticValue;
	}
	else if (current->m_type == NT_ASSIGNMENT) {
		lhs = lookup(current->m_leftChild->m_identifier, true);
		lhs->m_semanticValue = eval(current->m_rightChild);
		printf("%s = %f\n", current->m_leftChild->m_identifier, lhs->m_semanticValue);
		return lhs->m_semanticValue;
	}
	else if (current->m_type == NT_EQUALS) {
		return eval(current->m_leftChild) == eval(current->m_rightChild);
	}
	else if (current->m_type == NT_NEQUAL) {
		return eval(current->m_leftChild) != eval(current->m_rightChild);
	}
	else if (current->m_type == NT_LESSTHAN) {
		return eval(current->m_leftChild) < eval(current->m_rightChild);
	}
	else if (current->m_type == NT_GREATERTHAN) {
		return eval(current->m_leftChild) > eval(current->m_rightChild);
	}
	else if (current->m_type == NT_LTE) {
		return eval(current->m_leftChild) <= eval(current->m_rightChild);
	}
	else if (current->m_type == NT_GTE) {
		return eval(current->m_leftChild) >= eval(current->m_rightChild);
	}
	else if (current->m_type == NT_OR) {
		return eval(current->m_leftChild) || eval(current->m_rightChild);
	}
	else if (current->m_type == NT_AND) {
		return eval(current->m_leftChild) && eval(current->m_rightChild);
	}
	else if (current->m_type == NT_NOT) {
		return !eval(current->m_leftChild);
	}
	else if (current->m_type == NT_PARENTHESIS) {
		return eval(current->m_leftChild);
	}
	else if (current->m_type == NT_STATEMENTLIST) {
		eval(current->m_leftChild);
		if (current->m_rightChild != NULL)
			eval(current->m_rightChild);
	}
	else if (current->m_type == NT_COMPOUND) {
		if (current->m_leftChild != NULL)
			eval(current->m_leftChild);
		else
			return 0;
	}
	else if (current->m_type == NT_WHILE) {
		while (eval(current->m_leftChild))
			eval(current->m_rightChild);
	}
	else if (current->m_type == NT_IF) {
		if (eval(current->m_leftChild))
			eval(current->m_rightChild);
		else
			if (current->m_extraChild)
				eval(current->m_extraChild);
	}
	else if (current->m_type == NT_FUNCTIONCALL) {
		struct ptnode* funDef = current->m_leftChild->m_functionRoot;
		mapFormaltoActual(current->m_rightChild, funDef->m_rightChild);
		eval(current->m_extraChild);
		return returnValue;
	}
	else if (current->m_type == NT_EXP) {
		return exp(eval(current->m_leftChild));
	}
	else if (current->m_type == NT_SQRT) {
		return sqrt(eval(current->m_leftChild));
	}
	else if (current->m_type == NT_POW) {
		return pow(eval(current->m_leftChild->m_leftChild->m_leftChild), eval(current->m_leftChild->m_rightChild));
	}

	else if (current->m_type == NT_RETURN) {
		return returnValue = eval(current->m_leftChild);
	}

}



void mapFormaltoActual(struct ptnode* actual, struct ptnode* formal) {

	struct ptnode* aa = actual, *fa = formal;

	while (fa->m_type == NT_FORMALARGS) {

		if (fa->m_rightChild != NULL)
			fa->m_rightChild->m_semanticValue = eval(aa->m_rightChild);
		if (fa->m_leftChild != NULL && fa->m_leftChild->m_type == NT_IDENTIFIER)
			fa->m_leftChild->m_semanticValue = eval(aa->m_leftChild);

		aa = aa->m_leftChild;
		fa = fa->m_leftChild;

	}
}
