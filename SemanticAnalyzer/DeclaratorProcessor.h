#pragma once

#include "SymbolTable.h"
#include "ConstantExpressionEvaluator.h"
#include "C_Compiler/C_Grammar_Macros.h"
#include "SemanticAnalyzerError.h"

class DeclaratorProcessor
{

	static TypeNode* process_full_declarator(ParseTreeNode* declarator_node, SymbolTable& symbol_table);
	static TypeNode* process_nested_declarator(ParseTreeNode* declarator_node, SymbolTable& symbol_table);

	static TypeNode* process_direct_declarator(ParseTreeNode* direct_dec_node, SymbolTable& symbol_table);

	static TypeNode* process_array_declarator(ParseTreeNode* array_dec_node, SymbolTable& symbol_table);
	static TypeNode* process_function_declarator(ParseTreeNode* function_dec_node, SymbolTable& symbol_table);

};

