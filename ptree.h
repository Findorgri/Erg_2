#include <stdlib.h>
#include <stdio.h>

extern struct ptnode *root;

typedef enum nodetype {
	NT_NUMBER, NT_IDENTIFIER,
	NT_ADDITION, NT_SUBTRACTION, NT_MULTIPLICATION, NT_DIVISION,
	NT_ASSIGNMENT, NT_UNARYPLUS, NT_UNARYMINUS, NT_STATEMENT, NT_COMPILATIONUNIT
} NODETYPE;


struct ptnode
{
	int m_semanticValue;
	NODETYPE m_type;
	int m_serialNumber;
	char *m_label;

	char *m_identifier;

	struct ptnode *m_leftChild;
	struct ptnode *m_rightChild;
};

struct ptnode *NewNode(NODETYPE type,
	struct ptnode *left, struct ptnode *right, char *yytext);
void printGraphViz(FILE *fp, struct ptnode *current);
struct ptnode *lookup(char *identifier, bool query);
int eval(struct ptnode *current);
