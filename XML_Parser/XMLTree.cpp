#include<fstream>
#include<vector>
#include "XMLTree.h"

size_t XMLTree::internalId = 0;
std::vector <std::string> XMLTree::commandLines = { "open", "close", "save" , "saveas" , "help" , "exit" ,
							"print", "select",  "set",  "children",  "child",  "text",  "delete",  "newchild",  "xpath", "newattr"};

XMLTree::XMLTree()
{
	root = new Node("root", nullptr, "root_id");
}

XMLTree::XMLTree(const XMLTree& source)
{
	root = new Node(*source.root); // deep copy of tree -> new pointers
	getNodesDFS(root, listOfNodes); // now we can safely get a list of pointers to nodes of tree with sharing them with source
}

XMLTree& XMLTree::operator=(const XMLTree& source)
{
	if (this != &source)
	{
		delete root;

		root = new Node(*source.root);
		getNodesDFS(root, listOfNodes);
	}
	return *this;
}

XMLTree::~XMLTree()
{
	freeMemory();
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
									 
			if (i <= size) // we can ignore userInput with smaller length than i and not do some checks
			{			   // example: if user inputs 'close' we don't check commands with length bigger than 5
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

		case 15: // settext <id> <value>
			getXMLInfo(xmlInfo, userInput, 7);
			break;
		}
		
		switch (indexOfCommand)	// get the command index the user inputs
		{
		case 0:	// open <file> is index 0
			open(fileOpen, fileName); // read file
			if (fileOpen)
			{
				getNodesDFS(root, listOfNodes); // get nodes and fix matching ids or give out internal ids if not present
				
				size_t maxId = internalId;	// we will find the biggest id in the tree, should be 0 at start
				size_t numberId = 0;		// convert ids to size_t for easier comparison

				for (Node* node : listOfNodes) // go through all nodes and attributes(keys) and find biggest id
				{
					numberId = to_size_t(node->id);
					if (numberId > maxId)
						maxId = numberId;

					for (Key* key : node->keys)
					{
						numberId = to_size_t(key->id);
						if (numberId > maxId)
							maxId = numberId;
					}
				}

				internalId = ++maxId; // set internalId to maxId + 1 and so each bigger internalId will be unique

				for (Node* node : listOfNodes) // go through all nodes and attributes(keys) and set unique id
				{							   // to nodes and keys(attributes) without ids'
					if (node->id == "")
						node->id = std::to_string(internalId++);

					for (Key* key : node->keys)
						if (key->id == "")
							key->id = std::to_string(internalId++);
				}

				// check for matching ids and give a unique id according
				for (Node* node : listOfNodes)  // just works :), goes through all nodes in lisfOfNodes
				{	
					for (Node* comp : listOfNodes) // for one node, goes through all nodes again
					{
						for (Key* key : node->keys) // compare id between node and keys(attribute) of different nodes
							if (node->id == key->id)
								key->id = std::to_string(internalId++);

						if (node->id == comp->id) //  compare id between node and node(attribute)
							comp->id = std::to_string(internalId++);
					}

					for(Key* key : node->keys) // compare id between node and keys(attribute) of same node
						if (node->id == key->id)
							key->id = std::to_string(internalId++);
				}
						
			}
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
				children(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 10: // child <id> <n>
			if (fileOpen)
				child(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 11: // text <id>
			if (fileOpen)
				text(xmlInfo);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 12: // delete <id> <key>
			if (fileOpen)
				deleteIdKey(xmlInfo, changesMade);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 13: // newchild <id>
			if (fileOpen)
				newchild(xmlInfo, changesMade);
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 14: // xpath <id> <XPath>
			//if (fileOpen)
				std::cout << "> Xpath commands not supported." << std::endl << std::endl;
			//else
				//std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		case 15: // newattr <id> <key> <value> ; custom command as my structure is not accurate to normal xml tree and
			if (fileOpen)	// it treats elements and attributes a little differently (different structures)
				newattr(xmlInfo, changesMade); // we can't add attributes without this command
			else
				std::cout << "> No file is opened." << std::endl << std::endl;
			break;

		default: //  if getIndexOfCommand(userInput) has returned -1 -> invalid command
			std::cout << "> " << userInput << ": command not found" <<std::endl << std::endl;
			break;
		}
	}
}

void XMLTree::freeMemory()
{
	delete root;
	root = nullptr;

	// we don't delete the pointers in listOfNodes as they are deleted when the destructor of root is called 
	// and we would be trying to delete something we have already deleted; we can use clear on listOfNodes 
	// as it only removes the pointers in the vector and doesn't delete the memory they are pointing to

	//for (Node* node : listOfNodes)
		//delete node;
	listOfNodes.clear();
}

size_t XMLTree::to_size_t(std::string str) const
{
	size_t number = 0;
	size_t i = 0;

	while (str[i] != '\0') // go through string
	{
		if (str[i] >= '0' && str[i] <= '9') // if symbol at position i is a number
			number = number * 10 + (str[i] - '0');	// add to number

		i++;
	}

	return number;
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

void XMLTree::getNodesDFS(Node* currentNode, std::vector <Node*>& listOfNodesParam)
{	
	size_t index = 0;
	listOfNodesParam.push_back(currentNode);

	while (index < currentNode->children.size())
		getNodesDFS(currentNode->children[index++], listOfNodesParam);

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
	std::cout << "open <file> ----------------- opens <file>\n";							// 0
	std::cout << "close ----------------------- closes currently opened file\n";			// 1
	std::cout << "save ------------------------ saves the currently open file\n";			// 2
	std::cout << "saveas <file> --------------- saves the currently open file in <file>\n";	// 3
	std::cout << "help ------------------------ prints this information\n";					// 4
	std::cout << "exit ------------------------ exists the program\n";						// 5
	std::cout << "print ----------------------- prints xml file to console\n";				// 6
	std::cout << "select <id> <key> ----------- prints attribute value by id and key\n";	// 7
	std::cout << "set <id> <key> <value> ------ sets a value to an attribute\n";			// 8
	std::cout << "children <id> --------------- prints values of attribute\n";				// 9
	std::cout << "child <id> <n> -------------- access to an elements n-th child\n";		// 10
	std::cout << "text <id> ------------------- access to an elements text\n";				// 11
	std::cout << "delete <id> <key> ----------- deletes an attribute of an element\n";		// 12
	std::cout << "newchild <id> --------------- adds a new child to element\n";				// 13
	//std::cout << "xpath <id> <XPath> ---------- simple xpath operations\n";					// 14 doesn't support xpath
	std::cout << "newattr <id> <key> <value> -- adds a new attribute to a node\n\n";		// 15
}

void XMLTree::select(std::string xmlInfo) const // select <id> <key>
{
	if (xmlInfo != "") // check if xmlInfo has any info
	{																	// seperate the id and key in xmlInfo
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));		// id is from position 0 to the first occurance of ' '
		std::string keyParam = xmlInfo.substr(xmlInfo.find(' ') + 1);	// key is the rest of string
																		// its the same for the other xml commands
		for (const Node* node : listOfNodes)	// go through all nodes is listOfNodes
			if (node->id == idParam)			// check which node has an id matching the one from xmlInfo
			{
				for (const Key* key : node->keys)	// go through all key of each node
					if (key->name == keyParam)		// and check for matching name of key
					{								// print to console
						std::cout << "> Value: " << key->value << std::endl << std::endl;
						return;
					}

				break;
			}
		// if we havent found a match inform user
		std::cout << "> The id is wrong or the key doesn't exist." << std::endl;
	}
	else
		std::cout<< "> No id and key inputed." << std::endl;
}

