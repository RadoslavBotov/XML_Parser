#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"Node.h"

class XMLTree
{
public:
	XMLTree();
	XMLTree(const XMLTree& source);
	XMLTree& operator = (const XMLTree& source);
	~XMLTree();

	void runProgram();

private:
	Node* root;
	std::vector <Node*> listOfNodes; // we will use this to skip using DFS every time we need to change nodes of tree	
	
	static std::vector <std::string> commandLines;
	static size_t internalId;

	short getIndexOfCommand(const char command[9]);
	void getFileName(std::string& fileNameParam, const char* userInput, short commandSize) const;
	void getXMLInfo(std::string& xmlInfo, const char* userInput, short commandSize) const;
	void getNodesDFS(Node* currentNode);

	void open(bool& fileOpenParam, const std::string fileNameParam);
	void save(const bool fileOpenParam, const std::string fileNameParam, bool command = true) const;
	void closeFile(bool& fileOpenParam, const std::string fileNameParam);
	void printHelp() const;

	void select(std::string xmlInfo) const;
	void set(std::string xmlInfo, bool& changesMade) const;
	void children(std::string xmlInfo) const;
	void child(std::string xmlInfo) const;
	void text(std::string xmlInfo) const;
	void deleteIdKey(std::string xmlInfo, bool& changesMade) const;
	void newchild(std::string xmlInfo) const;
};