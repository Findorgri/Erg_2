%{
#include <stdio.h>
#include <stdlib.h>
#include "ptree.h"
#include "lab3demo2.tab.h"

%}

%option noyywrap
%%

";" { return ';'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"=" { return '='; }
"==" {return EQUALS; }
"<" {return '<';}
">" {return '>';}
"<=" {return LTE;}
">=" {return GTE;}
"!=" {return NEQUAL;}
"&&" {return AND;}
"||" {return OR;}
"function" {return FUNCTION;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;}
"return" {return RETURN;}
"!" {return '!';}
"," {return ',';}
"(" {return '(';}
")" {return ')';}
"{" {return '{'; }
"}" {return '}'; }
"pow" {return POW;}
"exp" {return EXP;}
"sqrt" {return SQRT;}

0|[1-9][0-9]* { yylval.pt = NewNode(NT_NUMBER,NULL,NULL,yytext);
				return NUMBER; }

[a-zA-Z][a-zA-Z0-9_]* {	
							yylval.pt = lookup(yytext, false);
						return IDENTIFIER; }

[ \t\r\n]

%%


