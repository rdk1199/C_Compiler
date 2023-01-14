#include "DeclaratorProcessor.h"

TypeNode* DeclaratorProcessor::process_full_declarator(ParseTreeNode* declarator_node, SymbolTable& symbol_table)
{
	if (declarator_node->children.size() == 0 || declarator_node->children.size() > 2)
	{
		throw InvalidParseTree();
	}

	if (declarator_node->children.size() == 1) //no pointer
	{
		if (declarator_node->children[0]->symbol != __DIRECT_DECLARATOR__)
		{
			throw InvalidParseTree();
		}

		return process_direct_declarator(declarator_node->children[0], symbol_table);
	}

}

TypeNode* DeclaratorProcessor::process_nested_declarator(ParseTreeNode* declarator_node, SymbolTable& symbol_table)
{

	if (declarator_node->children.size() == 0 || declarator_node->children.size() > 2)
	{
		throw InvalidParseTree();
	}

	if (declarator_node->children.size() == 1) //no pointer
	{
		if (declarator_node->children[0]->symbol != __DIRECT_DECLARATOR__)
		{
			throw InvalidParseTree();
		}

		return process_direct_declarator(declarator_node->children[0], symbol_table);
	}



}

TypeNode* DeclaratorProcessor::process_direct_declarator(ParseTreeNode* direct_dec_node, SymbolTable& symbol_table)
{
	if (direct_dec_node->children.size() == 3 && direct_dec_node->children[1]->symbol == __DECLARATOR__)
	{
		return process_nested_declarator(direct_dec_node->children[1], symbol_table);
	}


}

TypeNode* DeclaratorProcessor::process_array_declarator(ParseTreeNode* array_dec_node, SymbolTable& symbol_table)
{

}

TypeNode* DeclaratorProcessor::process_function_declarator(ParseTreeNode* function_dec_node, SymbolTable& symbol_table)
{

}