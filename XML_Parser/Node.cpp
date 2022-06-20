#include "Node.h"
#include<iostream>

Node::Node(std::string nameParam, Node* parentParam, std::string idParam)
{
	name = nameParam;
	parent = parentParam;

	if (parent == nullptr && idParam == "") // parent is nullptr only in root, so we check if we have a custom user id
		id = "root_id";	// if not we set a default id
	else // if parent isn't nullptr
		if (idParam != "") // we check if we have a custom id
			id = idParam;
		else id = "blank_id";	// else we set a default id

	(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;

	// children and elements are vectors
}

Node::Node(const Node& source)
{
	name = source.name;
	parent = source.parent;

	if (parent == nullptr && source.id == "") // parent is nullptr only in root, so we check if we have a custom user id
		id = "root_id";	// if not we set a default id
	else // if parent isn't nullptr
		if (source.id != "") // we check if we have a custom id
			id = source.id;
		else id = "blank_id";	// else we set a default id

	(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;
	
	for (Node* child : source.children)
		children.push_back(new Node(*child));

	for (Key* el : source.keys)
		keys.push_back(new Key(*el));
}

Node& Node::operator = (const Node& source)
{
	if (this != &source)
	{
		freeMemory();

		name = source.name;
		parent = source.parent;

		if (parent == nullptr && source.id == "") // parent is nullptr only in root, so we check if we have a custom user id
			id = "root_id";	// if not we set a default id
		else // if parent isn't nullptr
			if (source.id != "") // we check if we have a custom id
				id = source.id;
			else id = "blank_id";	// else we set a default id

		(parent == nullptr) ? depth = 0 : depth = parent->depth + 1;

		for (Node* child : source.children)
			children.push_back(new Node(*child));

		for (Key* el : source.keys)
			keys.push_back(new Key(*el));
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
	children.push_back(new Node(source.name, this, source.id));
}

void Node::addElement(const std::string name, const std::string value, const std::string id)
{
	keys.push_back(new Key(name, value, id));
}

void Node::freeMemory()
{
	id = "0";
	name = "";
	parent = nullptr;

	for (Node* child : children)
		delete child;
	children.clear();

	for (Key* el : keys)
		delete el;
	keys.clear();
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
	os << "<" << source.name << " id=\"" << source.id << "\">" << std::endl;

	for (Key* key : source.keys)
	{
		source.printIndent(os, 1);
		os << *key << std::endl;
	}

	for (Node* child : source.children)
		os << *child;

	source.printIndent(os);
	os << "</" << source.name << ">" << std::endl;

	return os;
}

std::istream& operator >> (std::istream& is, Node& source)
{
	static bool inRoot = true;	// not the best way to read root node but it works
	size_t indexOfChild = 0;	// as a deep copy of node is used in addNode, we need a way to track with which child we are working
	std::string buffer;			// we first read from file in buffer and from there we check some things and act accordingly
	std::string helper1;		// helper string when we want to get the text and id for a node
	std::string helper2;

	// there are 3 types of lines we get in buffer:
	// * <entry> - the name of the node
	//	if is has id some additional check are made and id of node is set, otherwise we give it an unique id of our own
	// * <element>text</...> - element name and text
	// * </entry> - ending of current node and we have to go up a level in the tree

	// code only for root node of tree, not the best way but it works
	if (inRoot)	// it executes only once, on the first (is >> *root)
	{
		inRoot = false;

		getline(is, buffer, '<');	// we get rid of the opening '<', as each line starts with one
		getline(is, buffer, '\n');	// get the rest of the line in the document
		if (buffer.find("id") != -1)	// we check if it has id and if true:
		{
			source.name = source.getKey(buffer, ' ');	// we set the key of current node before we get id 

			size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the position of the opening \"
			helper1 = buffer.substr(index, buffer.find('\"', index) - index);	// a little math to find length of id
			source.id = helper1;									// set id 
		}
		else
			source.name = source.getKey(buffer, '>');
	}

	while (is) // while file hasn't ended
	{
		helper1 = "";
		helper2 = "";

		getline(is, buffer, '<');	// we get rid of the opening '<', as each line starts with one
		getline(is, buffer, '\n');	// get the rest of the line in the document

		if (buffer.find('<') == -1 && buffer.find('/') != -1) // something to serve as floor of operator >>
			if (source.parent != nullptr) // if we are in root of tree we don't want to return but break from while
				return is;				  // so we can read file to end and not corrupt it or something
			else
				break; // we break only when buffer contains "</" or we go up a level in the tree

		if (buffer.find('<') != -1) // if we find another opening '<', then that line is an attribute(key)
		{											// < name id = "11" > ALL_PEOPLE< / name>
			helper2 = source.getKey(buffer, '>');
			helper1 = buffer.substr(buffer.find('>') + 1, buffer.find('<') - helper2.length() - 1);
			
			if (helper2.find("id") != -1)	// we check if it has id and if true:
			{
				size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the pos of the opening "
				std::string idParam = buffer.substr(index, buffer.find('\"', index) - index);
				
				source.addElement(helper2.substr(0, buffer.find(' ')), helper1, idParam);
			}
			else
				source.addElement(helper2, helper1, "");
		}
		else
		{   // we assume buffer is an <entry> line so:
			if (buffer.find("id") != -1)	// we check if it has id and if true:
			{
				helper1 = source.getKey(buffer, ' ');	// we set the key of current node before we get id 

				size_t index = buffer.find('\"') + 1;	// as we know there is an id, we get the pos of the opening "
				helper2 = buffer.substr(index, buffer.find('\"', index) - index);	// a little math to find length of id									// set id 
			}
			else
				helper1 = source.getKey(buffer, '>');

			source.addNode(Node(helper1, &source, helper2)); // add a new node
			
			is >> *source.children[indexOfChild++]; // we go down a level in the tree
		}
	}

	while (getline(is, buffer)); // read any leftover symbols in file if there are any

	return is;
}
