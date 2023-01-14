#pragma once

#include <vector>
#include "Scanner.h"



struct ParseTreeNode
{
	std::string lexeme;
	
	int symbol;

	ParseTreeNode* parent;
	std::vector<ParseTreeNode*> children; 

	ParseTreeNode(std::string word, int grammar_symbol, ParseTreeNode* ancestor, std::vector<ParseTreeNode*> descendants) :
		lexeme(word),
		symbol(grammar_symbol),
		parent(ancestor),
		children(descendants)
	{}

	/*
	~ParseTreeNode()
	{
		for (int i = 0; i < children.size(); i++)
		{
			(*children[i]).~ParseTreeNode();
			delete children[i];
		}
	}
	*/

	void print()
	{
		if (children.size() == 0)
		{
			std::cout << "symbol: " << symbol << " word: "<<  lexeme;
		}

		else
		{
			std::cout << "symbol: " << symbol << "( ";
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->print();
			}
			std::cout << ")";
		}
	}

	void print_leaves()
	{
		if (children.size() == 0)
		{
			std::cout << "[symbol: " << symbol << " word: " << lexeme << "]";
		}

		else
		{
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->print_leaves();
			}
		}
	}
};

class ParseTree
{
private:
	ParseTreeNode& root;

public:
	ParseTree(ParseTreeNode& tree_root);
	ParseTreeNode& get_root() { return root; }
	void print();
	void print_leaves() { root.print_leaves(); }
	//~ParseTree();
};

