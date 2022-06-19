#include<fstream>
#include "XMLTree.h"

std::vector <std::string> XMLTree::commandLines = { "open", "close", "save" , "saveas" , "help" , "exit" ,
									"print", "select",  "set",  "children",  "child",  "text",  "delete",  "newchild",  "xpath"};

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
	std::string xmlInfo;		// information after xml commands such as id, key, value, n or xpath
	char userInput[128] = {};	// the input we get from user
	char commandIdentifier[9] = {};	// helps us get the first symbols of user input and check if they are from commandLine
	short indexOfCommand = -1;	// after identifying command we store it's index from commandLine here

	printHelp();		// prints instructions to console in the beginning of dialog

	/*root->addNode("node_1");
	root->addNode("node_2");
	root->addNode("node_3");
	root->children[0]->addNode("node_1_1");

	root->addElement("qwe", "123");
	root->children[0]->addElement("qwe", "123");
	root->children[1]->addElement("qwe", "123");
	root->children[2]->addElement("qwe", "123");*/

	while (runProgram)	// while runProgramm is true, run dialog with user / until command exit
	{
		std::cout << "& ";							// prompt for user input
		std::cin.getline(userInput, 128, '\n');		// get user input
		unsigned size = strlen(userInput);

		// if command has a ' ' character, we check for commands with file names, paths or XML commands
		//if (size > 6)
		
		indexOfCommand = -1; // we reset it as to not break once in the below for()
		for (unsigned short i = 8; i >= 3; i--) // our commands have a min length of 3 and max length of 8
		{										// we iterate backwards as to not confuse save for saveas
			if (indexOfCommand != -1)	// check if we have found a valid command
				break;
									 // no command has length 7 so we can saftly skip the check and save a little time
			if (i != 7 && i <= size) // and ignore userInput with smaller length than i and not do some checks
			{						 // example: if user inputs 'close' we don't check commands with length bigger than 5
				strncpy_s(commandIdentifier, userInput, i); // we get the first i symbols from user input
				indexOfCommand = getIndexOfCommand(commandIdentifier); // and check if they are a valid command
			}
		}

		// commands 1, 2, 4, 5, 6 are standalone, so we have to check for file name, path or other if other commands were inputed
		switch (indexOfCommand)
		{
		case 0: // open
			getFileName(fileName, userInput, 4);
			break;

		case 3: // saveas
			getFileName(newFileName, userInput, 6);
			break;

		case 7: // select <id> <key>
			getXMLInfo(xmlInfo, userInput, 6);
			break;

		case 8: // set <id> <key> <value>
			getXMLInfo(xmlInfo, userInput, 3);
			break;

		case 9: // children <id>
			getXMLInfo(xmlInfo, userInput, 8);
			break;

		case 10: // child <id> <n>
			getXMLInfo(xmlInfo, userInput, 5);
			break;

		case 11: // text <id>
			getXMLInfo(xmlInfo, userInput, 4);
			break;

		case 12: // delete <id> <key>
			getXMLInfo(xmlInfo, userInput, 6);
			break;

		case 13: // newchild <id>
			getXMLInfo(xmlInfo, userInput, 8);
			break;

		case 14: // xpath <id> <XPath>
			getXMLInfo(xmlInfo, userInput, 5);
			break;
		}
		
		switch (indexOfCommand)	// get the command index the user inputs
		{
		case 0:	// open <file> is index 0
			open(fileOpen, fileName);
			getNodesDFS(root);
			break;

		case 1:	// close is index 1
			closeFile(fileOpen, fileName);
			break;

		case 2:	// save is index 2
			save(fileOpen, fileName);
			break;

		case 3:	// saveas <file> is index 3
			save(fileOpen, newFileName, false);
			break;

		case 4:	// help is index 4
			printHelp();	// prints all valid commands user can input
			break;

		case 5:	// exit is index 5
			if (fileOpen && changesMade) // check if we have an open file and made changes to it
			{
				//fileOpen = false; // even if it's unnecessary
				std::cout << "> You have an open file with unsaved changes, please select 'close', 'save' or 'saveas' first." << std::endl;
				
				short tempIndex = 1; // preset to close program without saving file changes
				while (true) // until user selects close or save
				{
					std::cout << "& ";	// user inputs save or saveas
					std::cin.getline(userInput, 128, '\n');

					if (strcmp(userInput, "close") == 0) // check if userInput is close
						break;
					else if (strcmp(userInput, "save") == 0) // check if userInput is save
					{
						tempIndex = 2;
						break;
					}
					else					// check if userInput is saveas
					{
						strncpy_s(commandIdentifier, userInput, 6);
						if (strcmp(commandIdentifier, "saveas") == 0)
						{
							tempIndex = 3;
							getFileName(newFileName, userInput, 6); // if command is 'saveas ', get file name or path
							break;
						}
					}
				}

				switch (tempIndex)
				{
				case 1: // close
					closeFile(fileOpen, fileName);
					break;

				case 2: // save
					save(fileOpen, fileName);
					break;

				case 3: // saveas
					save(fileOpen, newFileName, false);
					break;
				}
			}
			runProgram = false;
			std::cout << "> Exiting program ..." << std::endl;
			break;

		case 6: // print
			std::cout << *root << std::endl;
			break;

		case 7: // select <id> <key>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 8: // set <id> <key> <value>
			if (fileOpen)
				set(xmlInfo, changesMade);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 9: // children <id>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 10: // child <id> <n>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 11: // text <id>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 12: // delete <id> <key>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 13: // newchild <id>
			if (fileOpen)
				select(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		//case 14: // xpath <id> <XPath>
		//	if (fileOpen)
		//		select(xmlInfo);
		//	else
		//		std::cout << "> No file is opened." << std::endl << std::endl;
		//	break;

		default: //  if getIndexOfCommand(userInput) has returned -1 -> invalid command
			std::cout << "> Invalid command. Index: " << indexOfCommand <<std::endl << std::endl;
			break;
		}
	}
}

