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
	bool save(const std::string fileNameParam) const;
	bool open(const std::string fileNameParam);
	bool getFileName(std::string& fileNameParam, const char* userInput, const char* command, short commandSize, short& indexParam) const;
};