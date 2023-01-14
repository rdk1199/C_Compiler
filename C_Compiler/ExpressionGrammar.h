#pragma once

#include "Scanner.h"
#include "ContextFreeGrammar.h"

extern std::set<int> terminals;
extern std::set<int> nonterminals;
extern std::multimap<int, std::vector<int >> productions;
extern int start;
extern ContextFreeGrammar expression_grammar;

extern std::set<int> test_terminals;
extern std::set<int> test_nonterminals;
extern std::multimap<int, std::vector<int>> test_productions;
extern int test_start;
extern ContextFreeGrammar test_grammar;

int evaluate_expression_tree(ParseTreeNode* start_node);
int get_int_value(std::string int_string);
int get_char_value(std::string char_string);