#include"ir.h"
#include"ast.h"
#include"lin_ast.h"
#include"symtable.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"parser.tab.h"

extern struct lin_ast_node *l_ast;

//List of all program data
//Will be used for interpretation and for compilation
struct ir_node *ir = NULL;
	
struct ir_node *ir_get_node(char *s)
{
	struct ir_node *i;

	i = ir;

	if(s == NULL)
	{
		//printf("Error: invalid name\n");
		return NULL;
	}
	while(i != NULL)
	{
		if(strcmp(i->name,s)==0)
			return i;
		i = i->next;
	}
	return NULL;
}

//Sets dpendency on source node, on proper side
void ir_set_dependency(struct ir_node * source,struct ir_node *dependency, int side)
{
	struct dependent_node *d;

	if(source == NULL)
		return;
	
	d = (struct dependent_node *)malloc(sizeof(struct dependent_node));

	d->d_node = dependency;
	d->side = side;

	d->next = source->depend_list;

	source->depend_list = d;
}


//Goes through the linear AST and generates the IR for every node
//Then sets all dependencies
void ir_gen_ir()
{
	//GEnerate IR per node
	struct lin_ast_node *a;
	struct ir_node *i;

	a = l_ast;

	while(a != NULL)
	{
		i = (struct ir_node *)malloc(sizeof(struct ir_node));

		i->name = a->name;
		//If constant
		if(a->type == LIN_AST_TERM && a->identifier == NULL)
		{
			i->state = ALIVE;
			i->l_value = a->constant;
			i->operation = SOURCE;
		}
		else
		{
			//Unary operators start as "NEAR_ALIVE"
			if(i->operation == LIFE || i->operation == SOURCE)
				i->state = NEAR_ALIVE;
			else
				i->state = UNDEAD;
		}
		i->operation = a->operator;

		//No dependencies yet
		i->depend_list = NULL;

		i->next = ir;
		ir = i;

		a->IR_done = 1;
		a = a->next;
	}

	

	//Map dependencies
	a = l_ast;

	while(a != NULL)
	{
		if(!(a->type == LIN_AST_TERM && a->identifier == NULL))
		{
			ir_set_dependency(ir_get_node(a->ls_exp),ir_get_node(a->name),D_LEFT);
			ir_set_dependency(ir_get_node(a->rs_exp),ir_get_node(a->name),D_RIGHT);
		}
		a = a->next;
	}
}






