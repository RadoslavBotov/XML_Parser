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
	bool changesMade = false;	// tells us if user has made changes to the XML_Tree
	std::string fileName;		// name of the file when user uses open
	std::string newFileName;	// name of the file when user uses saveas as to not lose the original name of file
	char userInput[128];		// the input we get from user
	short indexOfCommand = -1;

	root->key = "people";
	root->addNode("person");
	root->addNode("person");
	root->addNode("person");
	root->children[0]->addElement("name", "Ivan Burochovski");
	root->children[0]->addElement("address", "Russia");
	root->children[1]->addElement("name", "Alex Johns");
	root->children[1]->addElement("address", "USA");
	root->children[2]->addElement("name", "Radoslav Botov");
	root->children[2]->addElement("address", "Bulgaria");

	printHelp();		// prints instructions to console in the beginning of dialog

	while (runProgram)	// while runProgramm is true, run dialog with user / until command exit
	{
		std::cout << "& ";							// prompt for user input
		std::cin.getline(userInput, 128, '\n');		// get user input
		
		// if length of command bigger than 6, we check for commands 'open' and 'saveas' and a file name or path
		if (strlen(userInput) > 6)
		{
			bool flag; // below we set to false if the userInput passed to the first getFileName is for command 'open'
					   // otherwise it gets set to true and we check if it's 'saveas' command

			flag = getFileName(fileName, userInput, "open ", 4, indexOfCommand);   // if command is 'open ', get file name or path
			if (flag)
				getFileName(newFileName, userInput, "saveas ", 6, indexOfCommand); // if command is 'saveas ', get file name or path
		}
		else
			indexOfCommand = getIndexOfCommand(userInput); // if  length of command is less than 6, we check for other commands
		
		switch (indexOfCommand)	// get the command index the user inputs
		{
		case 0:	// open <file> is index 0
			if (!fileOpen)
			{
				fileOpen = open(fileName);	// if file is successfully opened -> fileOpen will be true, otherwise it will be false
				if (fileOpen)				// as such we can use it here instead of creating another boolean or calling open again
					std::cout << "Successfully opened " + fileName << std::endl << std::endl;
				// ... function that reads from file and save information as a tree with root the field 'root'
			}
			else
				std::cout << "> Another file is already opened." << std::endl << std::endl;
			break;

		case 1:	// close is index 1
			if (fileOpen)
			{
				fileOpen = false;
				std::cout << "Successfully closed " + fileName << std::endl << std::endl;
			}
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
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
				newFileName = ""; // we don't need newFileName again so we can get rid of it's information
			}
			else
				std::cout << "No file opened." << std::endl << std::endl;	// if we don't have an 'open' file inform user
			break;

		case 4:	// help is index 4
			printHelp();	// prints all valid commands user can input
			break;

		case 5:	// exit is index 5
			if (fileOpen && changesMade) // check if we have an open file
			{
				std::cout << "> You have an open file with unsaved changes, please select 'close' or 'save' first." << std::endl;
				while (true) // until user selects close or save
				{
					std::cout << "& ";	// user inputs save or saveas
					std::cin.getline(userInput, 128, '\n');

					short tempIndex = 1; // preset to close program without saving file changes
					if (strcmp(userInput, "close") == 0) // close has index 1
						break;
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
	for (short i = 0; i < 6; i++)
		if (strcmp(command, commandLines[i]) == 0)
			return i;
	
	return -1;
}

bool XMLTree::getFileName(std::string& fileNameParam, const char* userInput, const char* command, short commandSize, short& indexParam) const
{
	char tempBuffer[128]; // store command and a buffer for the file name as strncpy_s doesn't work with string
	
	if (strcmp(command, "open ") == 0)
		indexParam = 0;
	if (strcmp(command, "saveas ") == 0)
		indexParam = 3;

	strncpy_s(tempBuffer, userInput, commandSize + 1); // we get first commandSize symbols of user input
	if (strcmp(tempBuffer, command) == 0) // we only need to see if they are the 'open' command
	{
		strncpy_s(tempBuffer, userInput + commandSize + 1, strlen(userInput) - commandSize); // get the file name after open
		fileNameParam = tempBuffer;

		if (fileNameParam.find(".xml") == -1) // check if fileNameParam doesn't contain '.xml' and add at the end
			fileNameParam += ".xml";
		
		return false; // userInput is for command so we don't want to enter getFileName() again in runProgram
	}

	return true; // userInput wasn't for command so we want to enter getFileName() again and check for 'saveas' command
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

bool XMLTree::save(const std::string fileNameParam) const
{
	std::ofstream out(fileNameParam);// sets goodbit to false if fileName is an incorrect path

	if (!out && fileNameParam != "") //  if there was an error opening file or fileName was empty
	{
		std::cout << "> Error saving to " + fileNameParam << std::endl;
		std::cout << "> File not saved successfully" << std::endl << std::endl;
		return false;	// return true as to NOT write in runProgram that the file was saved successfully
	}

	out << *root;	// write tree, correctly formated, to file
	out.close();

	return true;	// return true as to write in runProgram that the file was saved successfully
}

bool XMLTree::open(const std::string fileNameParam)
{
	std::ifstream in(fileNameParam); // normally sets goodbit to false if file doesn't exist or path is incorrect

	if (!in && fileNameParam != "")	// if file doesn't exist or path is incorrect
	{
		std::ofstream out(fileNameParam); // we create a file with that name or path
		// if file name is NOT a path we don't have a problem and just create a file

		if (fileNameParam.find('\\') != -1) // if the file name was a path we have to check:
			if (!out) // if that path was NOT correct
			{
				std::cout << "> " + fileNameParam + " does not exist." << std::endl; // inform user
				std::cout << "> Path was incorrect and a file could not be created." << std::endl << std::endl;
				return false; // return false as a file could NOT be created
			}
			else // if that path was correct
			{
				std::cout << "> " + fileNameParam + " does not exist." << std::endl; // inform user
				std::cout << "> An empty file with the same name was created and can now be opened." << std::endl << std::endl;
				return true; // return true as a file could be created
			}

		out.close();
	}

	//in >> root;	  // read file and save as a tree in the root field
	in.close();

	return true;      // return true as to write in runProgram that the file was opened successfully
}