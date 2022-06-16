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

void Node::addElement(const std::string name, const std::string contents)
{
	elements.push_back(new Element(name, contents));
}

void Node::print(std::ostream& os, bool printChildren) const
{
	printIndent(os);
	os << "<" << key << ">" << std::endl;

	for (Element* el : elements)
	{
		printIndent(os, 1);
		os << "<" << el->name << ">" << el->contents << "</" << el->name << ">" << std::endl;
	}

	if (printChildren)
		for (Node* child : children)
			child->print(os);

	printIndent(os);
	os << "</" << key << ">" << std::endl;
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

void Node::printIndent(std::ostream& os, short offSet) const
{
	for (size_t i = 0; i < depth + offSet; i++)
		os << "  ";
}

std::ostream& operator << (std::ostream& os, const Node& source)
{
	source.printIndent(os);
	os << "<" << source.key << ">" << std::endl;

	for (Element* el : source.elements)
	{
		source.printIndent(os, 1);
		os << "<" << el->name << ">" << el->contents << "</" << el->name << ">" << std::endl;
	}

	for (Node* child : source.children)
		os << *child;

	source.printIndent(os);
	os << "</" << source.key << ">" << std::endl;

	return os;
}