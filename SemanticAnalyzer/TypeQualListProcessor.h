
#pragma once

#include "SymbolTable.h"
#include "ConstantExpressionEvaluator.h"
#include "C_Compiler/C_Grammar_Macros.h"
#include "SemanticAnalyzerError.h"


class TypeQualListProcessor
{
	static std::set<TypeQualifier> process_type_qualifier_list(ParseTreeNode* type_qual_list_node);
	static std::set<TypeQualifier> get_type_qualifiers(ParseTreeNode* type_qual_node, std::set<TypeQualifier>& qual_set);
};

