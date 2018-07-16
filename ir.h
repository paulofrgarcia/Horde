#ifndef IR_H
#define IR_H

#include"ast.h"

//state definitions
#define UNDEAD		3
#define NEAR_ALIVE 	2		
#define ALIVE		1
#define DEAD		0


#define D_LEFT	0
#define D_RIGHT	1


struct ir_node;

//Used to implement a list of ir_nodes that depend on us (this datum)
//Will be implemented as static memory in compilation
struct dependent_node
{
	struct ir_node *d_node;
	int side; //left or right
	struct dependent_node *next;
};

struct ir_node
{
	char *name; //can be NULL for unnamed data
	unsigned int state; //alive, dead, undead
	int operation; //atomic operation to perform
	//left value also used as resolved value, e.g., constant or result of calculation
	int l_value; //left and right values 
	int r_value;
	//pointers to all undead datums that depend on us
	struct dependent_node *depend_list; //List of dependent nodes
	//Pointer to next IR node
	struct ir_node *next;
};


//Generates the partial IR from the AST
//struct ir_node *ir_gen_partial_ir(struct ast_node *a);

//Generates the first stage IR from the AST
//struct ir_node *ir_gen_first_stage(struct ast_node *a);

//Fixes all dependency pointers in all data
//void ir_fix_dependencies(struct ir_node *i);

//Adds dependency dep to entry source
//void ir_add_dependency(struct ir_node *source,struct ir_node *dep);

void ir_gen_ir();

struct ir_node *ir_get_node(char *s);

void ir_set_dependency(struct ir_node * source,struct ir_node *dependency, int side);

#endif
