CC = g++
CFLAGS = -g -O0 -std=c++11

compile: miniL.lex miniL.y
	bison -v -d --file-prefix=y miniL.y
	flex miniL.lex
	g++ -o miniL y.tab.c lex.yy.c -lfl

clean:
	rm -f lex.yy.c y.tab.* y.output *.o miniL miniL-lex.c miniL-parser.c miniL-parser.c miniL-parser.dot miniL-parser.h miniL-parser.output out.mil