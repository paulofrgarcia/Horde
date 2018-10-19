#include"lin_ast.h"
#include"parser.tab.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct lin_ast_node *l_ast = NULL;

void lin_print_last(struct lin_ast_node *l)
{
	if(l != NULL)
	{
		printf("%d\t",l->operator);
		if(l->type == LIN_AST_TERM)
		{
			printf("%s",l->name);
			if(l->identifier)
				printf("\t%s\n",l->identifier);
			else
				printf("\t%d\n",l->constant);
		}
		else
		{
			printf("%s\t%s %s\n",l->name,l->ls_exp,l->rs_exp);
		}
		lin_print_last(l->next);
	}
}

char *lin_gen_name()
{
	static int cnt = 0;
	char tmp[256];
	char *result;

	sprintf(tmp,"orc%d",cnt++);
	result=(char *)malloc(strlen(tmp)+1);
	strcpy(result,tmp);
	return result;
}

int lin_name_exists(char *n)
{
	if(n == NULL)
		return 0;

	struct lin_ast_node *l = l_ast;

	while(l != NULL)
	{
		if(strcmp(l->name,n)==0)
			return 1;
		l = l->next;
	}	

	return 0;
}

//Creates a new Lin AST entry
//If n is NULL, creates a new name
//returns n or created name
//If n already exists, returns without doing anything
char *lin_new_const(char *n, int c)
{
	if(n == NULL)
		n = lin_gen_name();

	//check for existence
	if(lin_name_exists(n))
		return n;

	//Add here
	struct lin_ast_node *l;
	l = (struct lin_ast_node *)malloc(sizeof(struct lin_ast_node));

	l->name = n;
	l->type = LIN_AST_TERM;

	l->constant = c;
	l->identifier = NULL;

	l->operator = SOURCE;

	l->IR_done = 0;

	l->next = l_ast;

	l_ast = l;

	return n;
}

//Creates a new Lin AST entry
//If n is NULL, creates a new name
//returns n or created name
//If n already exists, returns without doing anything
char *lin_new_ident(char *n, char *i)
{
	if(n == NULL)
		n = lin_gen_name();

	//check for existence
	if(lin_name_exists(n))
		return n;

	//Add here
	struct lin_ast_node *l;
	l = (struct lin_ast_node *)malloc(sizeof(struct lin_ast_node));

	l->name = n;
	l->type = LIN_AST_TERM;

	l->constant = 0;
	l->identifier = i;

	l->operator = SOURCE;

	l->IR_done = 0;

	l->next = l_ast;

	l_ast = l;

	return n;
}


//Creates a new Lin AST entry
//If n is NULL, creates a new name
//returns n or created name
//If n already exists, returns without doing anything
char *lin_new_sexp(char *n, int o, char *left, char *right)
{
	if(n == NULL)
		n = lin_gen_name();

	//check for existence
	if(lin_name_exists(n))
		return n;

	//Add here
	struct lin_ast_node *l;
	l = (struct lin_ast_node *)malloc(sizeof(struct lin_ast_node));

	l->name = n;
	l->type = LIN_AST_SEXP;

	l->constant = 0;
	l->identifier = NULL;

	l->ls_exp = left;
	l->rs_exp = right;

	l->operator = o;

	l->IR_done = 0;

	l->next = l_ast;

	l_ast = l;

	return n;
}

//We want the linearized AST to be a sequence of data
//Each is named (names are created for anonymous S_expressions), and points to 2 other data (plus operation) or to constant/identifier

char *lin_gen_last(struct ast_node *a)
{
	struct ir_node *result = NULL;
	if(a != NULL)
	{	
		switch(a->type)
		{
		case AST_DATUM:
		{
			//Is it a constant?
			if(a->s_exp->type == AST_CONST)
			{
				lin_new_const(a->name, a->s_exp->constant);
				
				//move on to next datum
				lin_gen_last(a->n_datum);
			}
			//Not a constant, S expression or identifier
			else
			{
				//Just depends of another datum?
				if(a->s_exp->type == AST_IDENT)
				{
					lin_new_ident(a->name, a->s_exp->identifier);
					
					lin_gen_last(a->n_datum);
				}
				//Full S_Expression
				else
				{
					char *lname = lin_gen_last(a->s_exp->ls_exp);
					char *rname = lin_gen_last(a->s_exp->rs_exp);
					
					lin_new_sexp(a->name, a->s_exp->operator, lname, rname);
					
					lin_gen_last(a->n_datum);
				}
			}
		} break;
		case AST_SEXP:
		{
			char *lname = lin_gen_last(a->ls_exp);
			char *rname = lin_gen_last(a->rs_exp);
			return lin_new_sexp(NULL, a->operator, lname, rname);
		} break;
		case AST_IDENT:
		{
			return a->identifier; // lin_new_ident(NULL, a->identifier);
		} break;
		case AST_CONST:
		{
			return lin_new_const(NULL, a->constant);
		} break;
		default:
		{
			printf("Error: unknown AST type.\n");
			return NULL;
		}		
		}		
	}
	return NULL;
}
