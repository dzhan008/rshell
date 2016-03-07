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
int isFile(const char*);
int isDirectory(const char*);
int exists(const char*);
bool check(const vector<string>&);
void parseFilePath(vector<string>&);
int runTestCommand(vector<string>&);

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

    for(tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
    {
        commandLine.push_back(*it); 
    }
    if(check(commandLine) == true)
    {
        runTestCommand(commandLine);
    }
    else
    {
        return;
    }
}

//1 if type, 0 if not
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
// 0 if exists, -1 if it does not
int exists(const char* path)
{
    struct stat path_stat;
    return stat(path, &path_stat);
}
bool check(const vector<string>& cmdLine)
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
        << cmdLine.at(cmdLine.size()-1) << endl;
        return false;
    }
    //Check if proper flag was called. Note that other flags exist but for this
    //program user is limited to only 3 flags
    if(cmdLine.at(1) != "-e" && cmdLine.at(1) != "-f" && cmdLine.at(1) != "-d")
    {
        if(cmdLine.size() != 2 && cmdLine.size() != 3)
        {
            cout << "-bash: " << cmdLine.at(0) << ": " << cmdLine.at(1) 
            << ": unary operator expected\n";
            return false;
        }
    }
    return true;
}

//Seperates command line into a string with just the file path
void parseFilePath(vector<string>& cmdLine)
{
    string filePath;
    if(cmdLine.at(1) == "-e" || cmdLine.at(1) == "-f" || cmdLine.at(1) == "-d")
    {
        filePath = cmdLine.at(2);
    }
    else //If no flag was specified
    {
        filePath = cmdLine.at(1);
    }
    cmdLine.clear();
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("/");
    tokenizer tok(filePath, sep);
    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
    {
        cmdLine.push_back(*it); 
    }
}

