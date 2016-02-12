#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <pwd.h>

using namespace std;
using namespace boost;

class Shell {
    private:
        string command;
        bool exit_override;
        //Step 1: Ask for command
        //Step 2: Parse command
        //Step 3: Execute command
        //Step 4: Account for connectors
    public:
        Shell() {exit_override = false;};
        ~Shell() {};
        bool run() 
        {
            char* name;
            char hostname[1024];
            gethostname(hostname, 1024);
            struct passwd* pass;
            pass = getpwuid(getuid());
            name = pass->pw_name;
            exit_override = false;   
            while(command != "exit" || !exit_override) 
            {
                cout << "exit_override:" << exit_override << endl;
                if(exit_override)
                {
                    cout << "Attempting to exit..." << endl;
                    return false;
                }
                else
                {
                cout << name << "@" << hostname << "$ ";
                getline(cin, command);
                if(command == "exit") //If the command is just exit
                {
                    return false;  
                }
                else if(command != "") //If we entered an actual command
                {
                    if(parse(command) == 2)
                    {
                        cout << "setting override..." << endl;
                        exit_override = true;
                    }
                }
                else
                {
                    command = "pwd";
                    parse(command);
                }
                }
            }
        }
        int parse(string cmdLine) 
        {
            vector <string> v;
            //Defines a tokenizer type that seperates cmdLine by spaces
            typedef tokenizer<char_separator<char> > tokenizer;
            char_separator<char> sep(" ");
            tokenizer tok(cmdLine, sep);
            
            
            //Checks each token and puts it in a vector.
            for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
            {
                    v.push_back(*it); 
            }
            v = analyze_split(v);
                            
            vector<string> command_to_execute;
            for (int i = 0; i < v.size(); ++i)
            {
                //If comment is seen, we don't push it to our command.
                //Instead, we execute it and jump out of the loop.
                if(v.at(i).at(0) == '#')
                {
                    bool temp = execute(command_to_execute);
                    return 1;
                }
                else if (i == v.size() - 1)
                {
                    if (v.at(i) == "exit")
                    {
                        cout << "Oh hey an exit!" << endl;
                        return 2;
                    }
                    command_to_execute.push_back(v.at(i));
                    bool temp = execute(command_to_execute);
                    if (temp == false)
                    {
                        return 2; //Return code for exit
                    }
                }
                else if (v.at(i) == "||" || v.at(i) == "&&" || v.at(i) == ";")
                {
                    bool temp = execute(command_to_execute);
                    command_to_execute.clear();
                    temp = connector(temp, v.at(i));
                    if (temp == false)
                    {
                        if(v.at(i-1) == "exit")
                        {
                            return 2;
                        }
                        return 1; //Return code for failure
                    }
                }
                else
                {
                    command_to_execute.push_back(v.at(i));
                }
                cout << v.at(i) << endl;
            }
            return 0; //Return code for command sucessfully parsed
        }
        /*Potential Problem: This algorithm will keep repeating connectors,
        ie. ls&&&&ls
        And since we split the two pairs of && into two elements, it'll attempt
        to call the connector twice. I suppose we can check for the when we
        start executing the command.*/
        
        //Accounts for connectors mixed with other text
        vector<string> analyze_split(vector<string>& v)
        {
            vector<string> commands;
            //Look at each string in the vector.

            for(int i = 0; i < v.size(); ++i)
            {
                string temp;
                //Look at each letter of each string for connectors.
                for(int j = 0; j < v.at(i).size(); ++j)
                {
                    //Tests whether or not we found a connector.
                    if(v.at(i).at(j) == '|' && v.at(i).at(j + 1) == '|')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }

                        temp = "||";
                        commands.push_back(temp);
                        temp = "";
                        ++j;
                    }
                    else if(v.at(i).at(j) == '&' && v.at(i).at(j + 1) == '&')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }
                        
                        temp = "&&";
                        commands.push_back(temp);
                        temp = "";
                        ++j;
                    }
                    else if(v.at(i).at(j) == ';')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }
                        
                        temp = ";";
                        commands.push_back(temp);
                        temp = "";
                    }
                    else
                    {
                        temp = temp + v.at(i).at(j);
                    }
                }
                if(temp != "")
                {
                    commands.push_back(temp);
                }

            }
            //Push the string in case there are no connectors.
            //commands.push_back(temp);
            //Input should now be organized correctly by parts.
            return commands;
        }
        
        //Executes the command.
        bool execute(vector<string>cmd)
        {
                //Accounts for case if a comment is inputted.
                if (cmd.size() == 0)
                {
                    return false;
                }
                else if (cmd.at(0) == "exit" && cmd.size() == 1)
                {
                    cout << "Exit statement hit." << endl;
                    return false;
                }
                else
                {
                char* temp[cmd.size()+ 1];
                //Sets the command in a c-string form.
                for(int i = 0; i < cmd.size(); ++i)
                {
                    temp[i] = (char*)cmd.at(i).c_str();
                }
                //We push a null character at the end of the array to let execvp
                //know where to end with the command.
                temp[cmd.size()] = NULL;
                
                pid_t pid = fork();
                if (pid == 0)
                {
                    //If the command does not work
                    if(execvp(temp[0], temp) == -1)
                    {
                        perror("exec");
                        return false;
                    }
                }
                if (pid > 0)
                {
                    if (wait(0) == -1)
                    {
                        perror("wait");
                    }
                }
                return true;
                }
        }
        bool connector(bool cmdExecuted, string connector)
        {
            if(connector == "||") 
            {
                if(cmdExecuted == false)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if(connector == "&&")
            {
                if(cmdExecuted == true)
                {
                    return true;  
                }
                else
                {
                    return false;
                }
            }
            else if(connector == "#")
            {
                return false;
            }
            //Semicolon will always execute next command 
            else 
            {
                return true;    
            }
        }
};