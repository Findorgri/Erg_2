%{
#include <stdio.h>
#include <stdlib.h>
#include "ptree.h"

int yylex(void);
int yyerror(const char *msg);

extern FILE *yyin;
%}

%union{
	struct ptnode *pt;
}

%verbose
%error-verbose

%start compilation_unit

%right '='
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%token <pt> NUMBER IDENTIFIER
%type <pt> compilation_unit statement expression
%%

compilation_unit : statement { root = $$ = NewNode(NT_COMPILATIONUNIT, $1, NULL, NULL); }
				 | compilation_unit statement { root = $$ = NewNode(NT_COMPILATIONUNIT, $1, $2, NULL); }
				 ;

statement : expression ';' { $$ = NewNode(NT_STATEMENT, $1, NULL, NULL); }
		  | ';' { $$ = NewNode(NT_STATEMENT, NULL, NULL, NULL); }
		  ;

expression : NUMBER { $$ = $1 }
		   | IDENTIFIER { $$ = $1 }
		   | IDENTIFIER '=' expression { $$ = NewNode(NT_ASSIGNMENT, $1, $3, NULL); }
		   | expression '+' expression { $$ = NewNode(NT_ADDITION, $1, $3, NULL); }
		   | expression '-' expression { $$ = NewNode(NT_SUBTRACTION, $1, $3, NULL); }
		   | expression '*' expression { $$ = NewNode(NT_MULTIPLICATION, $1, $3, NULL); }
		   | expression '/' expression { $$ = NewNode(NT_DIVISION, $1, $3, NULL); }
		   | '+' expression %prec UMINUS { $$ = NewNode(NT_UNARYPLUS, $2, NULL, NULL); }
		   | '-' expression %prec UMINUS { $$ = NewNode(NT_UNARYMINUS, $2, NULL, NULL); }
		   ;

%%

int main(int argc, char **argv)
{
	int result;

	FILE *fp;
	yyin = fopen("test.txt", "r");
	yyparse();
	
	
	fp = fopen("tree.dot", "w");
	fprintf(fp, "digraph g{\n");
	printGraphViz(fp, root);
	fprintf(fp, "}\n");
	fclose(fp);

	system("dot -Tgif tree.dot -o tree.gif");
	result = eval(root);
	return 0;
}

int yyerror(const char *msg)
{
	printf("Error: %s\n", msg);
	return 1;
}

/*

addition_list : addition Q_MARK	{ root = $$ = NewNode(NT_addition_list, $1, NULL, NULL); }
			  | addition_list addition Q_MARK { root = $$ = NewNode(NT_addition_list, $1, $2, NULL); }
			  ;

addition : NUMBER PLUS NUMBER			{ $$ = NewNode(NT_addition, $1, $3, NULL); }
		 | addition PLUS NUMBER			{ $$ = NewNode(NT_addition, $1, $3, NULL); }
		 ;
*/