void XMLTree::set(std::string xmlInfo, bool& changesMade)  // set <id> <key> <value>
{
	if (xmlInfo != "") // check if xmlInfo has any info
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));		// get id: from pos 0 to first ' '
		xmlInfo = xmlInfo.substr(xmlInfo.find(' ') + 1);// as we already store id in idParam, we set xmlInfo to be from that first ' ' to the end of string
		std::string keyParam = xmlInfo.substr(0, xmlInfo.find(' ')); // this way we ease key and value seperation as we now again have
		std::string valueParam = xmlInfo.substr(xmlInfo.find(' ') + 1); // ' ' in 2 places instead of 3

		for (Node* node : listOfNodes) // // go through all nodes is listOfNodes
		{
			for (Key* key : node->keys) // go through all key of each node
				if (key->name == keyParam && key->id == idParam) // and check for matching name of key and id for Keys
				{
					changesMade = true; // say we have made changed to file for exit command

					std::cout << "> Value '" << key->value;
					key->value = valueParam;
					std::cout << "' successfully changed to: " << key->value << std::endl << std::endl;

					return;
				}

			if (node->id == idParam && node->name == keyParam) // check for matching name and id for Nodes
			{
				changesMade = true; // say we have made changed to file for exit command

				std::cout << "> Value '" << node->name;
				node->name = valueParam;
				std::cout << "' successfully changed to: " << node->name << std::endl << std::endl;

				return;
			}
		}

		// if no match was found inform user
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

		for (const Node* node : listOfNodes)	// go through all nodes is listOfNodes
			if (node->id == idParam) // find a matching id from nodes; we don't look at keys as they cant have a child
			{
				std::cout << "> Attributes:" << std::endl;
				for (Key* key : node->keys)	// not sure which is wanted for children -> keys or children of node
					std::cout << *key << std::endl;

				std::cout << "> Children nodes:" << std::endl;
				for (Node* child : node->children)	// so i added both
					std::cout << child->name << " id=\"" << child->id <<"\"" << std::endl;

				std::cout << std::endl;
				return;
			}

		std::cout << "> The id is wrong or doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id inputed." << std::endl << std::endl;
}

