#Project Rshell
CS100 Project

By: Michaella Sheng and David Zhang
#Summary
Rshell is a simple command shell run in a program. It implements the basic functionality 
	of a regular shell used in consoles. Rshell allows users to enter commands normally used 
	in shell along with added argument for applicable commands. Users can also chain these 
	commands together using conditional connectors, allowing them to run another command 
	if the first one would succeed or fail.

#Installation
Type this on your console:
		
	make
	
This will compile the existing code and create the Rshell executable in a new folder called bin. To run the program, type:
	
	bin/rshell.out
	
#Usage
Rshell is a project used to mimic shells like bash. Like the original shell, you can start entering commands to do specific tasks. When you first open the program you will be prompted immediately by the console. For example:

	student009@test-01$
From here,  you can start entering your commands

##Commands
You can enter bash commands onto the console. You are allowed to enter multiple commands (see connectors below) and enter flags for each command. For example:

	student009@test-01$ ls -a

This runs ls -a onto the console. The shell will catch mistakes with incorrect input as well, such as:

	student009@test-01$ pwddasa
	exec: No such file or directory.

##Connectors
To chain commands together, you can use connectors to execute certain commands under special conditions. 

In order to use a connector, you must enter it right after a command. You are allowed to enter it after or without a space.

Correct syntax:
	
	ls -a|| pwd
	pwd && ls -a
	
Incorrect syntax:

	ls-a||pwd
	mkdir||;pwd


There are three connectors: ||, &&, and ;. They function as follows:

###||

Will only run the second command if the first command fails. Example:

	mkdirhello || ls -a
	
'mkdirhello' will fail, but the '||' connector will allow 'ls -a' to run.

###&&

Will only run the second command if the first command succeeds. Example:

	pwd && exit
	
'pwd' will succeed, and with the '&&' connector 'exit' will be run afterwards.

###;

Runs the second command after the first command. Example:

	mv test.cpp bin ; exit
	
'mv test.cpp bin' will be run, followed by 'exit'

You are also allowed to chain as many connectors together as possible. Example:

	ls -a;rm hello.cpp;exit
	
'ls -a' will run, then 'rm hello.cpp' will follow, and finally 'exit' will be called in order.


	
#Known Bugs:
User enters wrong command followed by an || connector and followed by exit. 
The program does not exit the shell as it should. It reprompts for the next command instead.

Adding three or more of the same connector is supposed to throw an error on the entire command line, but it throws an out of range error.

If there are any other bugs you encounter while running this program, feel free to create an issue on Github.
We appreciate it!