short XMLTree::getIndexOfCommand(const char command[9])
{
	short br = 0;

	for (std::string str : commandLines)
	{
		if (command == str)
			return br;

		br++;
	}

	return -1;
}

void XMLTree::getFileName(std::string& fileName, const char* userInput, short commandSize) const
{
	char tempBuffer[128] = {}; // store command and a buffer for the file name as strncpy_s didn't work with string for me

	// as we know the size of the command user inputed we can start reading from userInput + some offSet
	// where that offset is commandSize + 1; we assume there is a ' ' symbol after the command so the +1
	strncpy_s(tempBuffer, userInput + commandSize + 1, strlen(userInput) - commandSize); // get the file name
	fileName = tempBuffer;

	if (fileName.find(".xml") == -1) // check if fileNameParam doesn't contain '.xml' and add at the end
		fileName += ".xml";
}

void XMLTree::getXMLInfo(std::string& xmlInfo, const char* userInput, short commandSize) const
{
	char tempBuffer[128] = {}; // store command and a buffer for xmlInfo as strncpy_s didn't work with string for me

	// as we know the size of the command user inputed we can start reading from userInput + some offSet
	// where that offset is commandSize + 1; we assume there is a ' ' symbol after the command so the +1
	strncpy_s(tempBuffer, userInput + commandSize + 1, strlen(userInput) - commandSize); // get id, key, value (depends on command)
	xmlInfo = tempBuffer;																 // further work in runProgram
}

void XMLTree::getNodesDFS(Node* currentNode)
{	
	size_t index = 0;
	listOfNodes.push_back(currentNode);

	while (index < currentNode->children.size())
		getNodesDFS(currentNode->children[index++]);

	return;
}

void XMLTree::open(bool& fileOpenParam, const std::string fileNameParam)
{
	if (!fileOpenParam) // check if a file hasn't been already opened
	{
		if (fileNameParam == "") // check if the file name is empty
		{
			std::cout << "> No file name given." << std::endl << std::endl;
			fileOpenParam = false;
			return;
		}

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
					fileOpenParam = false;
					return; // return false as a file could NOT be created
				}

			std::cout << "> " + fileNameParam + " does not exist." << std::endl; // inform user
			std::cout << "> An empty file with the same name was created and can now be opened." << std::endl << std::endl;
			fileOpenParam = true;
			return; // return true as a file could be created

			out.close();
		}

		in >> *root;	  // read file and save as a tree in the root field
		in.close();

		std::cout << "Successfully opened " + fileNameParam << std::endl << std::endl;

		fileOpenParam = true;
		return;      // return true as to write in runProgram that the file was opened successfully
	}
	else
		std::cout << "> Another file is already opened." << std::endl << std::endl;
}

void XMLTree::save(bool fileOpenParam, const std::string fileNameParam, bool command) const
{
	if (fileOpenParam) // ckech if a file is opened
	{
		if (fileNameParam == "") // check if the file name is empty
		{
			std::cout << "> No file name given." << std::endl << std::endl;
			return;
		}

		std::ofstream out(fileNameParam);// sets goodbit to false if fileName is an incorrect path

		if (!out && fileNameParam != "") //  if there was an error opening file or fileName was empty
		{
			std::cout << "> Error saving to " + fileNameParam << std::endl;
			std::cout << "> File not saved successfully" << std::endl << std::endl;
			return;	// return true as to NOT write in runProgram that the file was saved successfully
		}

		out << *root; // write tree, correctly formated, to file
		out.close();

		if (command) // if true we inform user of 'save'
			std::cout << "> Successfully saved " + fileNameParam << std::endl << std::endl;
		else	     // if false we inform user of 'saveas' 
			std::cout << "> Successfully saved another " + fileNameParam << std::endl << std::endl;
	}
	else
		std::cout << "No file opened." << std::endl << std::endl;	// if we don't have an 'open' file inform user

	return;	// return true as to write in runProgram that the file was saved successfully
}

