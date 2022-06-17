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

const void Node::setKey(std::string buffer, char symbol)
{
	key = buffer.substr(0, buffer.find(symbol));
}

void Node::printIndent(std::ostream& os, short offSet) const
{
	for (size_t i = 0; i < depth + offSet; i++)
		os << "  ";
}

std::ostream& operator << (std::ostream& os, const Node& source)
{
	source.printIndent(os);
	os << "<" << source.key;

	//if (source.id != "")
		os << " id=\"" << source.id << "\">" << std::endl;

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

std::istream& operator >> (std::istream& is, Node& source)
{
	std::string buffer;
	std::string key;
	std::string current;
	
	//while (is) // while file hasn't ended
	{
		getline(is, buffer, '<');	// we get rid of the opening '<', as each line starts with one
		getline(is, buffer, '\n');	// get the rest of the line in the document

		if (buffer.find('<') != -1) // if we find another opening '<', then that line is an element/attribute
		{
			current = buffer.substr(0, buffer.find('>'));
			//source.addElement(current, );
		}

		// we assume buffer is an <entry> line so:
		if (buffer.find("id") != -1)	// we check if it has id and if true:
		{
			source.setKey(buffer, ' ');	// we set the key of current node before we get id 

			size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the pos of the opening "
			current = buffer.substr(index, buffer.find('\"', index) - index);	// a little math to find length of id
			source.id = current;									// set id 
		}
		else
			source.setKey(buffer, '>');

		if (buffer.find('<', 1))
		{

		}
		std::cout << buffer << std::endl;
		std::cout << current << std::endl;

		//if (buffer.find('/') != -1)

		//source.addNode("!!!");
		// current node or element ended
	}

	return is;
}
