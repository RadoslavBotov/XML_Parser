#pragma once
#include<string>
#include<vector>
#include"Key.h"

struct Node
{
public:
	//Node();
	/// Constructor with parameters.
	///
	/// Parameters for key, value and parent node pointer and id with default value "".
	Node(std::string nameParam, Node* parentParam, std::string idParam = "");

	/// Copy constructor.
	Node(const Node& source);

	/// Operator =.
	Node& operator = (const Node& source);

	/// Destructor.
	///
	/// Uses freeMemory() to clear.
	~Node();

	/// Add node.
	/// 
	/// Adds a node to children by creating a new node based on nameParam.
	void addNode(std::string nameParam);

	/// Add node.
	/// 
	/// Adds a node to children using copy constructor.
	void addNode(const Node& source);

	/// Add element.
	/// 
	/// Adds an element to keys by creating a new Element based on name, value and id.
	void addElement(const std::string name, const std::string value, const std::string id);

	/// Operator <<.
	friend std::ostream& operator << (std::ostream& os, const Node& source);

	/// Operator >>.
	///
	/// Reads from a file, creating a tree structure with a root source.
	friend std::istream& operator >> (std::istream& is, Node& source);

// variables / fields
	/// Unique id.
	std::string id;

	/// Name of node.
	std::string name;

	/// Children of node.
	///
	/// A vector of pointers to other Node objects acting as children to this->node;
	std::vector <Node*> children;

	/// Keys/Attributes.
	/// 
	/// A vector of pointers to Key objects acting as attributes of this->node;
	std::vector <Key*> keys;

	/// Parent pointer.
	/// 
	/// A pointer to the parent Node of this->node.
	Node* parent;

	/// Depth of node in tree structure.
	///
	/// A helper variable with which we determine the depth of this->node once and use for indention when writing to file.
	unsigned depth;

private: // internal functions
	/// Prints indent based on depth.
	/// 
	/// Prints indent based on this->depth to a gives os stream. OffSet is used to print an additionl 4 spaces
	/// when we are printing attributes as they don't have a depth of their own.
	void printIndent(std::ostream& os, short offSet = 0) const;

	/// Frees memory of vectors.
	/// 
	/// As std::vector::clear() doesn't automatically delete the memory a pointer is pointing at, we have to do so ourselves.
	void freeMemory();

	/// Get name of key.
	/// 
	/// Returns a substring of buffer from position 0 to the given symbol.
	const std::string getKey(std::string buffer, char symbol);
};