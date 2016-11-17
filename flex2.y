%{
#include <stdio.h>
#include <stdlib.h>
#include "ptree.h"

int yylex(void);

int yyerror(const char *message);

extern FILE* yyin;
%}

%union{

	struct ptnode* pt;

}


%token Q_MARK PLUS
%start addition_list

%token <pt> NUMBER
%type <pt> addition_list addition

%%

addition_list : addition Q_MARK {root = $$ = new_Node(NT_addition_list, $1, NULL, NULL);}
				| addition_list addition Q_MARK {root = $$ = new_Node(NT_addition_list, $1, $2, NULL);}
				;

addition : NUMBER PLUS NUMBER {$$ = new_Node(NT_addition, $1, $3, NULL);}
			| addition PLUS NUMBER {$$ = new_Node(NT_addition, $1, $3, NULL);}
			;

%%

int main(int argc, char **argv){

	FILE* fp;
	
	yyin = fopen("test.txt", "r");
	yyparse();

	fp = fopen("tree.dot", "w");
	fprintf(fp, "digraph g{\n");
	printTraversal(fp, root);
	fprintf(fp, "}\n");
	fclose(fp);

	system("dot -Tgif tree.dot -o tree.gif");

	return 0;

}

int yyerror(const char *message){

	printf("Error: %s\n", message);
	return -1;

}