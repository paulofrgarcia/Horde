#ifndef LIN_AST_H
#define LIN_AST_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"ast.h"

#define LIN_AST_SEXP	0
#define LIN_AST_TERM	1

struct lin_ast_node
{
	//All nodes are named
	char *name;
	//S_exp or terminal
	int type;
	//if value (terminal)
	int constant;
	char *identifier;
	//If s_exp
	int operator;
	char *ls_exp; //pointer to left datum
	char *rs_exp; //pointer to right datum
	//set if IR already generated
	int IR_done;
	struct lin_ast_node *next;
};




char *lin_gen_last(struct ast_node *a);
char *lin_gen_name();

//Creates a new Lin AST entry
//If n is NULL, creates a new name
//returns n or created name
//If n already exists, returns without doing anything
char *lin_new_const(char *n, int c);
char *lin_new_ident(char *n, char *i);
char *lin_new_sexp(char *n, int o, char *left, char *right);

int lin_name_exists(char *n);

void lin_print_last(struct lin_ast_node *l);

#endif
