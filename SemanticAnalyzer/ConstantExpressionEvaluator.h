#pragma once

#include "C_Compiler/ParseTree.h"
#include "SymbolTable.h"

class ConstantExpressionEvaluator
{
public:
	static int evaluate_integer_constant_expression(ParseTreeNode* constant_expression_node, SymbolTable& symbol_table);
};

class ConstExpressionError : public std::exception
{
	virtual const char* what() const throw()
	{
		return "error in constant expression";
	}
};

class NonConstantExpression : public std::exception
{
	virtual const char* what() const throw()
	{
		return "expression given was not an integer constant expression";
	}
};