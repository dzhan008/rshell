all:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
<<<<<<< HEAD
=======

clean:
	rm -r bin
>>>>>>> 74205051fe5ae7ed58b94604411f0cfed8e305e2
	
rshell:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
