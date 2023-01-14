#pragma once

#include <set>
#include <vector> 
#include <map>
#include <string>

class NFA;

class DFA {

private:

	int num_states;
	std::vector<std::map<char, int>> trans_table;
	std::set<int> acc_states;
	std::set<char> alphabet;
	std::map<int, int> categorizer;

	std::vector<std::set<int>> split(const std::set<int>& s, const std::set<std::set<int>>& P);
	std::vector<std::set<int>> split_acc_states();

public:

	DFA();
	DFA(const std::set<char>& alpha, const std::vector<std::map<char, int>>& t_table, const std::set<int>& a_states);
	DFA(const std::set<char>& alpha, const std::vector<std::map<char, int>>& t_table, const std::set<int>& a_states, const std::map<int, int>& categories);
	int accepts(const std::string& str) const; //returns category, or -1 if string is invalid
	bool accepts(int state) const {return acc_states.count(state) > 0; }
	std::set<char> const get_alphabet() { return alphabet; }
	std::vector<std::map<char, int>> const get_trans_table() { return trans_table; }
	int const get_size();
	std::set<int> const get_acc_states();
	void const print_transition_table();
	void minimize();
	NFA to_NFA();
	bool const accept_test(const std::vector<std::string>& strs,const std::vector<bool>& expected, std::string test_name);
	bool const categorize_test(const std::vector<std::string>& strs, const std::vector<int>& expected, std::string test_name);
	std::map<int, int> const get_categorizer() { return categorizer; }
	std::map<char, int> operator[](int i) const { return trans_table[i]; }
	int categorize(int i) const;
	void print_acc_states();
	void remove_dead_states();
	void write_to_file(std::string file_name) const;

};

class DFACategoryConstructionError : public std::exception
{
	virtual const char* what() const throw()
	{
		return "DFA construction failed: invalid categories";
	}
};

class InvalidDFATest : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Invalid DFA test attempted: length of string list must match length of expected outputs";
	}
};