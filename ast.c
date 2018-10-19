#include"ast.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symtable.h"


struct ast_node *new_ast_datum(char *n, struct ast_node *a, struct ast_node *next)
{
	struct ast_node *node;
	node = (struct ast_node *)malloc(sizeof(struct ast_node));
	node->name = n;
	node->type = AST_DATUM;
	node->s_exp = a;
	node->n_datum = next;
	//blank
	node->ls_exp = node->rs_exp = NULL;
	node->identifier = NULL;

	return node;
}
struct ast_node *new_ast_sexp(int o, struct ast_node *l, struct ast_node *r)
{
	struct ast_node *node;
	node = (struct ast_node *)malloc(sizeof(struct ast_node));
	node->type = AST_SEXP;
	node->operator = o;
	node->ls_exp = l;
	node->rs_exp = r;
	//blank
	node->s_exp = NULL;
	node->identifier = NULL;
	node->n_datum = NULL;	

	return node;
}

struct ast_node *new_ast_ident(char *i)
{
	struct ast_node *node;
	node = (struct ast_node *)malloc(sizeof(struct ast_node));
	node->type = AST_IDENT;
	node->identifier=i;
	//blank
	node->s_exp = NULL;
	node->ls_exp = node->rs_exp = NULL;
	node->n_datum = NULL;

	return node;
}
struct ast_node *new_ast_cons(int c)
{
	struct ast_node *node;
	node = (struct ast_node *)malloc(sizeof(struct ast_node));
	node->type = AST_CONST;
	node->constant = c;
	//blank
	node->s_exp = NULL;
	node->ls_exp = node->rs_exp = NULL;
	node->n_datum = NULL;

	return node;
}

//AST correctness checking
int ast_check_valid_identifiers(struct ast_node *a)
{
	switch (a->type)
	{
	case AST_DATUM:
	{
		if(ast_check_valid_identifiers(a->s_exp))
			return 1;
		if(a->n_datum != NULL)
			return ast_check_valid_identifiers(a->n_datum);
		else
			return 0;
	} break;
	case AST_SEXP:
	{
		if(ast_check_valid_identifiers(a->ls_exp))
			return 1;
		if(ast_check_valid_identifiers(a->rs_exp))
			return 1;
	} break;
	case AST_IDENT:
	{
		return sym_valid_datum(a->identifier);
	} break;
	case AST_CONST:
	{
		return 0;
	} break;
	default:
		printf("Unknown AST node type.\n");
		return 1;
	}
	
	return 0;
}

int ast_typecheck()
{
	//check that all used identifiers are valid data
	if(ast_check_valid_identifiers(ast))
	{
		return 1;
	}
	//Here, AST is correct except for zombie (referring to same identifier)
	//TODO
	return 0;
}

//Displays the AST (debug purposes)
void ast_print_ast(struct ast_node *a, int cnt)
{
	int i;
	if(a == NULL)
	return;

	for(i=0;i<cnt;i++)
	{
		printf("\t");
	}
	
	switch(a->type)
	{
	case(AST_DATUM):
	{
		printf("DATUM %s\n",a->name);
		ast_print_ast(a->s_exp, cnt+1);
		printf("\n");
		ast_print_ast(a->n_datum, cnt);
	} break;
	case(AST_IDENT):
	{
		printf("IDENT %s\n",a->identifier);
	} break;
	case(AST_CONST):
	{
		printf("CONST %d\n",a->constant);
	} break;
	case(AST_SEXP):
	{
		printf("SEXP\n");
		ast_print_ast(a->ls_exp, cnt+1);
		ast_print_ast(a->rs_exp, cnt+1);
	} break;
	}
}








