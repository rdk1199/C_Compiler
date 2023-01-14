#include "EnumSpecifierProcessor.h"
#include "SymbolTable.h"
#include "SemanticAnalyzerError.h"

using std::string;
using std::vector;
using std::map;

void EnumSpecifierProcessor::process_enum_specifier(ParseTreeNode* enum_specifier_node, SymbolTable& symbol_table) //what should this return?
{
	if (enum_specifier_node->symbol != __ENUM_SPECIFIER__)
	{
		throw InternalCallError();
	}

	if (enum_specifier_node->children.size() < 2)
	{
		throw InvalidParseTree();
	}

	if (enum_specifier_node->children.size() == 2) //6.7.2.3-3
	{
		string enum_tag = enum_specifier_node->children[1]->lexeme;

		TypeTableEntry t_table_entry = symbol_table.find_type(enum_tag);

		if (t_table_entry.is_null || t_table_entry.type_tree.get_base_type() != TypeCategory::_enum)
		{
			throw IncompleteEnumDeclaration();
		}

		return;
	}

	//create new enum type node and type table entry

	EnumTypeNode* enum_type_node = new EnumTypeNode({});

	enum_type_node->complete = false;

	TypeTree enum_type_tree(enum_type_node);

	TypeTableEntry t_table_entry(enum_type_tree);

	int enumerator_list_index = -1;

	if (enum_specifier_node->children[1]->symbol == __IDENTIFIER__)
	{
		string enum_tag = enum_specifier_node->children[1]->lexeme;

		symbol_table.insert_tag_symbol(enum_tag, t_table_entry);

		enumerator_list_index = 3;

	}

	else
	{
		symbol_table.insert_tag_symbol("", t_table_entry);

		enumerator_list_index = 2;
	}

	process_enumerator_list(enum_specifier_node->children[enumerator_list_index], symbol_table, enum_type_node, t_table_entry);

	enum_type_node->complete = true;
}

int EnumSpecifierProcessor::process_enumerator_list(ParseTreeNode* enumerator_list_node, SymbolTable& symbol_table, EnumTypeNode* enum_type_node, TypeTableEntry current_t_table_entry)
{
	if (enumerator_list_node->symbol != __ENUMERATOR_LIST__)
	{
		throw InternalCallError();
	}

	int prev = -1;

	if (enumerator_list_node->children.size() > 1)
	{
		prev = process_enumerator_list(enumerator_list_node->children[0], symbol_table, enum_type_node, current_t_table_entry);
	}

	ParseTreeNode* enumerator_node = enumerator_list_node->children.back();

	if (enumerator_node->symbol != __ENUMERATOR__ || enumerator_node->children.size() == 0)
	{
		throw InvalidParseTree();
	}

	ParseTreeNode* enumeration_constant_node = enumerator_node->children[0];

	if (enumeration_constant_node->symbol != __IDENTIFIER__)
	{
		throw InvalidParseTree();
	}

	string enum_constant_tag = enumeration_constant_node->lexeme;

	if (symbol_table.check_identifier(enum_constant_tag))
	{
		throw RedeclaredIdentifier();
	}

	//ADD NEW ENUM ID TO SYMBOL TABLE

	int value = prev + 1;

	if (enumerator_node->children.size() == 3)
	{
		value = ConstantExpressionEvaluator::evaluate_integer_constant_expression(enumerator_node->children.back(), symbol_table);
	}

	enum_type_node->values.insert(std::pair<string, int>{enum_constant_tag, value});

	return value;
}

	