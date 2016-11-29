%{
#include <stdio.h>
#include <stdlib.h>
#include "ptree.h"
#include "flex2.tab.h"

%}

%option noyywrap
%%

";" { return ';'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"=" { return '='; }

0|[1-9][0-9]* { yylval.pt = NewNode(NT_NUMBER,NULL,NULL,yytext);
				return NUMBER; }

[a-zA-Z][a-zA-Z0-9_]* {	
							yylval.pt = lookup(yytext, false);
						return IDENTIFIER; }

%%


