#include "DeclarationProcessor.h"
#include "ContextFreeGrammar.h"
#include "C_Grammar_Macros.h"

using std::set;
using std::string;
using std::map;

bool check_for_typedef(ParseTreeNode* declaration_specifiers_node);
void process_init_declarator_list(ParseTreeNode* init_declarator_list_node, set<string>& out_set);
set<string> process_declarator(ParseTreeNode* declarator_node);
set<string> process_direct_declarator(ParseTreeNode* direct_declarator_node);

set<string> DeclarationProcessor::process_declaration_prefix(ParseTreeNode* declaration_prefix_node)
{

	if (declaration_prefix_node->symbol != __DECLARATION_PREFIX__)
	{
		throw ParseError();
	}

	set<string> out = {};

	if (check_for_typedef(declaration_prefix_node->children[0]))
	{
		process_init_declarator_list(declaration_prefix_node->children[1], out);
	}

	return out;
}

bool check_for_typedef(ParseTreeNode* declaration_specifiers_node)
{
	if (declaration_specifiers_node->symbol != __DECLARATION_SPECIFIERS__)
	{
		throw ParseError();
	}

	bool typedef_child = ((declaration_specifiers_node->children[0]->symbol == __STORAGE_CLASS_SPECIFIER__) && (declaration_specifiers_node->children[0]->children[0]->symbol == __TYPEDEF__));

	if (declaration_specifiers_node->children.size() == 1)
	{
		return typedef_child;
	}

	else if (declaration_specifiers_node->children.size() == 2)
	{
		return typedef_child || check_for_typedef(declaration_specifiers_node->children[1]);
	}

	else
	{
		throw ParseError();
		return false; 
	}
}

void process_init_declarator_list(ParseTreeNode* init_declarator_list_node, set<string>& out_set)
{
	if (init_declarator_list_node->symbol != __INIT_DECLARATOR_LIST__)
	{
		throw ParseError();
	}

	if (init_declarator_list_node->children.size() == 3)
	{
		set<string> child_names = process_declarator(init_declarator_list_node->children[2]->children[0]);

		out_set.insert(child_names.begin(), child_names.end());

		process_init_declarator_list(init_declarator_list_node->children[0], out_set);
	}

	else if (init_declarator_list_node->children.size() == 1)
	{
		set<string> child_names = process_declarator(init_declarator_list_node->children[0]->children[0]);

		out_set.insert(child_names.begin(), child_names.end());
	}

	else
	{
		throw ParseError();
	}
}

set<string> process_declarator(ParseTreeNode* declarator_node)
{
	if (declarator_node->symbol != __DECLARATOR__)
	{
		throw ParseError();
	}

	if (declarator_node->children.size() == 1)
	{
		return process_direct_declarator(declarator_node->children[0]);
	}

	else if (declarator_node->children.size() == 2)
	{
		return process_direct_declarator(declarator_node->children[1]);
	}

	else
	{
		throw ParseError();

		return {};
	}

}

set<string> process_direct_declarator(ParseTreeNode* direct_declarator_node)
{
	if (direct_declarator_node->symbol != __DIRECT_DECLARATOR__)
	{
		throw ParseError();
	}

	if (direct_declarator_node->children[0]->symbol == __IDENTIFIER__)
	{
		return { direct_declarator_node->children[0]->lexeme };
	}

	else if (direct_declarator_node->children[1]->symbol == __DECLARATOR__)
	{
		return process_declarator(direct_declarator_node->children[1]);
	}

	else if (direct_declarator_node->children[0]->symbol == __DIRECT_DECLARATOR__)
	{
		return process_direct_declarator(direct_declarator_node->children[0]);
	}

	else
	{
		throw ParseError();
		return {};
	}
}