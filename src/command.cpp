#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <vector>

using namespace std;

class command {
	private:
		bool result;
	public:
		vector<string>command;
		bool get_result()
		{
		   return result;
		};
		void set_result(bool res)
		{
		   result = res;
		}
		vector<string> get_command()
		{
		   return command;
		}



};