void XMLTree::child(std::string xmlInfo) const // child <id> <n>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string n = xmlInfo.substr(xmlInfo.find(' ') + 1);

		for (const Node* node : listOfNodes)
			if (node->id == idParam)
			{								 // as n is to be an index we
				size_t index = to_size_t(n); // convert n to a size_t value so we can compare it

				if (node->children.size() > index)
					std::cout << node->children[index]->name << " id='" << node->children[index]->id << "'" << std::endl << std::endl;
				else
					std::cout << "> Element '" << node->name << "' has no child with index: " << index << std::endl << std::endl;

				return;
			}

		std::cout << "> The id is wrong or doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id and n inputed." << std::endl << std::endl;
}

void XMLTree::text(std::string xmlInfo) const // text <id>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));

		for (const Node* node : listOfNodes) // go through all nodes in tree
		{
			for (const Key* key : node->keys) // go through all their keys
				if (key->id == idParam)	      // and check for matching ids
				{
					std::cout << "> Text: " << key->value << std::endl << std::endl;
					return;
				}

			if (node->id == idParam) // check if nodes have a match too
			{
				std::cout << "> Text(name): " << node->name << std::endl << std::endl;
				return;
			}
		}	

		std::cout << "> The id is wrong or doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id inputed." << std::endl << std::endl;
}

