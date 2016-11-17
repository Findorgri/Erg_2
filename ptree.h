#include <stdio.h>
#include <stdlib.h>

extern struct ptnode* root;

typedef enum nodetype{

	NT_NUMBER, NT_addition_list, NT_addition

}NODETYPE;

struct ptnode{

	int semanticValue;
	int SerialNumber;
	NODETYPE type;
	char* graph_label;

	struct ptnode* left_child;
	struct ptnode* right_child;

};

struct ptnode* new_Node(NODETYPE n_type, struct ptnode* left, struct ptnode* right, char* yytext);

void printTraversal(FILE* fp, struct ptnode* current_n);