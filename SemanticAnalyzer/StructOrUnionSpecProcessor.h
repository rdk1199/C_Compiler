#pragma once

#include "SymbolTable.h"
#include "ConstantExpressionEvaluator.h"
#include "C_Compiler/C_Grammar_Macros.h"

class StructOrUnionSpecProcessor
{
	friend class DeclarationProcessor;

private:

	static void process_struct_or_union_specifier(ParseTreeNode* specifier_node, SymbolTable& symbol_table);

};

