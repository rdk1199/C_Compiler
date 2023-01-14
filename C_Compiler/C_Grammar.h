#pragma once

#include "Scanner.h"
#include "ContextFreeGrammar.h"


extern std::set<int> C_terminals;
extern std::set<int> C_nonterminals;
extern std::multimap<int, std::vector<int>> C_productions;
extern int C_start;
//extern map<int, map<int, Action>> C_actions;
//extern map<int, map<int, int>> C_go_to;
extern ContextFreeGrammar C_grammar;
