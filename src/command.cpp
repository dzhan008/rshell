#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <vector>

using namespace std;

class command {
	private:
		bool result;
		int type; //0 = command 1 = connector
	public:
		vector<string>command;
		command()
		{
		   result = false;
		   type = 0;
		}
		command(bool res, int ty)
		{
		   result = res;
		   type = ty;
		}
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
		int get_type()
		{
		   return type;
		}



};
