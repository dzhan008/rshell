all:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell.o
	
rshell:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell.o
