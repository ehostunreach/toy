all: scanner

scanner: parser lexer
	gcc -Wall -o scanner lex.yy.c parser.tab.c -lfl

lexer: lexer.l
	flex lexer.l

parser: parser.y
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner
