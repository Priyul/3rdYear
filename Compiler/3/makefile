CC=g++
CFLAGS=-c -Wall

all: main

main: lexer.o token.o parser.o node.o ast_node.o main.o
	$(CC) lexer.o token.o parser.o node.o ast_node.o main.o -o main

lexer.o: lexer.cpp
	$(CC) $(CFLAGS) lexer.cpp

token.o: token.cpp
	$(CC) $(CFLAGS) token.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp

node.o: node.cpp
	$(CC) $(CFLAGS) node.cpp

ast_node.o: ast_node.cpp
	$(CC) $(CFLAGS) ast_node.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

run:
	./main

clean:
	rm -rf *o main
