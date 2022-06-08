#pragma once
#include<string>
#include<vector>
#include"Element.h"

struct Node
{
public:
	//Node();
	Node(std::string keyParam, const Node* parentParam);
	Node(const Node& source);
	Node& operator = (const Node& source);
	~Node();

	void addNode(std::string keyParam);

	void print() const;

// variables / fields
	std::string id;
	std::string key;
	std::vector <Node*> children;
	std::vector <Element*> elements;

	const Node* parent;
	unsigned depth;

private:
	void printIndent() const;
	void freeMemory();
};