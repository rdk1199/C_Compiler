#pragma once

#include <set>
#include <vector> 
#include <map>
#include <string>

class DFA;

class NFA {
private:
	int num_states; 
	std::set<char> alphabet;
	std::vector<std::map<char, std::set<int>>> trans_table;
	std::set<int> acc_states; 
	std::vector<std::set<int>> epsilon_closures; 
	std::map<int, int> categorizer;

	bool epsilon_closures_found = false;

	void calculate_epsilon_closures();
	std::set<int> epsilon_closure(int state);
	std::set<int> epsilon_closure(std::set<int> states);
	
	std::set<int> trans_table_union(std::set<int> states, char c);

	DFA const subset_construct_DFA();

public:
	NFA(const std::set<char>& alpha, const std::vector<std::map<char, std::set<int>>>& t_table, const std::set<int>& a_states);
	NFA(const std::set<char>& alpha, const std::vector<std::map<char, std::set<int>>>& t_table, const std::set<int>& a_states, const std::map<int, int>& categories);
	NFA(const std::set<char>& alpha, char c);
	NFA(const std::set<char>& alpha, const std::set<char>& chars);
	NFA(const std::set<char>& alpha, const std::string& s);
	NFA(const std::set<char>& alpha, const std::set<std::string>& strs);

	static DFA subset_construct_DFA(NFA& nfa);
	void print_epsilon_closures();
	std::set<int> get_acc_states() const {return acc_states; }
	std::vector<std::map<char, std::set<int>>> get_trans_table() const {return trans_table; }
	std::set<char> get_alphabet() const { return alphabet; }
	int get_size() const { return num_states; }
	static NFA kleene_closure(const NFA& nfa); 
	void const print_trans_table();
	static DFA combine_categorize(const std::vector<NFA>& nfas, const std::vector<int>& categories);
	void merge_acc_states();
	static NFA multi_union(const std::vector<NFA>& terms);
    static NFA multi_concat(const std::vector<NFA>& terms);
	static NFA multi_concat(const NFA& nfa, int n);
	static NFA complement(const std::set<char>& alpha, const std::set<char>& letters);
};

NFA operator+(const NFA& a, const NFA& b); //Thompson's construction union
NFA operator*(const NFA& a, const NFA& b); //Thompson's construction concatenation


class NFAEpsilonClosureOutOfBounds : std::exception
{
	virtual const char* what() const throw()
	{
		return "NFA Epsilon Closure out of bounds access";
	}
};

class NFAConstructionAcceptingStatesError : std::exception
{
	virtual const char* what() const throw()
	{
		return "NFA construction failed: invalid accepting state(s) detected";
	}
};

class IllegalNFAOperation : std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Illegal NFA operation attempted";
	}
};

class NFACategoryConstructionError : public std::exception
{
	virtual const char* what() const throw()
	{
		return "NFA construction failed: invalid categories";
	}
};

