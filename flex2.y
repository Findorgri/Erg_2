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
%left OR
%left AND
%left EQUALS NEQUAL '<' '>' LTE GTE
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS NOT
%nonassoc ')'
%nonassoc ELSE

%token <pt> NUMBER IDENTIFIER WHILE IF RETURN POW EXP SQRT FUNCTION
%type <pt> compilation_unit statement expression functiondef formalargs if_stmnt while_stmnt compound_stmnt statement_list param_list
%%

compilation_unit : statement { root = $$ = NewNode(NT_COMPILATIONUNIT, $1, NULL, NULL); }
				 | compilation_unit statement { root = $$ = NewNode(NT_COMPILATIONUNIT, $1, $2, NULL); }
				 | functiondef { $$ = NewNode(NT_COMPILATIONUNIT, $1, NULL, NULL); }
				 | compilation_unit functiondef { $$ = NewNode(NT_COMPILATIONUNIT, $1, $2, NULL); }
				 ;

functiondef : FUNCTION IDENTIFIER '(' formalargs ')' compound_stmnt { $$ = NewNode(NT_FUNCTIONDEF, $2, $4, $6, NULL);
																	  $2->m_functionRoot = $$; }
			| FUNCTION IDENTIFIER '(' ')' compound_stmnt { $$ = NewNode(NT_FUNCTIONDEF, $2, $5, NULL); }
			;

formalargs : IDENTIFIER { $$ = NewNode(NT_FORMALARGS, $1, NULL, NULL); }
			| formalargs ',' IDENTIFIER { $$ = NewNode(NT_FORMALARGS, $1, $3, NULL); }
			;

statement : expression ';' { $$ = NewNode(NT_STATEMENT, $1, NULL, NULL); }
		  | if_stmnt { $$ = NewNode(NT_STATEMENT, $1, NULL, NULL); }
		  | while_stmnt { $$ = NewNode(NT_STATEMENT, $1, NULL, NULL); }
		  | compound_stmnt { $$ = NewNode(NT_STATEMENT, $1, NULL, NULL); }
		  | ';' { $$ = NewNode(NT_STATEMENT, NULL, NULL, NULL); }
		  ;

compound_stmnt : '{' statement_list '}' { $$ = NewNode(NT_COMPOUND, $2, NULL, NULL); }
				| '{' '}' { $$ = NewNode(NT_COMPOUND, NULL, NULL, NULL); }
				;

statement_list : statement { $$ = NewNode(NT_STATEMENTLIST, $1, NULL, NULL); }
				| statement_list statement { $$ = NewNode(NT_STATEMENTLIST, $1, $2, NULL); }
				;

if_stmnt : IF '(' expression ')' statement { $$ = NewNode(NT_IF, $3, $5, NULL); }
			| IF '(' expression ')' statement ELSE statement { $$ = NewNode(NT_IF, $3, $5, $7, NULL); }
			;

while_stmnt : WHILE '(' expression ')' statement { $$ = NewNode(NT_WHILE, $3, $5, NULL); }
;

expression : NUMBER { $$ = $1; }
		   | IDENTIFIER { $$ = $1; }
		   | RETURN expression { $$ = NewNode(NT_RETURN, $2, NULL, NULL); }
		   | IDENTIFIER '=' expression { $$ = NewNode(NT_ASSIGNMENT, $1, $3, NULL); }
		   | expression '+' expression { $$ = NewNode(NT_ADDITION, $1, $3, NULL); }
		   | expression '-' expression { $$ = NewNode(NT_SUBTRACTION, $1, $3, NULL); }
		   | expression '*' expression { $$ = NewNode(NT_MULTIPLICATION, $1, $3, NULL); }
		   | expression '/' expression { $$ = NewNode(NT_DIVISION, $1, $3, NULL); }
		   | '+' expression %prec UMINUS { $$ = NewNode(NT_UNARYPLUS, $2, NULL, NULL); }
		   | '-' expression %prec UMINUS { $$ = NewNode(NT_UNARYMINUS, $2, NULL, NULL); }
		   | expression EQUALS expression { $$ = NewNode(NT_EQUALS, $1, $3, NULL); }
		   | expression NEQUAL expression { $$ = NewNode(NT_NEQUAL, $1, $3, NULL); }
		   | expression '<' expression { $$ = NewNode(NT_LESSTHAN, $1, $3, NULL); }
		   | expression '>' expression { $$ = NewNode(NT_GREATERTHAN, $1, $3, NULL); }
		   | expression LTE expression { $$ = NewNode(NT_LTE, $1, $3, NULL); }
		   | expression GTE expression { $$ = NewNode(NT_GTE, $1, $3, NULL); }
		   | expression AND expression { $$ = NewNode(NT_AND, $1, $3, NULL); }
		   | expression OR expression { $$ = NewNode(NT_OR, $1, $3, NULL); }
		   | '!' expression %prec NOT { $$ = NewNode(NT_NOT, $2, NULL, NULL); }
		   | '(' expression ')' { $$ = NewNode(NT_PARENTHESIS, $2, NULL, NULL); }
		   | IDENTIFIER '(' param_list ')' { $$ = NewNode(NT_FUNCTIONCALL, $1, $3, NULL); }
		   | IDENTIFIER '(' ')' { $$ = NewNode(NT_FUNCTIONCALL, $1, NULL, NULL); }
		   | EXP '(' expression ')' { $$ = NewNode(NT_EXP, $3, NULL, NULL); }
		   | SQRT '(' expression ')' { $$ = NewNode(NT_SQRT, $3, NULL, NULL); }
		   | POW '(' param_list ')' { $$ = NewNode(NT_POW, $3, NULL, NULL); }
		   ;

param_list : expression { $$ = NewNode(NT_PARAMLIST, $1, NULL, NULL); }
			| param_list ',' expression { $$ = NewNode(NT_PARAMLIST, $1, $3, NULL); }
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
