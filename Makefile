all:
	mkdir bin
	g++ shell.cpp main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell.o
	
rshell:
	mkdir bin
	g++ shell.cpp main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell.o
