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
