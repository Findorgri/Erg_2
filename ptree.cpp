#include "ptree.h"
#include <cstring>
#include <cstdio>

int SerialNumber = 0;

struct ptnode* root;

const char* nodestrings[] = { "NUMBER", "addition_list", "addition" };

struct ptnode* new_Node(NODETYPE n_type, struct ptnode* left, struct ptnode* right, char* yytext) {

	struct ptnode* newNode = (struct ptnode*) malloc(sizeof(struct ptnode));

	newNode->SerialNumber = SerialNumber++;

	newNode->type = n_type;
	newNode->left_child = left;
	newNode->right_child = right;

	newNode->graph_label = (char*)malloc(50);
	strcpy(newNode->graph_label, nodestrings[n_type]);
	strcat(newNode->graph_label, "_");
	char buffer[30];
	itoa(newNode->SerialNumber, buffer, 10);
	strcat(newNode->graph_label, buffer);

	if (yytext != NULL) {

		newNode->semanticValue = atoi(yytext);

	}
	else {

		newNode->semanticValue = -1;

	}

	return newNode;

}

void printTraversal(FILE * fp, struct ptnode * current_n)
{

	if (current_n->left_child != NULL) {

		fprintf(fp, "\"%s\" -> \"%s\";\n", current_n->graph_label, current_n->left_child->graph_label);
		printTraversal(fp, current_n->left_child);

	}

	if (current_n->right_child != NULL) {

		fprintf(fp, "\"%s\" -> \"%s\";\n", current_n->graph_label, current_n->right_child->graph_label);
		printTraversal(fp, current_n->right_child);

	}

}

