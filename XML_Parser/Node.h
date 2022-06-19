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
	/// Parameters for key value and parent node pointer.
	Node(std::string nameParam, const Node* parentParam);
	/// Copy constructor.
	/// 
	/// .
	Node(const Node& source);
	/// Operator =.
	/// 
	/// .
	Node& operator = (const Node& source);
	/// Destructor.
	///
	/// Uses freeMemory() to clear.
	~Node();

	void addNode(std::string nameParam);
	void addNode(const Node& source);
	void addElement(const std::string name, const std::string contents);

	friend std::ostream& operator << (std::ostream& os, const Node& source);
	friend std::istream& operator >> (std::istream& is, Node& source);

// variables / fields
	std::string id;
	std::string name;
	std::vector <Node*> children;
	std::vector <Key*> keys;

	const Node* parent;
	unsigned depth;

private: // internal functions
	void printIndent(std::ostream& os, short offSet = 0) const;
	void freeMemory();

	const std::string getKey(std::string buffer, char symbol);
	//std::string getKey(std::string& buffer) const;
};