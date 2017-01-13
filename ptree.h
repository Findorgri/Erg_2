#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern struct ptnode *root;

typedef enum nodetype {
	NT_NUMBER, NT_IDENTIFIER,
	NT_ADDITION, NT_SUBTRACTION, NT_MULTIPLICATION, NT_DIVISION,
	NT_ASSIGNMENT, NT_UNARYPLUS, NT_UNARYMINUS, NT_STATEMENT, NT_COMPILATIONUNIT,
	NT_FUNCTIONDEF, NT_FORMALARGS, NT_COMPOUND, NT_STATEMENTLIST, NT_IF, NT_WHILE, NT_RETURN, NT_EQUALS, NT_NEQUAL,
	NT_LESSTHAN, NT_GREATERTHAN, NT_LTE, NT_GTE, NT_AND, NT_OR, NT_NOT, NT_PARENTHESIS, NT_FUNCTIONCALL, NT_EXP, NT_SQRT,
	NT_POW, NT_PARAMLIST
} NODETYPE;


struct ptnode
{
	double m_semanticValue;
	NODETYPE m_type;
	int m_serialNumber;
	char *m_label;

	char *m_identifier;

	struct ptnode* m_functionRoot;

	struct ptnode *m_leftChild;
	struct ptnode *m_rightChild;
	struct ptnode* m_extraChild;
};

struct ptnode *NewNode(NODETYPE type,
	struct ptnode *left, struct ptnode *right, char *yytext);
struct ptnode *NewNode(NODETYPE type,
	struct ptnode *left, struct ptnode *right, struct ptnode* m_extraChild, char *yytext);
void printGraphViz(FILE *fp, struct ptnode *current);
struct ptnode *lookup(char *identifier, bool query);
double eval(struct ptnode *current);
void mapFormaltoActual(struct ptnode* actual, struct ptnode* formal);
