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

	void testFunction();

	// Open
	// Close
	// Save
	// Save As
	

private:
	Node* root;
	std::string fileName;
};