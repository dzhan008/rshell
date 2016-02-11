all:
	g++ shell.cpp main.cpp -o executable
	
clean:
	rm executable