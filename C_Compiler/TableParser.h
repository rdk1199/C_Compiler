#pragma once

#include "ContextFreeGrammar.h"

extern const std::set<int> table_grammar_terminals;
extern const std::set<int> table_grammar_terminals;
extern const std::multimap<int, std::vector<int>> table_grammar_productions;
extern int table_start; 
extern ContextFreeGrammar table_grammar; 

extern std::pair<std::map<int, std::map<int, Action>>, std::map<int, std::map<int, int>>> get_tables_from_file(std::string file_name);

class TableParseError : public std::exception
{

	virtual const char* what() const throw()
	{
		return "table parse error";
	}

};

