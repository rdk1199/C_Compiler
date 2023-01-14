#pragma once

#include "C_Compiler/ParseTree.h"
#include "ASTree.h"

class SemanticAnalyzer
{
private:
	ParseTree source_tree;
	

public:
	ASTree build_AST();

};

