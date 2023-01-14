#pragma once

#include "ParseTree.h"

class DeclarationProcessor
{


public: 
	static std::set<std::string> process_declaration_prefix(ParseTreeNode* declaration_prefix_node);

};

