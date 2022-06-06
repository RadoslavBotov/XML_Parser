#pragma once
#include<string>
#include<vector>
#include"Element.h"

class Node
{
public:
	//Node();
	Node(std::string keyParam, const Node* parentParam);
	Node(const Node& source);
	Node& operator = (const Node& source);
	~Node();
	
	const Node* getParent() const;
	unsigned getDepth() const;
	std::string& accessId();
	std::string& accessKey();
	Node* accessChild(const size_t index) const;
	Element* accessElement(const size_t index) const;

	void addNode(std::string keyParam);

	void print() const;

private:
	std::string id;
	std::string key;
	std::vector <Node*> children;
	std::vector <Element*> elements;

	const Node* parent;
	unsigned depth;

	void printIndent() const;
	void freeMemory();
};