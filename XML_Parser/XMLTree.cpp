#include<fstream>
#include "XMLTree.h"

const char XMLTree::commandLines[6][7] = { "open", "close", "save" , "saveas" , "help" , "exit" };

XMLTree::XMLTree()
{
	root = new Node("root", nullptr);	//
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
	bool runProgram = true;		// tells us if we have to run dialog or not
	bool fileOpen = false;		// we simulate a file; save, save-as, close, open all have an ofstream internally
	std::string fileName;		// we remember the name of the file we open so we can work with it correctly
	char userInput[128];		// the input we get from user

	root->addNode("node 1");
	root->addNode("node 2");
	root->children[0]->addNode("node 1_1");
	root->children[1]->addNode("node 1_2");

	printHelp();				// prints instructions to console in the beginning of dialog

	while (runProgram)			// while runProgramm is true, run dialog with user / until command exit
	{
		std::cout << "& ";							// prompt for user input
		std::cin.getline(userInput, 128, '\n');		// get user input
		
		// open, close, save, saveas, help, exit <=> command
		//    4,     5,    4,      6,    4,    4 <=> length of command
		if (strlen(userInput) > 6) // if length of command bigger than 6, there may be a name of file after command
		{
			// ... code
		}

		switch (getIndexOfCommand(userInput))		// get the command index the user inputs
		{
		case 0:						// open <file> is index 0
			fileOpen = true;
			// ... function that reads from file and save information as a tree with root the field 'root'
			break;

		case 1:						// close is index 1
			if (fileOpen)
			{
				fileOpen = false;
				std::cout << "Successfully closed " + fileName + ".xml" << std::endl;
			}
			else
				std::cout << "> No file is opend." << std::endl;
			break;

		case 2:						// save is index 2
			if (fileOpen)
			{
				save(fileName);		// we pass the fileName we got from the open command
				std::cout<< "> Successfully saved " + fileName + ".xml" << std::endl;
			}
			else
				std::cout << "No file opened." << std::endl;	// if we don't have an 'open' file inform user
			break;

		case 3:			// saveas <file> is index 3
			if (fileOpen)
			{
				// get new file name
				save(fileName);		// we use save again but we pass the new file name as parameter so we create a new file
				std::cout << "> Successfully saved another " + fileName + ".xml" << std::endl;
			}
			else
				std::cout << "No file opened." << std::endl;	// if we don't have an 'open' file inform user
			break;

		case 4:				// help is index 4
			printHelp();	// prints all valid commands user can input
			break;

		case 5:			// exit is index 5
			if (fileOpen) // check if we have an open file
			{
				std::cout << "> You have an open file with unsaved changes, please select 'close' or 'save' first." << std::endl;
				while (true)
				{
					std::cout << "& ";	// user inputs save or saveas
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

		default: //  if getIndexOfCommand(userInput) has returned -1 -> invalid command
			std::cout << "> Invalid command." << std::endl;
			break;
		}
	}
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

void XMLTree::save(std::string fileNameParam) const
{
	std::ofstream out(fileNameParam);
	out << root;
	out.close();
}