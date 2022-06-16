#include<iostream>
#include<fstream>
#include"Node.h"
#include"XMLTree.h"
using namespace std;

int main()
{
	//XMLTree tree;
	//tree.runProgram();

    std::vector <std::string> commandLines = { "open", "close", "save" , "saveas" , "help" , "exit" ,
                                    "print", "select",  "set",  "children",  "child",  "text",  "delete",  "newchild",  "xpath" };
    string input;
    while (getline(cin, input))
    { // quit the program with ctrl-d
        auto it = commandLines.find(input);
        if (it != end(c))
        {
            (it->second)(); // execute the command
        }
        else
        {
            std::cout << "command \"" << input << "\" not known" << std::endl;
        }
    }

	return 0;
}