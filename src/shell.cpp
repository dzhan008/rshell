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
            
            //Quick check if the first command is exit.
	        if (v.at(0) == "exit")
	        {
		        return 2;
	        }
	        
	        //Organize the vector by precedence.
	        vector<vector <string> > commands = organize_commands(v);
	        
	        //Create a new vector to store and run commands.
	        //Also create a last_output variable, telling us whether or not the
	        //last output of a command was true or false.
            vector<string> command_to_execute;
	        int last_output = 2; //0 - false 1 - true
	        int cnt_output = 2;
            
            for (unsigned i = 0; i < commands.size(); ++i)
            {
                    
                for (unsigned j = 0; j < commands.at(i).size(); ++j)
                {
                    //std::cout << commands.at(i).at(j) << std::endl; Use to output commands
                    //If comment is seen, we don't push it to our command.
                    //Instead, we execute it and jump out of the loop.
                    if(commands.at(i).at(j).at(0) == '#')
                    {
                        execute(command_to_execute);
                        return 4; //Error code for command
                    }
                    //If a connector is detected.
                    else if (commands.at(i).at(j) == "||" || commands.at(i).at(j) == "&&" || commands.at(i).at(j) == ";")
                    {
                        //std::cout << commands.at(i).at(j) << std::endl;
                        //std::cout << last_output << " " << cnt_output << std::endl;

                        //Case 0: The connector is either the only element in the vector OR the first element
                        //Also does quick checks for && in case the past commands failed already
                        //Check if the command failed last time and the connector is an "and" symbol
    		            if (last_output == 0 && commands.at(i).at(j) == "&&")
    		            {
    			            return 1;
    		            }
    		            //Checks if the last command succeeded, the input is &&, and that it is either the only command OR the first element
    		            //in another command.
    		            else if (last_output == 1 && commands.at(i).at(0) == "&&" && j == 0)
    		            {
    		                cnt_output = 1; //We want to simulate that && connector goes true, so we make out cnt_output to 1 to indicate that
    		                                // we can go on with the next command.
    		            }
    		            //Checks for past || operators. If the command before || runs true, everything afterward should
                        //not run at all.
                        else if(last_output == 1 && cnt_output == 0)
                        {
                            break;
                        }
    		            //Checks if the last command failed, the input is ||, and that it is either the only command OR the first element
    		            //in another command.
    		            else if (last_output == 0 && commands.at(i).at(0) == "||")
    		            {
    		                last_output = 2;
    		            }
    		            //Checks if the last command failed and that the connector is an "or" symbol
    		            else if (last_output == 0 && commands.at(i).at(j) == "||")
    		            {
    		                ++i;
    		            }
    		            //Checks if the last command succeeded, the input is ||, and that it is either the only command OR the first element
    		            //in another command.
    		            else if(last_output == 1 && commands.at(i).at(0) == "||")
    		            {
    		                cnt_output = 0;
    		                break; //We know that if it is either of those cases, then we don't do anything else to the rest of the input.
    		            }
    		            else
    		            {
    		                //CASE 1: There are inputs BEFORE the connector.
    		                //Execute the command. Store its result in a boolean.
                            bool cmd_output = execute(command_to_execute);
    		                last_output = cmd_output;
                            command_to_execute.clear(); //Clear the vector to add in new commands.
                            //Check whether or not to add in new commands depending on the connector.
                            cnt_output = connector(cmd_output, commands.at(i).at(j));
                            
                            if (cnt_output == 0)
                            {   
    			                if (cmd_output == 1) //If the first command failed and we have an || connector after
    			                {
    			                    break; //Just return since nothing else is supposed to happen for that command.
    			                }
                                if (cmd_output == 0) //If the first command failed and we have an && connector after
    			                {
    			                    break; //Break because we're done with this command.
    			                }
                            }
    		            }
                    }
                    //CASE 2: The command has no connectors AND/OR this is the last element of the command
                    else if (j == (unsigned)commands.at(i).size() - 1)
                    {
                        //Checks for past || operators. If the command before || runs true, everything afterward should
                        //not run at all.
                        if(last_output == 1 && cnt_output == 0)
                        {
                            break;
                        }
                        else if (commands.at(i).at(j) == "exit")
                        {
                            return 2;
                        }
                        //Push the last command onto the vector
                        command_to_execute.push_back(commands.at(i).at(j));
                        bool cmd_output = execute(command_to_execute);
                        last_output = cmd_output; //Check to see if the command failed.
                        command_to_execute.clear();
                        if (cmd_output == false)
                        {
                            break; //Return code for error
                        }
                    }
                    //CASE 3: An piece of a command that is not a connector is pushed in.
                    else
                    {
                        if(last_output == 1 && cnt_output == 0)
                        {
                            break;
                        }
                        else
                        {
                            command_to_execute.push_back(commands.at(i).at(j));
                        }
                    }
                   // cout << v.at(i) << endl;
                }
            }
            return 0; //Return code for command sucessfully parsed
            
        }

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
                    else if(v.at(i).at(j) == '#')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }
                
                        temp = "#";
                        commands.push_back(temp);
                        temp = "";
                    }
                    else if(v.at(i).at(j) == '(')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }
                
                        temp = "(";
                        commands.push_back(temp);
                        temp = "";
                    }
                    else if(v.at(i).at(j) == ')')
                    {
                        if(temp != "")
                        {
                            commands.push_back(temp);
                        }
                
                        temp = ")";
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
	    /*for (unsigned i = 0; i < commands.size(); i++)
	    {
		    cout << commands.at(i) << endl;
	    }*/
            return commands;
        }
        //TO DO: Check for erroneous paratheses. *****************************************************
        vector<vector<string> >organize_commands(vector<string> whole_command)
        {
            //Vector to be returned
            vector<vector<string> > commands;
            //Vector of a single command that is constantly pushed into the vector above.
            vector<string> command;
            bool para_open = false; //Checks if the parathese is open or not.
            int para_loc = 0;
            
            for(unsigned i = 0; i < whole_command.size(); ++i)
            {
                if(whole_command.at(i) == "(")
                {
                    //We now know that there is an open paratheses.
                    para_open = true;
                    //Push the current command onto the vector.
                    if(!command.empty())
                    {
                        commands.push_back(command);
                        command.clear();
                    }
                    //Then, push the paratheses on the vector.
                    command.push_back(whole_command.at(i));
                    para_loc = command.size() - 1; //Store the location of the open paratheses.

                }
                else if(para_open == true)
                {
                    //There is a closed paratheses which will close the inner command.
                    if(whole_command.at(i) == ")")
                    {
                        //We know that this is a closed set of paratheses, so we push that command on there.
                        //Afterwards we removed the opening paratheses since we know that is this is a closed
                        //command.
                        para_open = false;
                        command.erase(command.begin() + para_loc);
                        commands.push_back(command);
                        command.clear();
                    }
                    else
                    {
                        //In this case we just add the piece of the command onto the vector before we close it.
                        command.push_back(whole_command.at(i));
                    }
                }
                else
                {   //Just add the piece of the command onto the vector.
                    command.push_back(whole_command.at(i));
                }
            }
            //If no paratheses were left in the vector we simply check of any commands were put into the vector
            //And push that in as well.
            if(!command.empty())
            {
                commands.push_back(command);
            }

            //Test to see if the commands are actually organized correctly.
            /*for (unsigned i = 0; i < commands.size(); ++i)
	        {
	            for(unsigned j = 0; j < commands.at(i).size(); j++)
	            {
	                std::cout << commands.at(i).at(j) << std::endl;
	            }
	            std::cout << "COMMAND" << std::endl;
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
                    return false;
                }
                else
                {
		        char **temp = new char*[cmd.size() + 1];
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