int runTestCommand(vector<string>& cmdLine) 
{
    if(cmdLine.at(1) == "-f")
    {
        parseFilePath(cmdLine);
        //Saves the smaller paths. Note: Will only work with command lines with 
        //no more than 3 commands
        string path1; string path2;
        if(cmdLine.size() == 2)
        {
             path1 = cmdLine.at(0) + "/" + cmdLine.at(1);
        }
        if(cmdLine.size() == 3)
        {
            path2 = cmdLine.at(0) + "/" + cmdLine.at(1) + "/" + cmdLine.at(2);
        }
        char temp1[path1.size()+1]; char temp2[path2.size()+1];
        //Checks every single file within the path to see if it exists
        for(unsigned i = 0; i < cmdLine.size(); ++i)
        {
            char temp[cmdLine.at(i).size()+1];
            for(unsigned j = 0; j < cmdLine.at(i).size(); ++j)
            {
                temp[j] = cmdLine.at(i).at(j);
            }
            temp[cmdLine.at(i).size()] = '\0';
            //If file does not exist, check for current combined 
            //file path existence
            if(exists(temp) == -1)
            {
                //If the file is the first in the path, return false
                if(i == 0)
                {
                    cout << "(False)\n";
                    return 1;
                }
                //Otherwise check the combined path
                else if(i == 1)
                {
                    for(unsigned j = 0; j < path1.size(); ++j)
                    {
                        temp1[j] = path1.at(j);
                    }
                    temp1[path1.size()] = '\0';
                    if(exists(temp1) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
                else //i ==2
                {
                    for(unsigned j = 0; j < path2.size(); ++j)
                    {
                        temp2[j] = path2.at(j);
                    }
                    temp2[path2.size()] = '\0';
                    if(exists(temp2) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
            }
            //If the first file in a path is regular, path is invalid. 
            if(isFile(temp) == 1 && i == 0)
            {
                cout << "(False)\n";
                return 1;
            }
        }
        //At this point we know the file path exists. Now we will do a final
        //check for the file type on the last item in cmdLine
        if(cmdLine.size() == 1)
        {
            char temp[cmdLine.at(cmdLine.size()-1).size()+1];
            for(unsigned i = 0; i < cmdLine.at(cmdLine.size()-1).size(); ++i)
            {
                temp[i] = cmdLine.at(cmdLine.size()-1).at(i);
            }
            temp[cmdLine.at(cmdLine.size()-1).size()] = '\0';
        
            if(isFile(temp) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
        else if (cmdLine.size() == 2)
        {
            if(isFile(temp1) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
        else
        {
            if(isFile(temp2) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
    }
    else if(cmdLine.at(1) == "-d")
    {
        parseFilePath(cmdLine);
        //Saves the smaller paths. Note: Will only work with command lines with 
        //no more than 3 commands
        string path1; string path2;
        if(cmdLine.size() == 2)
        {
             path1 = cmdLine.at(0) + "/" + cmdLine.at(1);
        }
        if(cmdLine.size() == 3)
        {
            path2 = cmdLine.at(0) + "/" + cmdLine.at(1) + "/" + cmdLine.at(2);
        }
        char temp1[path1.size()+1]; char temp2[path2.size()+1];
        for(unsigned i = 0; i < cmdLine.size(); ++i)
        {
            char temp[cmdLine.at(i).size()+1];
            for(unsigned j = 0; j < cmdLine.at(i).size(); ++j)
            {
                temp[j] = cmdLine.at(i).at(j);
            }
            temp[cmdLine.at(i).size()] = '\0';
            //If file does not exist, exit function and return false
            if(exists(temp) == -1)
            {
                //If the file is the first in the path, return false
                if(i == 0)
                {
                    cout << "(False)\n";
                    return 1;
                }
                //Otherwise check the combined path
                else if(i == 1)
                {
                    for(unsigned j = 0; j < path1.size(); ++j)
                    {
                        temp1[j] = path1.at(j);
                    }
                    temp1[path1.size()] = '\0';
                    if(exists(temp1) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
                else //i ==2
                {
                    for(unsigned j = 0; j < path2.size(); ++j)
                    {
                        temp2[j] = path2.at(j);
                    }
                    temp2[path2.size()] = '\0';
                    if(exists(temp2) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
            }
            //If the first file in a path is regular, path is invalid. 
            if(isFile(temp) == 1 && i == 0)
            {
                cout << "(False)\n";
                return 1;
            }
        }
        //Does a final check for the file type on the last item in the file
        if(cmdLine.size() == 1)
        {
            char temp[cmdLine.at(cmdLine.size()-1).size()+1];
            for(unsigned i = 0; i < cmdLine.at(cmdLine.size()-1).size(); ++i)
            {
                temp[i] = cmdLine.at(cmdLine.size()-1).at(i);
            }
            temp[cmdLine.at(cmdLine.size()-1).size()] = '\0';
        
            if(isDirectory(temp) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
        else if (cmdLine.size() == 2)
        {
            if(isDirectory(temp1) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
        else
        {
            if(isDirectory(temp2) == 1)
            {
                cout << "(True)\n";
                return 0;
            }
            else
            {
                cout << "(False)\n";
                return 1;
            }
        }
    }
    else if(cmdLine.at(1) == "-e")
    {
        parseFilePath(cmdLine);
        string path1; string path2;
        if(cmdLine.size() == 2)
        {
             path1 = cmdLine.at(0) + "/" + cmdLine.at(1);
        }
        if(cmdLine.size() == 3)
        {
            path2 = cmdLine.at(0) + "/" + cmdLine.at(1) + "/" + cmdLine.at(2);
        }
        char temp1[path1.size()+1]; char temp2[path2.size()+1];
        for(unsigned i = 0; i < cmdLine.size(); ++i)
        {
            char temp[cmdLine.at(i).size()+1];
            for(unsigned j = 0; j < cmdLine.at(i).size(); ++j)
            {
                temp[j] = cmdLine.at(i).at(j);
            }
            temp[cmdLine.at(i).size()] = '\0';
            //If file does not exist, exit function and return false
            if(exists(temp) == -1)
            {
                //If the file is the first in the path, return false
                if(i == 0)
                {
                    cout << "(False)\n";
                    return 1;
                }
                //Otherwise check the combined path
                else if(i == 1)
                {
                    for(unsigned j = 0; j < path1.size(); ++j)
                    {
                        temp1[j] = path1.at(j);
                    }
                    temp1[path1.size()] = '\0';
                    if(exists(temp1) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
                else //i ==2
                {
                    for(unsigned j = 0; j < path2.size(); ++j)
                    {
                        temp2[j] = path2.at(j);
                    }
                    temp2[path2.size()] = '\0';
                    if(exists(temp2) == -1)
                    {
                        cout << "(False)\n";
                        return 1;
                    }
                }
            }
            //If the first file in a path is regular, path is invalid. 
            if(isFile(temp) == 1 && i == 0)
            {
                cout << "(False)\n";
                return 1;
            }
        }
        cout << "(True)\n";
        return 0;
    }
}

int main()
{
    parseCommand();
    return 0;
}