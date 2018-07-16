FILES	= lex.yy.c parser.tab.c main.c ast.c symtable.c ir.c lin_ast.c runtime.c
CC	= gcc
CFLAGS	= -g 

hlc:		$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o hlc

lex.yy.c:	lexer.l parser.tab.c
		flex lexer.l

parser.tab.c:	parser.y 
		bison -d parser.y

clean:
		rm -f *.o *~ lex.yy.c parser.tab.c parser.tab.h hlc
