#pragma once

#include <set>
#include <vector> 
#include <map>
#include <string>
#include "DFA.h"

class CompressedDFA
{
private:
	int num_states;
	std::map<char, int> char_cat; 
	std::vector<std::vector<int>> trans_table;
	std::set<int> acc_states;
	std::set<char> alphabet;
	std::map<int, int> categorizer;

	CompressedDFA(const std::set<char>& alph, const std::map<char, int>& c_cat, const std::vector<std::vector<int>>& t_table, const std::set<int>& a_states, const std::map<int, int>& categories);
	

public:
	static CompressedDFA compress(DFA dfa);
	int const accepts(const std::string& str);
	std::set<char> const get_alphabet() { return alphabet; }
	std::vector<std::vector<int>> const get_trans_table() { return trans_table; }
	int get_size() const { return num_states; };
	std::set<int> get_acc_states() const { return acc_states; };
	void const print_transition_table();
	bool const accept_test(const std::vector<std::string>& strs, const std::vector<bool>& expected);
	bool const categorize_test(const std::vector<std::string>& strs, const std::vector<int>& expected);
	
};

