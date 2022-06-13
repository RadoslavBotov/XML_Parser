#pragma once
#include<string>
#include<vector>
#include"Element.h"

struct Node
{
public:
	//Node();
	/// Constructor with parameters.
	///
	/// Parameters for key value and parent node pointer.
	Node(std::string keyParam, const Node* parentParam);
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

	void addNode(std::string keyParam);
	void addNode(const Node& source);
	void addElement(const std::string name, const std::string contents);

	void print(std::ostream& os) const;

	friend std::ostream& operator << (std::ostream& os, const Node& source);

// variables / fields
	std::string id;
	std::string key;
	std::vector <Node*> children;
	std::vector <Element*> elements;

	const Node* parent;
	unsigned depth;

private: // internal functions
	void printIndent(std::ostream& os, short offSet = 0) const;
	void freeMemory();
};