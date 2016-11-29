#include "ptree.h"
#include <cstring>
#include <cstdio>

int serialNumberCounter = 0;
const char *nodeLabels[] = { "NT_NUMBER", "NT_IDENTIFIER",
"NT_ADDITION", "NT_SUBTRACTION", "NT_MULTIPLICATION", "NT_DIVISION",
"NT_ASSIGNMENT", "NT_UNARYPLUS", "NT_UNARYMINUS", "NT_STATEMENT", "NT_COMPILATIONUNIT" };
struct ptnode *root;
struct ptnode *symbols[50] = { NULL };

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
}

struct ptnode *lookup(char *identifier, bool query)
{
	int index = 0;
	struct ptnode *newnode;
	struct ptnode *p = symbols[0];

	while (p != NULL) {
		// i strcmp epistrefei 0 gia isotita 
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
		symbols[index] = newnode;
		return newnode;
	}

}

int eval(struct ptnode *current)
{
	int result;
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
			printf("Result = %d\n", eval(current->m_leftChild));
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
		return lhs->m_semanticValue = eval(current->m_rightChild);
	}
}
