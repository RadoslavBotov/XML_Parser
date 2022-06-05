#pragma once
#include<string>
#include<vector>

class Node
{
public:
	Node();
	Node(std::string keyParam, const Node* parentParam);
	//
	//
	~Node();

	void print() const;

private:
	std::string key;
	Node const* parent;
	std::vector<Node*> children;

	void freeMemory();
};