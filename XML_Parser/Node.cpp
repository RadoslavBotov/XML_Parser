#include "Node.h"
#include<iostream>

Node::Node()
{
	id = "root_id";
	key = "root_key";
	parent = nullptr;
	depth = 0;
}

Node::Node(std::string keyParam, const Node* parentParam)
{
	static unsigned idParam = 1;

	key = keyParam;
	parent = parentParam;

	if (parent == nullptr)
		id = "root_id";
	else
	{
		std::string temp = std::to_string(idParam++);
		if (parent->id == "root_id")
			id = temp;
		else
			id = parent->id + "_" + temp;
	}
	
	(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;
}

Node::Node(const Node& source)
{
	static unsigned idParam = 1;

	key = source.key;
	parent = source.parent;

	id = source.id + "_" + std::to_string(idParam++);
	depth = source.depth;
	
	for (Node* child : source.children)
		children.push_back(new Node(*child));
}

Node& Node::operator = (const Node& source)
{
	if (this != &source)
	{
		freeMemory();

		key = source.key;
		parent = source.parent;

		id = source.id;
		depth = source.depth;

		for (Node* child : source.children)
			children.push_back(new Node(*child));
	}
	return *this;
}

Node::~Node()
{
	freeMemory();
}

void Node::addNode(std::string keyParam)
{
	Node* newNode = new Node(keyParam, this);
	children.push_back(newNode);
}

void Node::print() const
{
	printIndent();
	std::cout << "Key: " << key << " id=" << id << std::endl;

	printIndent();
	std::cout << "Parent: ";
	(parent != nullptr) ? std::cout << parent->key : std::cout << "nullptr";
	std::cout << std::endl;

	printIndent();
	std::cout << "Depth: " << depth << std::endl;

	for (Node* child : children)
		child->print();
}

void Node::testFunction()
{
	// treat as if main
	addNode("node 1");
	addNode("node 2");
	children[0]->addNode("node 1_1");
	children[0]->addNode("node 1_2");

	print();
}

void Node::freeMemory()
{
	id = "0";
	key = "";
	parent = nullptr;

	for (Node* child : children)
		delete child;
}

void Node::printIndent() const
{
	for (size_t i = 0; i < depth; i++)
		std::cout << "    ";
}