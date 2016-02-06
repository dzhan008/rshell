#include <boost/tokenizer.hpp>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>

using namespace std;
using namespace boost;

class Shell {
    private:
        string command;
        vector <string> v;
    public:
        Shell() {};
        ~Shell() {};
        void parse(string parsley) 
        {
            typedef tokenizer<escaped_list_separator<char> > tokenizer;
            escaped_list_separator<char> sep("", "||&&;", "");
            tokenizer tokens(parsley, sep);
            int i = 0;
            for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it, ++i)
            {
                cout << *it << endl;
                //v.push_back(*it);
            }
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
        
/*        bool execute(vector<string> v)
        {
            for(i = 0; i < v.size(); ++i)
            {
                if(v.at(i) == "||")
                {
                    
                }
                else if(v.at(i) == "&&")
                {
                    
                }
                else if(v.at(i) == ";")
                {
                    
                }
                else
                {
                
                ls -a || cd blah
                    char* temp[2];
                    temp[0] = (char*)v.at(i).c_str();
                    temp[1] = NULL;
                    pit_t pid = fork();
                    if (pid == 0)
                    {
                        if(execvp(temp[0], temp) == -1)
                        {
                            perror ("exec");
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

                }
            }
            return true;
        }*/
        void run() 
        {
            while(command != "exit") 
            {
                cout << "$ ";
                getline(cin, command);
                cout << endl;
                parse(command);

            }
        }
};
