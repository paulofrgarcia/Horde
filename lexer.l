%{
 
/*
 * Lexer.l file
 * To generate the lexical analyzer run: "flex Lexer.l"
 */
 

#include "parser.tab.h"

#include <stdio.h>
 
%}


 
NUMBER      [0-9]+
IDENT	    [a-z|A-Z][a-z|A-Z|0-9|_]*
WS          [ \r\n\t]*
COMMENT	    "//".*
 
%%
 
{COMMENT}	{ /* Skip comment. */}
{WS}            { /* Skip blanks. */ }
{NUMBER}        { yylval.value = atoi(yytext); return CONSTANT; }
"+"		{return ADD;}
"-"		{return SUB;}
"*"		{return MUL;}
"/"		{return DIV;}
"%" 		{return MOD;}
"==" 		{return EQUALS;}
"!=" 		{return NOTEQUAL;}
">="		{return GREATEREQ;}
"<=" 		{return LESSEQ;}
">" 		{return GREATER;} 
"<"		{return LESS;} 
"if"		{return IF;}
"life" 		{return LIFE;}
"zombie" 	{return ZOMBIE;}
{IDENT}		{ yylval.name = strdup((const char *)yytext); return IDENTIFIER; }
.               { return yytext[0]; }
 
%%
 
int yywrap(){return 1;}

