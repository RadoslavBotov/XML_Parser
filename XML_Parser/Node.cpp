#include "Node.h"
#include<iostream>

Node::Node()
{
	parent = nullptr;

	for (Node* child : children)
		child = nullptr;
}

Node::Node(std::string keyParam, const Node* parentParam)
{
	key = keyParam;
	parent = parentParam;
}

Node::~Node()
{
	freeMemory();
}

void Node::print() const
{
	std::cout << "Key: " << key << std::endl;
	std::cout << "Parent: " << &parent << std::endl;
	std::cout << "Children(size): " << children.size() << std::endl;
}

void Node::freeMemory()
{
	key = "";

	parent = nullptr;

	for (Node* child : children)
		delete child;
}
