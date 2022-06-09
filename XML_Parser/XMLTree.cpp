#include<fstream>
#include "XMLTree.h"

const char XMLTree::commandLines[6][7] = { "open", "close", "save" , "saveas" , "help" , "exit" };

XMLTree::XMLTree()
{
	root = new Node("root", nullptr);
}

XMLTree::XMLTree(const XMLTree& source)
{
	root = new Node(*source.root);
}

XMLTree& XMLTree::operator=(const XMLTree& source)
{
	if (this != &source)
	{
		delete root;

		root = new Node(*source.root);
	}
	return *this;
}

XMLTree::~XMLTree()
{
	delete root;
}

void XMLTree::runProgram()
{
	bool runProgram = true;
	bool fileOpen = false;
	std::string fileName;
	char userInput[128];
	std::ofstream out;

	printHelp();

	while (runProgram)
	{
		std::cout << "& ";
		std::cin.getline(userInput, 128, '\n');
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
			printHelp();
			break;

		case 5:			// exit
			if (fileOpen) // check if we have an open file
			{
				std::cout << "> You have an open file with unsaved changes, please select 'close' or 'save' first." << std::endl;
				while (true)
				{
					std::cout << "& ";
					std::cin.getline(userInput, 128, '\n');

					short tempIndex = 1; // preset to close program without saving file changes
					if (strcmp(userInput, "close") == 0)
					{
						break;
					}
					else if (strcmp(userInput, "save") == 0)
					{
						tempIndex = 2; // set index to save file changes
						break;
					}
				}
				// saveFile();
			}
			runProgram = false;
			std::cout << "> Exiting program ..." << std::endl;
			break;

		default:
			std::cout << "> Invalid command." << std::endl;
			break;
		}
	}

	if (fileOpen)		// just in case file is still opened when we close the program
		out.close();

	// treat as if main
	/*root->addNode("node 1");
	root->addNode("node 2");
	root->children[0]->addNode("node 1_1");
	root->children[1]->addNode("node 1_2");

	root->print();
	//-----------------------------
	Node n1("Node 1", nullptr);
	Node n2("Node 2", nullptr);
	Node n3("Node 3", &n2);

	n1.print();
	n2.print();
	n3.print();
	*/
}

short XMLTree::getIndexOfCommand(const char command[7])
{
	for (size_t i = 0; i < 6; i++)
		if (strcmp(command, commandLines[i]) == 0)
			return i;
	
	return -1;
}

void XMLTree::printHelp() const
{
	std::cout << "The following commands are supported:\n";
	std::cout << "open <file> ------ opens <file>\n";
	std::cout << "close ------------ closes currently opened file\n";
	std::cout << "save ------------- saves the currently open file\n";
	std::cout << "saveas <file> ---- saves the currently open file in <file>\n";
	std::cout << "help ------------- prints this information\n";
	std::cout << "exit ------------- exists the program\n\n";
}