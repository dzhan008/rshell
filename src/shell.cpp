#include <boost/tokenizer.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include <pwd.h>
#include <iostream>

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
        void run() 
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
             
                if(exit_override)
                {
                    return;
                }
                else
                {
               	    cout << name << "@" << hostname << "$ ";
                    getline(cin, command);
                    if(command == "exit") //If the command is just exit
                    {
                        return;  
                    }
                    else if(command != "") //If we entered an actual command
                    {
			int result = parse(command);
		        if(result == 2)
                        {
                            exit_override = true;
			    exit(0);
                        }
			else if(result == 1)
			{
			    cout << "Error: Command Invalid." << endl;
			}
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
	    //Takes each tokenized phrase and anaylzes it for different cases.
            v = analyze_split(v);

	    if (v.at(0) == "exit")
	    {
		    return 2;
	    }
                            
            vector<string> command_to_execute;
	    bool last_output = false;
            for (unsigned i = 0; i < v.size(); ++i)
            {
                //If comment is seen, we don't push it to our command.
                //Instead, we execute it and jump out of the loop.
                if(v.at(i).at(0) == '#')
                {
                    execute(command_to_execute);
                    return 1;
                }
                else if (i == (unsigned)v.size() - 1)
                {
                    if (v.at(i) == "exit")
                    {
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
		    if (last_output == false && v.at(i) == "&&")
		    {
			return 1;
		    }
		    else if (last_output == false && v.at(i) == "||")
		    {
		        ++i;
		    }
		    else
		    {
                        bool cmd_output = execute(command_to_execute);
		        last_output = cmd_output;
                        command_to_execute.clear();
                        bool cnt_output = connector(cmd_output, v.at(i));
                        if (cnt_output == false)
                        {   
			    if (cmd_output == true)
			    {       
			        ++i;
			    }
                            if (cmd_output == false)
			    {
			        return 1; //Return code for failure
			    }
                        }
		    }
                }
                else
                {
                    command_to_execute.push_back(v.at(i));
                }
               // cout << v.at(i) << endl;
            }
            return 0; //Return code for command sucessfully parsed
        }
        /*Now that we have the command class, we can push commands into a vector.  Let us
	 * understand that a new command is a command surrounded by paratheses by default.
	 * So, a command without paratheses is treated as a regular command with only one
	 * set of paratheses around it.
	 * This makes parsing for these cases simpler since we can use our old algroithm
	 * for parsing just one command with multiple characters.
	 *
	 * Now, if we have sets of commands surrounding by a paratheses, i.e.
	 * (ls && mkdir) && (pwd || test)
	 *
	 * We know introduce types into our command class. See here that we have two
	 * commands surrounded by paratheses and a connector. This means that we must
	 * find out if each paratheses is true or false and then call the connector
	 * logic if a condition is made.
	 *
	 * So, we have two types of "commands": The command itself and the connector.
	 * Distinguishing these will allow us to manipulate what to do after a command
	 * is executed. We can use a boolean result to indicate whether or not a command
	 * has succeeded. If the type is a connector, we don't care about the result.
	 * Instead, we call the connector function to see if the previous command
	 * succeeded. If it succeeds, we will call the next set of commands. Again,
	 * this does NOT matter at all if we one ONLY one command with no paratheses (aka,
	 * anything we did in the second assignment).
	 *
	 * So, the execution of the commmands now takes in the vector of commands and
	 * executes commands in every element of that vector until something that causes
	 * the command to fail will run. If not, it'll run through the vector.
        */
        
        //Accounts for connectors mixed with other text
        vector<string> analyze_split(vector<string>& v)
        {
            vector<string> commands;
            //Look at each string in the vector.

            for(unsigned i = 0; i < v.size(); ++i)
            {
                string temp;
                //Look at each letter of each string for connectors.
                for(unsigned j = 0; j < v.at(i).size(); ++j)
                {
                    //Tests whether or not we found a connector.
		    if(v.at(i).at(j) == '|' && j + 1 != v.at(i).size())
		    {
                        if(v.at(i).at(j + 1) == '|')
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
		    }
		    else if(v.at(i).at(j) == '&' && j + 1 != v.at(i).size())
		    {	    
		        if( v.at(i).at(j + 1) == '&')
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
	    
           //Test to see if commands are actually parsed correctly.
	   /* for (unsigned i = 0; i < commands.size(); i++)
	    {
		    cout << commands.at(i) << endl;
	    }*/
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
		    char **temp = new char*[cmd.size() + 1];
               // char* temp[cmd.size()+ 1];
                //Sets the command in a c-string form.
                for(unsigned i = 0; i < cmd.size(); ++i)
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
	//Connector Logic is put here.
	//Made to decide whether or not to execute the next command
	//in sequence given the connector.
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
