all:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
<<<<<<< HEAD
<<<<<<< HEAD

clean:
	rm -r bin
=======
>>>>>>> 0dc170623b824614bcb1cf81b8eae7458a0b5343
=======
=======

clean:
	rm -r bin
>>>>>>> 74205051fe5ae7ed58b94604411f0cfed8e305e2
>>>>>>> 1773b8794a04ab3e1577931c214f8fb37a319857
	
rshell:
	mkdir bin
	g++ src/shell.cpp src/main.cpp -Wall -Werror -ansi -pedantic -o bin/rshell
