#ifndef AST_H
#define AST_H

#define AST_DATUM 0
#define AST_SEXP 1
#define AST_IDENT 2
#define AST_CONST 3

struct ast_node
{
	int type; /*0 datum, 1 S_Expression, 2 (S_exp) identifier, 3 (S_exp) constant*/
	//0 Datum
	char *name; //if datum, name
	struct ast_node *s_exp; //pointer to defining S_expression
	struct ast_node *n_datum; //pointer to next datum definition in program
	//1 S_Epression
	int operator;
	struct ast_node *ls_exp; //pointer to left S_expression
	struct ast_node *rs_exp; //pointer to right S_expression
	//2 identifier
	char *identifier;
	//3 constant
	int constant;
};

//AST creation during parsing
struct ast_node *new_ast_datum(char *n, struct ast_node *a, struct ast_node *next);
struct ast_node *new_ast_sexp(int o, struct ast_node *l, struct ast_node *r);
struct ast_node *new_ast_ident(char *i);
struct ast_node *new_ast_cons(int c);

struct ast_node *ast;

//AST correctness checking
int ast_check_valid_identifiers(struct ast_node *a);
int ast_typecheck();

//Displays the AST (debug purposes)
void ast_print_ast(struct ast_node *a, int cnt);


#endif
