#include "ParseTree.h"

#include <iostream>

using std::cout;
using std::endl;

ParseTree::ParseTree(ParseTreeNode& tree_root) :
	root(tree_root)
{}

void ParseTree::print()
{
	root.print();
	cout << endl;
}

/*
ParseTree::~ParseTree()
{
	root.~ParseTreeNode();
}
*/
