%{
#include <stdio.h>
#include <stdlib.h>
#include "ptree.h"
#include "flex2.tab.h"
%}

%option noyywrap
%%

";" {return Q_MARK;}
"+" {return PLUS;}
0|[1-9][0-9]* {yylval.pt = new_Node(NT_NUMBER, NULL, NULL, yytext);
				return NUMBER;}

%%