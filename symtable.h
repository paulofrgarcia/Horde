#ifndef SYMTABLE_H
#define SYMTABLE_H

#include"ast.h"
#include"ir.h"

struct sym_node
{
	char *name;
	//Pointer to AST entry
	struct ast_node *ast_entry;
	//Pointer to IR entry
	struct ir_node *ir_entry;
	struct sym_node *next;
};



void sym_add_sym(char *s, struct ast_node *a);
struct ast_node *sym_get_ast(char *s);

int sym_valid_datum(char *i);

//Adds pointer to IR entry i of symbol named s
void sym_add_ir(char *s, struct ir_node *i);

//Returns pointer to IR entry of symbol s
struct ir_node *sym_get_ir(char *s);

#endif