void XMLTree::closeFile(bool& fileOpenParam, const std::string fileNameParam)
{
	if (fileOpenParam)
	{
		fileOpenParam = false; // simulate closing of file
		root->~Node(); // free current tree of information
		root = new Node("root", nullptr); // create new root for XML_Tree encase of another open command
		std::cout << "Successfully closed " + fileNameParam << std::endl << std::endl;
	}
	else
		std::cout << "> No file is opened." << std::endl << std::endl;
}

void XMLTree::printHelp() const
{
	std::cout << "The following commands are supported:\n";									// Index in commandLies:
	std::cout << "open <file> --------------- opens <file>\n";								// 0
	std::cout << "close --------------------- closes currently opened file\n";				// 1
	std::cout << "save ---------------------- saves the currently open file\n";				// 2
	std::cout << "saveas <file> ------------- saves the currently open file in <file>\n";	// 3
	std::cout << "help ---------------------- prints this information\n";					// 4
	std::cout << "exit ---------------------- exists the program\n";						// 5
	std::cout << "print --------------------- prints xml tree\n";							// 6
	std::cout << "select <id> <key> --------- prints attribute by id and key\n";			// 7
	std::cout << "set <id> <key> <value> ---- sets a value to an attribute\n";				// 8
	std::cout << "children <id> ------------- prints values of attribute\n";				// 9
	std::cout << "child <id> <n> ------------ access to an elements n-th child\n";			// 10
	std::cout << "text <id> ----------------- access to an elements text\n";				// 11
	std::cout << "delete <id> <key> --------- deletes an attribute of an element\n";		// 12
	std::cout << "newchild <id> ------------- adds a new child to element\n";				// 13
	std::cout << "xpath <id> <XPath> -------- simple xpath operations\n\n";					// 14
}

void XMLTree::select(std::string xmlInfo) const // select <id> <key>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string keyParam = xmlInfo.substr(xmlInfo.find(' ') + 1);

		for (const Node* node : listOfNodes)
			if (node->id == idParam) // xmlInfo.substr(0)
			{
				for (const Key* key : node->keys)
					if (key->name == keyParam) // xmlInfo.substr(0)
					{
						std::cout << "> Value: " << key->value << std::endl << std::endl;
						return;
					}

				break;
			}

		std::cout << "> The id is wrong or the key doesn't exist." << std::endl;
	}
	else
		std::cout<< "> No id and key inputed." << std::endl;
}

void XMLTree::set(std::string xmlInfo, bool& changesMade) const  // set <id> <key> <value>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		xmlInfo = xmlInfo.substr(xmlInfo.find(' ') + 1);
		std::string keyParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string valueParam = xmlInfo.substr(xmlInfo.find(' ') + 1);

		for (const Node* node : listOfNodes)
			if (node->id == idParam) // xmlInfo.substr(0)
			{
				for (Key* key : node->keys)
					if (key->name == keyParam) // xmlInfo.substr(0)
					{
						changesMade = true;

						std::cout << "> Value '" << key->value;
						key->value = valueParam;
						std::cout << "' successfully changed to: " << key->value << std::endl << std::endl;

						return;
					}

				break;
			}

		std::cout << "> The id is wrong or the key doesn't exist." << std::endl;
	}
	else
		std::cout << "> No id, key and value inputed." << std::endl;
}

void XMLTree::children(std::string xmlInfo) const // children <id>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		xmlInfo = xmlInfo.substr(xmlInfo.find(' ') + 1);
		std::string keyParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string valueParam = xmlInfo.substr(xmlInfo.find(' ') + 1);

		std::cout << "set_" << idParam << "_" << std::endl;
		std::cout << "_" << keyParam << "_" << std::endl;
		std::cout << "_" << valueParam << "_" << std::endl;

		for (const Node* node : listOfNodes)
			if (node->id == idParam) // xmlInfo.substr(0)
			{
				for (Key* key : node->keys)
					if (key->name == keyParam) // xmlInfo.substr(0)
					{
						std::cout << "> Value '" << key->value;
						key->value = valueParam;
						std::cout << "' successfully changed to: " << key->value << std::endl << std::endl;

						return;
					}

				break;
			}

		std::cout << "> The id is wrong or the key doesn't exist." << std::endl;
	}
	else
		std::cout << "> No id, key and value inputed." << std::endl;
}
