#include "ConstantExpressionEvaluator.h"
#include "C_Compiler/C_Grammar_Macros.h"

int ConstantExpressionEvaluator::evaluate_integer_constant_expression(ParseTreeNode* constant_expression_node, SymbolTable& symbol_table)
{
	if (constant_expression_node->symbol != __CONSTANT_EXPRESSION__)
	{
		throw ConstExpressionError();
	}

	return 0;
}