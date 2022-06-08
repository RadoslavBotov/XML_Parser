#include "Node.h"
#include<iostream>

//Node::Node()
//{
//	id = "root_id";
//	key = "root_key";
//	parent = nullptr;
//	depth = 0;
//}

Node::Node(std::string keyParam, const Node* parentParam)
{
	key = keyParam;
	parent = parentParam;

	(parent == nullptr) ? id = "root_id" : id = parent->id + "_";
	(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;

	// we don't set capacity for children and elements
	// as they are vectors and auto resize
}

Node::Node(const Node& source)
{
	key = source.key;
	parent = source.parent;

	(parent == nullptr) ? id = "root_id" : id = parent->id + "_";
	(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;
	
	for (Node* child : source.children)
		children.push_back(new Node(*child));

	for (Element* el : source.elements)
		elements.push_back(new Element(*el));
}

Node& Node::operator = (const Node& source)
{
	if (this != &source)
	{
		freeMemory();

		key = source.key;
		parent = source.parent;

		(parent == nullptr) ? id = "root_id" : id = parent->id + "_";
		(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;

		for (Node* child : source.children)
			children.push_back(new Node(*child));

		for (Element* el : source.elements)
			elements.push_back(new Element(*el));
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
	newNode->depth = newNode->parent->depth + 1;

	children.push_back(newNode);
}

void Node::addNode(const Node& source)
{
	Node* newNode = new Node(source);
	newNode->parent = this;

	newNode->id = parent->id + "_";
	newNode->depth = newNode->parent->depth + 1;

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

void Node::freeMemory()
{
	id = "0";
	key = "";
	parent = nullptr;

	for (Node* child : children)
		delete child;
	children.clear();

	for (Element* el : elements)
		delete el;
	elements.clear();
}

void Node::printIndent() const
{
	for (size_t i = 0; i < depth; i++)
		std::cout << "    ";
}