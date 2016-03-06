#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace boost;
using namespace std;

void parseCommand(); 
int isFile(const char *path);
int isDirectory(const char *path);
bool check(const vector<string> cmdLine);
int test(const vector<string> cmdLine);
bool execute(vector<string> cmdLine);

//Splits command line into seperate strings based on spaces
void parseCommand()
{
    string command;
    vector<string> commandLine;
    
    cout << "Enter a command.\n";
    getline(cin, command);
    
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep(" ");
    tokenizer tok(command, sep);

    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
    {
        commandLine.push_back(*it); 
    }
    if(check(commandLine) == true)
    {
        test(commandLine);
    }
    else
    {
        return;
    }
}

//0 is returned on success, -1 for error
int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
int isDirectory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool check(const vector<string> cmdLine)
{
    //Checks if proper test call was made
    if(cmdLine.at(0) != "test" && cmdLine.at(0) != "[")
    {
        cout << "-bash: " << cmdLine.at(0) << ": command not found\n";
        return false;
    }
    //Checks if test command was closed correctly using bracket notation
    if(cmdLine.at(0) == "[" && cmdLine.at(cmdLine.size()-1) != "]")
    {
        cout << "-bash: syntax error near unexpected token '" 
        << cmdLine.at(cmdLine.size()-1) << "'\n";
        return false;
    }
    //Check if proper flag was called. Note that other flags exist but for this
    //program user is limited to only 3 flags
    // if(cmdLine.at(1) != "-e" && cmdLine.at(1) != "-f" && cmdLine.at(1) != "-d")
    // {
    //     cout << "-bash: " << cmdLine.at(0) << ": " << cmdLine.at(1) 
    //     << ": unary operator expected\n";
    //     return false;
    // }
    return true;
}

int test(const vector<string> cmdLine) 
{
    if(cmdLine.at(1) == "-f")
    {
        //Execute file search and test if regular file
        cout << "(True)\n";
        return 0;
        cout << "(False)\n";
        return 1;
    }
    else if(cmdLine.at(1) == "-d")
    {
        //Execute file search and test if it is a directory
        cout << "(True)\n";
        return 0;
        cout << "(False)\n";
        return 1;
    }
    else if(cmdLine.at(1) == "-e")
    {
        //Execute file search
        cout << "(True)\n";
        return 0;
        cout << "(False)\n";
        return 1;
    }
}

bool execute(vector<string> cmdLine)
{
    char* temp[cmdLine.size()+ 1];
    //Sets the command in a c-string form.
    for(int i = 0; i < cmdLine.size(); ++i)
    {
        temp[i] = (char*)cmdLine.at(i).c_str();
    }
    temp[cmdLine.size()] = NULL;
    
    pid_t pid = fork();
    if (pid == 0)
    {
        if(execvp(temp[0], temp) == -1)
        {
            perror("exec");
            return false;
        }
    }
    else
    {
        while (wait(0) == -1)
        {
            perror("wait");
        }
    }
    return true;
}

int main()
{
    parseCommand();
    return 0;
}