void XMLTree::deleteIdKey(std::string xmlInfo, bool& changesMade) // delete <id> <key>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string keyParam = xmlInfo.substr(xmlInfo.find(' ') + 1);

		size_t indexNode = 0;
		for (Node* node : listOfNodes) // we go through all our nodes in the tree and check:
		{
			size_t indexKey = 0; // the index in node->keys of the key we want to delete
			for (Key* key : node->keys) // if any key matches the id and keyName user has inputed
				if (key->id == idParam && key->name == keyParam)
				{
					changesMade = true; // say we have made changed to the tree structure

					size_t lastElement = node->keys.size() - 1; // find index of last element of node->keys
					std::swap(node->keys[indexKey], node->keys[lastElement]); // swap element we want to delete and last element

					delete node->keys[lastElement]; // delete last element
					node->keys[lastElement] = nullptr; // and set it to nullptr
					node->keys.pop_back(); // discard last element
					// we do this because the std::vector::erase() function wasn't available to use and this method was 
					// invented for that, one downside is that it changes the order of the elements in node->keys

					std::cout << "> Successfully deleted '" << keyParam << "'." << std::endl << std::endl;
					return;
				} // if key doesn't match the id and keyName user has inputed, increase indexKey
				else indexKey++;

			if (node->id == idParam && node->name == keyParam) // same deals with nodes with one difference
			{ // we have node pointers to the same place in two places in XMLTree: listOfNodes and the children vector of parent node
				changesMade = true;

				size_t lastElement = listOfNodes.size() - 1; // find index of last element of node->children
				std::swap(listOfNodes[indexNode], listOfNodes [lastElement]); // swap element we want to delete and last element

				Node* parentNode = listOfNodes[lastElement]->parent; // will need later as to not break program

				// we will delete the node from listOfNodes but there will be a nullptr somewhere in the children vector of 
				// parent node, thats why we got its parent node before deleting it and the index of the node in children of parent

				indexNode = 0; // we can reuse indexNode from before as we have found our node in listOfNodes
				for (Node* child : parentNode->children) // we get the index of node(the one we want to delete) in children vector
					if (child == node)
						break; // if no match we increase indexNode
					else indexNode++;

				delete listOfNodes[lastElement]; // delete last element
				listOfNodes[lastElement] = nullptr; // and set it to nullptr
				listOfNodes.pop_back(); // discard last element of listOfNodes

				// we have handled listOfNodes and now the parents children vector
				// we swap the node pointer we want to delete with the last pointer in vector
				std::swap(parentNode->children[indexNode], parentNode->children[parentNode->children.size() - 1]);
				// as we have already delete the memory its pointing to, we can just pop it from vector
				parentNode->children.pop_back();

				std::cout << "> Successfully deleted '" << keyParam << "'." << std::endl << std::endl;
				return;
			}

			indexNode++;
		}

		std::cout << "> The id is wrong or the key doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id and key inputed." << std::endl << std::endl;
}

void XMLTree::newchild(std::string xmlInfo, bool& changesMade) // newchild <id>
{
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));

		for (Node* node : listOfNodes)
			if (node->id == idParam)
			{
				changesMade = true;
				
				// we give it name blank so we can later change it more easily with set command
				Node* newNode = new Node("blank", node, std::to_string(internalId++));
				listOfNodes.push_back(newNode); // add node to the listOfNodes vector
				node->children.push_back(newNode); // and add it to the children vector of node with matching id

				std::cout << "> Successfully added child to '" << node->name << "' with id='" << newNode->id << "'." << std::endl << std::endl;
				return;
			}

		std::cout << "> The id is wrong or doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id inputed." << std::endl << std::endl;
}

void XMLTree::newattr(std::string xmlInfo, bool& changesMade) // settext <id> <key> <value>, custom xml command
{				
	// Without this we can't add keys(attributes) to the tree as my code makes a distinction between the two and
	// doesn't treat attributes as nodes. I realized this at the end and it was too much to change in the last day.
	if (xmlInfo != "")
	{
		std::string idParam = xmlInfo.substr(0, xmlInfo.find(' '));
		xmlInfo = xmlInfo.substr(xmlInfo.find(' ') + 1);
		std::string keyParam = xmlInfo.substr(0, xmlInfo.find(' '));
		std::string valueParam = xmlInfo.substr(xmlInfo.find(' ') + 1);

		for (Node* node : listOfNodes)
			if (node->id == idParam)
			{
				changesMade = true;

				Key* newKey = new Key(keyParam, valueParam, std::to_string(internalId++));
				node->keys.push_back(newKey);

				std::cout << "> Successfully added attribute " << *newKey << " to '" << node->name << "'." << std::endl << std::endl;
				return;
			}

		std::cout << "> The id is wrong or the key doesn't exist." << std::endl << std::endl;
	}
	else
		std::cout << "> No id and index inputed." << std::endl << std::endl;
}