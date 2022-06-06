#include "XMLTree.h"

XMLTree::XMLTree()
{
	root = new Node("root", nullptr);
}

XMLTree::XMLTree(const XMLTree& source)
{
	root = new Node(*source.root);
}

XMLTree& XMLTree::operator=(const XMLTree& source)
{
	if (this != &source)
	{
		delete root;

		root = new Node(*source.root);
	}
	return *this;
}

XMLTree::~XMLTree()
{
	delete root;
}

void XMLTree::testFunction()
{
	// treat as if main
	root->addNode("node 1");
	root->addNode("node 2");
	root->getChild(0)->addNode("node 1_1");
	root->getChild(0)->addNode("node 1_2");

	root->print();
}