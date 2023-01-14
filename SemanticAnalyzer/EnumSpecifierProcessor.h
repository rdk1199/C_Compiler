#pragma once

#include "SymbolTable.h"
#include "ConstantExpressionEvaluator.h"
#include "C_Compiler/C_Grammar_Macros.h"

class EnumSpecifierProcessor
{
	friend class DeclarationProcessor;

private:

	static void process_enum_specifier(ParseTreeNode* enum_specifier_node, SymbolTable& symbol_table);
	static int process_enumerator_list(ParseTreeNode* enumerator_list_node, SymbolTable& symbol_table, EnumTypeNode* enum_type_node, TypeTableEntry current_t_table_entry);

};

