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
	
	std::string& getId();
	std::string& getKey();
	Node* getChild(const size_t index) const;
	Element* getElement(const size_t index) const;
	const Node* getParent() const;
	unsigned getDepth() const;

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