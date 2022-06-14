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
	static const char commandLines[6][7];

	short getIndexOfCommand(const char userInput[128]);
	bool getFileName(std::string& fileNameParam, const char* userInput, const char* command, short commandSize, short& indexParam) const;

	bool open(const std::string fileNameParam);
	void save(bool fileOpenParam, const std::string fileNameParam, bool command = true) const;
	void closeFile(bool& fileOpenParam, const std::string fileNameParam);
	void printHelp() const;
};