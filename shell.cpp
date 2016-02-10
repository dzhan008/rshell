#include <boost/tokenizer.hpp>
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
        vector <string> v;
    public:
        Shell() {};
        ~Shell() {};
        void run() 
        {
            
            char * name;
            char hostname[1024];
            gethostname(hostname, 1024);
            struct passwd* pass;
            pass = getpwuid(getuid());
            name = pass->pw_name;
               
            while(command != "exit") 
            {
                cout << name << "@" << hostname << "$ ";
                getline(cin, command);
                cout << endl;
                parse(command);
            }
        }
        // Note that we cannot distinguish between "|" and "||" so if a user typed "|" on accident parsing would not catch it.
        // ie. char_separator<char> sep2("|"); == char_separator<char> sep2("||");
        void parse(string cmdLine) 
        {
            //Also cmdLine does not get updated each time we parse. Forgot about that.
            
            // typedef tokenizer<char_separator<char> > tokenizer1;
            // char_separator<char> sep1(" ");
            // tokenizer1 tokens1(cmdLine, sep1);
            
            //Parses by each command
            typedef tokenizer<char_separator<char> > tokenizer;
            char_separator<char> sep(" ");
            tokenizer tokens(cmdLine, sep);
            vector<string> v;
            string test;
            for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
            {
                if(*it == "||" || *it == "&&" || *it == ";")
                {
                    std::cout << "Connector detected!" << std::endl;
                    v.push_back(test);
                    test = "";
                    v.push_back(*it);

                }
                else
                {
                    test = test + *it + " ";
                }
                //cout << *it;
            }
            
            for (int i = 0; i < v.size(); ++i)
            {
                cout << v.at(i) << endl;
            }
            //cout << endl;
        }
        
        /*
        Notes: If we can't used an escaped_list_separator, we can simply split the characters by spaces. We then make a check where if a string is not ||, &&, or ;,
        then we concatencate it to the original string. For example:
        
        If our tokenizer has the strings "Hi" " Bye", "Meep", "||"
        We can have a string test and parse through the tokenizer to add Hi, Bye, and Meep together. However, if we see a || we can store it into another string.
        Then, we move on. This way we can grab all the arguments AND the connector in two separate strings.
        
        However, we may have a problem with this. Since we're splitting these up into different strings with delimiters, we need to find a way to keep the white space.
        We can keep it using Kept Limiters.
        */
        
        // bool execute(vector<string> v)
        // {
        //     for(int i = 0; i < v.size(); ++i)
        //     {
        //         if(v.at(i) == "||")
        //         {
                    
        //         }
        //         else if(v.at(i) == "&&")
        //         {
                    
        //         }
        //         else if(v.at(i) == ";")
        //         {
                    
        //         }
        //         else
        //         {
                
        //         }
        //         char* temp[2];
        //         temp[0] = (char*)v.at(i).c_str();
        //         temp[1] = NULL;
        //         pit_t pid = fork();
        //         if (pid == 0)
        //         {
        //             if(execvp(temp[0], temp) == -1)
        //             {
        //                 perror ("exec");
        //                 return false;
        //             }
        //         }
        //         if (pid > 0)
        //         {
        //             if (wait(0) == -1)
        //             {
        //                 perror("wait");
        //             }
        //         }

        //     }
        // }
        // return true;
    };