all:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
<<<<<<< HEAD

clean:
	rm -r bin
=======
>>>>>>> 0dc170623b824614bcb1cf81b8eae7458a0b5343
	
rshell:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
