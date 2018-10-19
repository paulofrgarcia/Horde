%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
#include<stdio.h>
#include "ast.h"
#include "symtable.h"

int yylex (void);

int yyerror() {
    printf("Syntax error\n");
}
 
%}

%union {
	char *name;
	int value;
	struct ast_node *node;
}
 

%token  ADD SUB MUL DIV MOD EQUALS NOTEQUAL GREATER LESS GREATEREQ LESSEQ IF LIFE ZOMBIE SOURCE
%token <name> IDENTIFIER
%token <value> CONSTANT

%type <value> operator 
%type <node> program s_exp

%%

program
	: IDENTIFIER '=' s_exp ';' 		{ast = $$ = new_ast_datum($1, $3, NULL); sym_add_sym($1,ast);}
	| program IDENTIFIER '=' s_exp ';' 	{ast = $$ = new_ast_datum($2, $4, $1); sym_add_sym($2,ast);} ;
 

s_exp
	: '(' IDENTIFIER ')' 		{$$ = new_ast_ident($2);}
	| '(' CONSTANT ')'		{$$ = new_ast_cons($2);}
	| '(' operator s_exp s_exp ')' 	{$$ = new_ast_sexp($2, $3, $4);} ;

operator
	: ADD 		{$$=ADD;}
	| SUB 		{$$=SUB;}
	| MUL 		{$$=MUL;}
	| DIV	 	{$$=DIV;}
	| MOD 		{$$=MOD;}
	| EQUALS 	{$$=EQUALS;}
	| NOTEQUAL	{$$=NOTEQUAL;}
	| GREATER 	{$$=GREATER;}
	| LESS 		{$$=LESS;}
	| GREATEREQ 	{$$=GREATEREQ;}
	| LESSEQ 	{$$=LESSEQ;}
	| IF 		{$$=IF;}
	| LIFE 		{$$=LIFE;} 
	| ZOMBIE 	{$$=ZOMBIE;} ;


 
%%
