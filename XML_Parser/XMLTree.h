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

	// Open
	// Close
	// Save As
	

private:
	Node* root;
	static const char commandLines[6][7];

	short getIndexOfCommand(const char userInput[128]);
	void printHelp() const;
	void save(std::string fileNameParam) const;
};