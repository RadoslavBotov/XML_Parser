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

	newNode->id = ""; // default id
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

const std::string Node::getKey(std::string buffer, char symbol)
{
	return buffer.substr(0, buffer.find(symbol));
}

void Node::printIndent(std::ostream& os, short offSet) const
{
	for (size_t i = 0; i < depth + offSet; i++)
		os << "  ";
}

std::ostream& operator << (std::ostream& os, const Node& source)
{
	source.printIndent(os);
	os << "<" << source.key << " id=\"" << source.id << "\">" << std::endl;

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
	static bool inRoot = true;
	size_t indexOfChild = 0;
	std::string buffer;
	std::string current;
	std::string name;

	// there are 3 types of lines:
	// * <entry> - key of node, which we read and set to key of node
	//	if is has id some additional check are made and id of node is set, otherwise we give it an unique id of our own
	// * <element>text</...> - read element name and text and set accordingly then add to node 
	// * </end> - when we encounter one we return
	// we just have to know when and how to go up and down the tree - kind of recursively

	// code only for root node of tree
	if (inRoot)
	{
		inRoot = false;

		getline(is, buffer, '<');	// we get rid of the opening '<', as each line starts with one
		getline(is, buffer, '\n');	// get the rest of the line in the document
		if (buffer.find("id") != -1)	// we check if it has id and if true:
		{
			source.key = source.getKey(buffer, ' ');	// we set the key of current node before we get id 

			size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the pos of the opening "
			current = buffer.substr(index, buffer.find('\"', index) - index);	// a little math to find length of id
			source.id = current;									// set id 
		}
		else
			source.key = source.getKey(buffer, ' ');
	}

	while (is) // while file hasn't ended
	{
		getline(is, buffer, '<');	// we get rid of the opening '<', as each line starts with one
		getline(is, buffer, '\n');	// get the rest of the line in the document

		if (buffer.find('<') == -1 && buffer.find('/') != -1) // something to serve as floor of operator >>
			if (source.parent != nullptr) // if we are in root of tree we don't want to return but break from while
				return is;					  // so we can read file to end and not corrupt it or something
			else
				break;

		if (buffer.find('<') != -1) // if we find another opening '<', then that line is an element/attribute
		{
			name = source.getKey(buffer, '>');
			current = buffer.substr(buffer.find('>') + 1, buffer.find('<') - name.length() - 1);
			source.addElement(name, current);
		}
		else
		{   // we assume buffer is an <entry> line so:
			if (buffer.find("id") != -1)	// we check if it has id and if true:
			{
				current = source.getKey(buffer, ' ');	// we set the key of current node before we get id 

				size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the pos of the opening "
				name = buffer.substr(index, buffer.find('\"', index) - index);	// a little math to find length of id									// set id 
			}
			else
				current = source.getKey(buffer, ' ');

			Node child(current, &source); // either not make a deep copy or something else 
			child.id = name;
			source.addNode(child);
			is >> *source.children[indexOfChild++];
		}
	}

	while (getline(is, buffer)); // read any leftover symbols in file if there are any

	return is;
}
