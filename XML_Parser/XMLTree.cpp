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
	std::string fileName = "testFile.xml";		// name of the file when user uses open
	std::string newFileName;	// name of the file when user uses saveas as to not lose the original name of file
	char userInput[128];		// the input we get from user

	root->key = "people";
	root->addNode("person");
	root->addNode("person");
	root->addNode("person");
	root->children[0]->addElement("name", "Ivan Burochovski");
	root->children[1]->addElement("name", "Petur Georgiev");
	root->children[2]->addElement("name", "Radoslav Botov");

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

		switch (getIndexOfCommand(userInput))	// get the command index the user inputs
		{
		case 0:	// open <file> is index 0
			fileOpen = open(fileName);	// if file is successfully opened -> fileOpen will be true, otherwise it will be false
			if(fileOpen)				// as such we can use it here instead of creating another boolean or calling open again
				std::cout << "Successfully opened " + fileName << std::endl << std::endl;
			// ... function that reads from file and save information as a tree with root the field 'root'
			break;

		case 1:	// close is index 1
			if (fileOpen)
			{
				fileOpen = false;
				std::cout << "Successfully closed " + fileName << std::endl << std::endl;
			}
			else
				std::cout << "> No file is opend." << std::endl << std::endl;
			break;

		case 2:	// save is index 2
			if (fileOpen)
			{
				if (save(fileName))		// we pass the fileName we got from the open command
					std::cout << "> Successfully saved " + fileName << std::endl << std::endl;
			}
			else
				std::cout << "No file opened." << std::endl << std::endl;	// if we don't have an 'open' file inform user
			break;

		case 3:	// saveas <file> is index 3
			if (fileOpen)
			{
				// get new file name
				if (save(newFileName)) // we use save again but we pass the new file name as parameter so we create a new file
					std::cout << "> Successfully saved another " + newFileName << std::endl << std::endl;
			}
			else
				std::cout << "No file opened." << std::endl << std::endl;	// if we don't have an 'open' file inform user
			break;

		case 4:	// help is index 4
			printHelp();	// prints all valid commands user can input
			break;

		case 5:	// exit is index 5
			if (fileOpen) // check if we have an open file
			{
				std::cout << "> You have an open file with unsaved changes, please select 'close' or 'save' first." << std::endl << std::endl;
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
			std::cout << "> Invalid command." << std::endl << std::endl;
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

bool XMLTree::save(std::string fileNameParam) const
{
	//std::ofstream out("C:\\Solutions\\XML_Parser\\tempFile.xml");
	std::ofstream out(fileNameParam);

	if (!out)
	{
		std::cout << "> Error saving file to " + fileNameParam + ".xml" << std::endl;
		std::cout << "> File not saved successfully." << std::endl;
		return false;	// return true as to NOT write in runProgram that the file was saved successfully
	}

	out << *root;	// write tree correctly formated in file
	out.close();

	return true;	// return true as to write in runProgram that the file was saved successfully
}

bool XMLTree::open(std::string fileNameParam)
{
	std::ifstream in(fileNameParam);

	if (!in)	// if there was some error when opening file
	{
		std::cout << "> Error reading from file " + fileNameParam + ".xml" << std::endl;	// inform user
		std::cout << "> File not read successfully." << std::endl;
		return false;		// return false as to NOT write in runProgram that the file was opened successfully
	}

	//in >> root;		// read file and save as a tree in the root field
	in.close();

	return true;	// return true as to write in runProgram that the file was opened successfully
}