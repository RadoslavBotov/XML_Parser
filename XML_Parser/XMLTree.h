#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"Node.h"

class XMLTree
{
public:
	/// Default constructor.
	/// 
	/// Initializes root field with default values: name = "root", parent = nullptr, id = "root_id".
	XMLTree();

	/// Copy constructor.
	/// 
	/// getNodesDFS() fills listOfNodes with pointers, so we make a deep copy of the tree structure so the nodes get new pointers
	/// and then use getNodesDFS() to get a list of those pointers. This way we don't share pointers between instances of XMLTree.
	XMLTree(const XMLTree& source);
	
	/// Operator =.
	XMLTree& operator = (const XMLTree& source);

	/// Destructor.
	/// 
	/// Uses freeMemory().
	~XMLTree();

	/// Simulate dialog with user.
	/// 
	/// As not to make user write their own code in main, we write it in runProgram and user only needs to create the XMLTree
	/// object and call this function. It is the only public function of XMLTree class.
	void runProgram();

private:
	/// The root of the tree.
	/// 
	/// Pointer to a Node object acting as root of our XMLTree.
	Node* root;

	/// All nodes of our tree.
	/// 
	/// A vector of pointers to Node objects. Makes it easier to make changes to the tree without using
	/// dfs or bfs for every command. Less recursion and easier logic.
	std::vector <Node*> listOfNodes; // we will use this to skip using DFS every time we need to change nodes of tree	
	// it can create problems when using copy constructor and operator = but its handled

	/// All command XMLTree supports.
	/// 
	/// A vector of strings, containing all commands our class supports.
	static std::vector <std::string> commandLines;

	/// Internal id value.
	/// 
	/// Used to give nodes and keys(attributes) a unique id of their own.
	static size_t internalId;

	/// Frees memory of vectors and root.
	/// 
	/// As std::vector::clear() doesn't automatically delete the memory a pointer is pointing at, we have to do so ourselves.
	void freeMemory();

	/// aoti() imitation.
	/// 
	/// Return all number in string str in order of left to right as a size_t value.
	size_t to_size_t(std::string str) const;

	/// Returns index of command in commandLines.
	/// 
	/// Used to get an index if command[9] matches a command from commandLines in order to determine if user
	/// input is valid command and a function has to be called to do something.
	short getIndexOfCommand(const char command[9]);

	/// Gets text after a command the user has inputed.
	/// 
	/// Gets the symbols from userInput after a certain position to end of input and places them in fileNameParam 
	/// returning them with the reference to the place the function was called.
	void getFileName(std::string& fileNameParam, const char* userInput, short commandSize) const;

	/// Gets text after a xml specific command the user has inputed.
	/// 
	/// Gets the symbols from userInput after a certain position to end of input and places them in xmlInfo 
	/// returning them with the reference to the place the function was called.
	void getXMLInfo(std::string& xmlInfo, const char* userInput, short commandSize) const;

	/// Makes a list of pointers to nodes.
	/// 
	/// Goes through the tree recursively adding each nodes pointer to listOfNodesParam. At the end we have pointers to
	/// all the nodes in our tree structure, so we can work easier with them. Introduces some problems but they are not that
	/// difficult to deal with.
	void getNodesDFS(Node* currentNode, std::vector <Node*>& listOfNodesParam);

	/// Opens a file.
	/// 
	/// Checks if a file has already been opened and if not, by a given name tries to open file to read from. If the file
	/// doesn't exist, it creates one and sets it as the file we are working with.
	void open(bool& fileOpenParam, const std::string fileNameParam);

	/// Save file.
	/// 
	/// Checks if a file has already been opened and if true, tries to open file to write to. If we pass the file name with
	/// which we are working with, it serves as the save command. If we pass a new file name, it serves as the saveas command.
	void save(const bool fileOpenParam, const std::string fileNameParam, bool command = true) const;

	/// Closes file.
	/// 
	/// Check if we have an open file and if true, sets fileOpenParam to false and deletes the root field with ~Node().
	void closeFile(bool& fileOpenParam, const std::string fileNameParam);

	/// Prints all supported commands.
	void printHelp() const;

	// the functions bellow have comments in the cpp file so i haven't explained them here

	/// Prints attribute value by id and key.
	/// 
	/// 
	void select(std::string xmlInfo) const;

	/// Sets a value to an attribute.
	/// 
	/// 
	void set(std::string xmlInfo, bool& changesMade);

	/// Prints values of attribute.
	/// 
	/// 
	void children(std::string xmlInfo) const;

	/// Access to an elements n-th child.
	/// 
	/// 
	void child(std::string xmlInfo) const;

	/// Access to an elements text.
	/// 
	/// 
	void text(std::string xmlInfo) const;

	/// Deletes an attribute of an element.
	/// 
	/// 
	void deleteIdKey(std::string xmlInfo, bool& changesMade);

	/// Adds a new child to element.
	/// 
	/// 
	void newchild(std::string xmlInfo, bool& changesMade); // add new child to a node

	/// Adds a new attribute to a node.
	/// 
	/// Custom command not in xml specification from project as my structure is a little different from the standard xml.
	/// Without this we can't add keys(attributes) to the tree as my code makes a distinction between the two and
	/// doesn't treat attributes as nodes. I realized this at the end and it was too much to change in the last day.
	void newattr(std::string xmlInfo, bool& changesMade); // add new attribute to a node <...>...</...>
};