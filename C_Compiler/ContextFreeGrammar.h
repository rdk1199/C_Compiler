#pragma once

#include <set>
#include <string>
#include <map>
#include <vector>

#include "ParseTree.h"
#include "Scanner.h"

struct LR1Item;
struct Action;

class ContextFreeGrammar
{
private:

	const int unique_start = -100; //used for LR(1) parsing
	const int epsilon = -1000;
	const int sentinel = -999;

	int num_symbols; //number of symbols (not including empty terminal)
	std::set<int> NT; //nonterminals 
	std::set<int> T; //terminals
	std::multimap<int, std::vector<int>> P; //productions
	
	int true_start; //must be a nonterminal

	std::map<int, std::set<int>> first_sets; 
	std::map<int, std::set<int>> follow_sets;
	std::map<std::pair<int, std::vector<int>>, std::set<int>> first_plus_sets;
	


	//map<std::vector<int>, set<int>> first_set_lookup_table;
	std::map<std::set<LR1Item>, std::set<LR1Item>> closure_lookup_table;

	int symbol_max; 

	void calculate_first_sets();
	void calculate_follow_sets();
	void calculate_first_plus_sets();
	
	std::set<int> first(const std::vector<int>& beta);
	std::set<int> first(const std::vector<int>& beta, int terminal);

	// LL(1) stuff
	bool backtrack_free = false;
	std::map<int, std::map<int, std::vector<int>>> LL_1_table;

	
	void eliminate_left_recursion();
	void eliminate_direct_left_recursion();
	void eliminate_indirect_left_recursion();
	void left_factor();
	bool construct_LL_1_table();
	
	//LR(1) stuff

	std::map<int, std::map<int, Action>> action_table;
	std::map<int, std::map<int, int>> go_to_table;

	std::vector<std::set<LR1Item>> canonical_collection;

	std::set<LR1Item> closure(std::set<LR1Item> items);
	std::set<LR1Item> go_to(std::set<LR1Item> lr_items, int symbol);

	void set_go_to(int start_state, int nonterminal, int end_state);
	void set_action(int start_state, int terminal, Action action);

	void calculate_CC();
	

public: 
	ContextFreeGrammar(std::set<int> terminals, std::set<int> nonterminals, std::multimap<int, std::vector<int>> productions, int _start);
	ContextFreeGrammar(std::set<int> terminals, std::set<int> nonterminals, std::multimap<int, std::vector<int>> productions, int _start, std::map<int, std::map<int, Action>> actions, std::map<int, std::map<int, int>> go_to);
	bool is_backtrack_free() { return backtrack_free; }
	std::map<int, std::map<int, std::vector<int>>> get_LL_1_table() { return LL_1_table;  }
	int get_start() { return unique_start; }

	void LL1_prepare();
	void LR1_prepare();

	void set_actions(std::map<int, std::map<int, Action>> new_actions) { action_table = new_actions; }
	void set_go_to(std::map<int, std::map<int, int>> new_go_to) { go_to_table = new_go_to; }

	void calculate_all_sets();
	void print_first_sets();

	void print_CC();
	void print_actions();
	void print_go_to();

	void write_LR1_tables_to_file(std::string file_name) const;

	ParseTree LL1_parse(std::vector<Word> token_string);
	ParseTree LR1_parse(std::vector<Word> token_string) const;

	ParseTree LR1_C_parse(std::vector<Word> token_string) const;
};

class CFGConstructionError : std::exception 
{
	virtual const char* what() const throw()
	{
		return "Attempted to construct invalid context free grammar";
	}
};

class InvalidFirstSet : std::exception
{
	virtual const char* what() const throw()
	{
		return "Attempted to calculate first set of empty vector";
	}
};

class EmptyParserInput :std::exception
{
	virtual const char* what() const throw()
	{
		return "Attempted to parse an empty token string";
	}
};

class MissingEOFMarker : std::exception
{
	virtual const char* what() const throw()
	{
		return "Token string does not have EOF token at end";
	}
};

class ParseError : std::exception
{
	virtual const char* what() const throw()
	{
		return "Parse Error: Program is invalid";
	}
};

class TableConstructionError : std::exception
{
	virtual const char* what() const throw()
	{
		return "Table construction error";
	}
};

struct LR1Item {

	int left;
	std::vector<int> right;
	int pos; //0 to right.size(); dot is before element at index pos
	int lookahead;

	LR1Item(std::pair<int, std::vector<int>> production, int position, int look) :
		left(production.first),
		right(production.second),
		pos(position),
		lookahead(look)
	{}

	LR1Item(int first, std::vector<int> second, int position, int look) :
		left(first),
		right(second),
		pos(position),
		lookahead(look)
	{}

	void print()
	{
		std::cout << left << " -> ";

		for (int i = 0; i < right.size(); i++)
		{
			if (i == pos)
			{
				std::cout << " | " << right[i] << " ";
			}

			else
			{
				std::cout << right[i] << " ";
			}
		}

		if (pos == right.size())
		{
			std::cout << " | ";
		}

		std::cout << "; lookahead: " << lookahead << std::endl;
	}
};

struct CCSet {

	std::set<LR1Item> item_set;
	int index = -1;
	bool marked = false; 

	CCSet(std::set<LR1Item> items, int idx) :
		index(idx),
		marked(false),
		item_set(items)
	{}
};

struct Action {

	enum class action {_shift, _reduce, _accept, _null};
	action action_type = action::_null;

	//shift values

	int to_state = -1; 

	// reduce values

	int left = -1;
	std::vector<int> right = {};

	//shift constructor
	Action(int state) :
		to_state(state),
		action_type(action::_shift)
	{}

	//reduce constructor
	Action(int first, std::vector<int> second) :
		left(first),
		right(second),
		action_type(action::_reduce)
	{}

	//accept constructor
	Action(bool accept) :
		action_type(accept ? action::_accept : action::_null)
	{}

	
	Action() :
		action_type(action::_null)
	{}
	

	std::map<action, std::string> action_names = { {action::_shift, "shift"}, {action::_reduce, "reduce"}, {action::_accept, "accept"}, {action::_null, "null"}};
		
	void print()
	{
		std::cout << "action type: " << action_names[action_type] << ",";
		switch (action_type)
		{
		case(action::_shift):
			std::cout << " shift to " << to_state << " ";
			break;
		case(action::_reduce):
			std::cout << " production : " << left << "-> ";
			for (int i = 0; i < right.size(); i++)
			{
				std::cout << right[i] << " ";
			}
			break;
			//std::cout << endl;
		default:
			std::cout << "null";
			break;
		}
	}
};

bool operator==(const Action& first, const Action& second);

