#include<iostream>
#include<fstream>
#include"Node.h"
#include"XMLTree.h"
using namespace std;

void help()
{
	std::cout << "\nThe following commands are supported:\n";
	std::cout << "open <file> ------ opens <file>\n";
	std::cout << "close ------------ closes currently opened file\n";
	std::cout << "save ------------- saves the currently open file\n";
	std::cout << "saveas <file> ---- saves the currently open file in <file>\n";
	std::cout << "help ------------- prints this information\n";
	std::cout << "exit ------------- exists the program\n\n";
}

short getIndexOfCommand(const char userInput[128])
{
	static const char commandLines[6][7] = { "open", "close", "save" , "saveas" , "help" , "exit" };
	
	for (short i = 0; i < 6; i++)
		if (strcmp(userInput, commandLines[i]) == 0)
			return i;

	return -1;
}

int main()
{
	Node n1("Node 1", nullptr);
	Node n2("Node 2", nullptr);
	Node n3("Node 3", &n2);

	n1.print();
	n2.print();
	n3.print();

	bool runProgram = true;
	char userInput[128];
	//XMLTree tree;
	//tree.testFunction();
	//tree.help();

	help();
	while (runProgram)
	{
		cin.getline(userInput, 128, '\n');
		//... work on input for easier command handling

		switch (getIndexOfCommand(userInput))
		{
		case 0:			// open <file>
			break;

		case 1:			// close
			break;

		case 2:			// save
			break;

		case 3:			// saveas <file>
			break;

		case 4:			// help
			help();
			break;

		case 5:			// exit
			runProgram = false;
			cout << "> Program closed!" << endl;
			break;

		default:
			cout << "> Invalid command." << endl;
			break;
		}
	}

	return 0;
}