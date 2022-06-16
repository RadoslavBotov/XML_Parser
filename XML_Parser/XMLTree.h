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
	static std::vector <std::string> commandLines;

	short getIndexOfCommand(const char command[]);
	void getFileName(std::string& fileNameParam, const char* userInput, short commandSize) const;
	void getXMLInfo(std::string& xmlInfo, const char* userInput, short commandSize) const;

	void open(bool& fileOpenParam, const std::string fileNameParam);
	void save(const bool fileOpenParam, const std::string fileNameParam, bool command = true) const;
	void closeFile(bool& fileOpenParam, const std::string fileNameParam);
	void printHelp() const;
};