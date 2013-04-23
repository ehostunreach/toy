all: scanner

scanner: parser lexer
	clang -Wall -o scanner ast.c print.c					\
					   symtab.c vm_state.c vm_value.c 	   \
						driver.c										\
						lex.yy.c parser.tab.c					\
					-lfl -lstdc++									\
					`pkg-config --cflags --libs glib-2.0`	\
					`llvm-config --cflags --ldflags --libs core analysis`

lexer: lexer.l
	flex lexer.l

parser: parser.y
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner
