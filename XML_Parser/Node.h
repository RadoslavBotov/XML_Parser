#pragma once
#include<string>
#include<vector>

class Node
{
public:
	Node();
	Node(std::string keyParam, const Node* parentParam = nullptr);
	Node(const Node& source);
	Node& operator = (const Node& source);
	~Node();
	
	void addNode(std::string keyParam);

	void print() const;
	void testFunction();

private:
	std::string id;
	std::string key;
	const Node* parent;
	std::vector <Node*> children;
	unsigned depth;

	void printIndent() const;
	void freeMemory();
};