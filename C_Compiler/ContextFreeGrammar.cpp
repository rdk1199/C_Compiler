

#include "ContextFreeGrammar.h"
#include "DeclarationProcessor.h"

#include <algorithm>
#include <iterator>
#include <stack>
#include <iostream>
#include <fstream>

#define __DECLARATION_PREFIX__ 177

using std::map;
using std::stack; 
using std::upper_bound;
using std::lower_bound;
using std::cout;
using std::endl;
using std::max;
using std::ofstream;
using std::multiset;
using std::multimap;
using std::vector;
using std::set;
using std::string; 

ContextFreeGrammar::ContextFreeGrammar(set<int> terminals, set<int> nonterminals, multimap<int, vector<int>> productions, int _start):
	num_symbols(terminals.size() + nonterminals.size()),
	NT(nonterminals),
	T(terminals),
	P(productions),
	true_start(_start)
{

	if (T.empty() || NT.empty())
	{
		throw CFGConstructionError();
	}
	//cout << productions.size();

	set<int> intersection;

	std::set_intersection(T.begin(), T.end(), NT.begin(), NT.end(), std::inserter(intersection ,intersection.end()));

	if (!intersection.empty() || NT.find(true_start) == NT.end())
	{
		throw CFGConstructionError(); //terminals and nonterminals must be disjoint, and start symbol must be a nonterminal;
	}

	symbol_max = max(*(T.rbegin()), *(NT.rbegin()));

	for (auto i = P.begin(); i != P.end(); i++)
	{
		if (NT.count((*i).first) == 0 || (*i).second.empty()) //if left is a nonterminal or right is empty, throw error
		{
			throw CFGConstructionError();
		}
	}

	T.insert(EOF);
	T.insert(epsilon);

	NT.insert(unique_start);
	P.insert(std::pair<int, vector<int>>(unique_start, { true_start }));
}

ContextFreeGrammar::ContextFreeGrammar(set<int> terminals, set<int> nonterminals, multimap<int, vector<int>> productions, int _start, map<int, map<int, Action>> actions, map<int, map<int, int>> go_to):
	num_symbols(terminals.size() + nonterminals.size()),
	NT(nonterminals),
	T(terminals),
	P(productions),
	true_start(_start),
	action_table(actions),
	go_to_table(go_to)
{

	if (T.empty() || NT.empty())
	{
		throw CFGConstructionError();
	}

	set<int> intersection;

	std::set_intersection(T.begin(), T.end(), NT.begin(), NT.end(), std::inserter(intersection, intersection.end()));

	if (!intersection.empty() || NT.find(true_start) == NT.end())
	{
		throw CFGConstructionError(); //terminals and nonterminals must be disjoint, and start symbol must be a nonterminal;
	}

	symbol_max = max(*(T.rbegin()), *(NT.rbegin()));

	for (auto i = P.begin(); i != P.end(); i++)
	{
		if (NT.count((*i).first) == 0 || (*i).second.empty()) //if left is a nonterminal or right is empty, throw error
		{
			throw CFGConstructionError();
		}
	}

	T.insert(EOF);
	T.insert(epsilon);

	NT.insert(unique_start);
	P.insert(std::pair<int, vector<int>>(unique_start, { true_start }));
}

void ContextFreeGrammar::eliminate_indirect_left_recursion()
{
	multimap<int, vector<int>> new_productions;

	for (auto i = NT.begin(); i !=  NT.end(); i++)
	{
		for (auto j = P.lower_bound(*i); j != P.upper_bound(*i); j++)
		{
			if (T.count((*j).second[0]) == 0 && (*j).second[0] < (*i)) //first term on right is < i
			{
				vector<int> new_right;

				for (auto k = P.lower_bound((*j).second[0]); k != P.upper_bound((*j).second[0]); k++)
				{
					for (int r = 0; r < (*k).second.size(); r++) //replace first term in right side of production with alternatives
					{
						new_right.push_back((*k).second[r]);
					}

					for (int r = 1; r < (*j).second.size(); r++) //put in remaining terms
					{
						new_right.push_back((*j).second[r]);
					}
				}
				new_productions.insert(std::pair<int, vector<int>>(*i, new_right));
			}

			else // just add rule to new productions without change
			{
				new_productions.insert(std::pair<int, vector<int>>(*i, (*j).second)); 
			}
		}
	}

	P = new_productions;
}

void ContextFreeGrammar::eliminate_direct_left_recursion()
{
	set<int> new_NT = NT;
	multimap<int, vector<int>> new_productions;

	for (auto i = NT.begin(); i != NT.end(); i++)
	{
		bool left_recursive = false;
	
		for (auto j = P.lower_bound(*i); j != P.upper_bound(*i); j++) //first check if any left recursion occurs with this symbol
		{
			if ((*j).second[0] == *i)
			{
				left_recursive = true;
				break;
			}
		}

		if (left_recursive)
		{
			int next_new_symbol = symbol_max + 1;
			symbol_max++;
			new_NT.insert(next_new_symbol);

			new_productions.insert(std::pair<int, vector<int>>(next_new_symbol, { epsilon }));

			for (auto j = P.lower_bound(*i); j != P.upper_bound(*i); j++)
			{
				if ((*j).second[0] != *i)
				{
					vector<int> new_right = (*j).second;
					new_right.push_back(next_new_symbol);
					new_productions.insert(std::pair<int, vector<int>>(*i, new_right));
				}

				else
				{
					vector<int> new_right = {};
					auto start = (*j).second.begin();
					start++;

					new_right.insert(new_right.end(), start, (*j).second.end());
					new_right.push_back(next_new_symbol);

					new_productions.insert(std::pair<int, vector<int>>(next_new_symbol, new_right));
				}

			}			
		}

		else
		{
			for (auto j = P.lower_bound(*i); j != P.upper_bound(*i); j++)
			{ 
				new_productions.insert(std::pair<int, vector<int>>(*i, (*j).second));
			}
		}
	}

	NT = new_NT;
	P = new_productions;
}

void ContextFreeGrammar::eliminate_left_recursion()
{
	eliminate_indirect_left_recursion();
	eliminate_direct_left_recursion();
}

void ContextFreeGrammar::calculate_first_sets()
{
	for (auto i = T.begin(); i != T.end(); i++)
	{
		first_sets[*i] = { *i };
	}

	for (auto i = NT.begin(); i != NT.end(); i++)
	{
		first_sets[*i] = {};
	}

	bool fixed_point_reached = false;

	while (!fixed_point_reached)
	{
		fixed_point_reached = true; 

		for (auto i = P.begin(); i != P.end(); i++)
		{
			int current_lhs = (*i).first;
			vector<int> beta = (*i).second;

			set<int> rhs = first(beta); 
			
			set<int> old_first_set = first_sets[current_lhs];

			set<int> new_first_set = old_first_set;
			new_first_set.insert(rhs.begin(), rhs.end());

			first_sets[current_lhs] = new_first_set;

			if (new_first_set.size() > old_first_set.size())
			{
				fixed_point_reached = false;
			}
		}
	}
}

void ContextFreeGrammar::calculate_follow_sets()
{
	for (auto i = NT.begin(); i != NT.end(); i++)
	{
		follow_sets[*i] = {};
	}

	follow_sets[true_start] = { EOF };

	bool fixed_point_reached = false;

	while (!fixed_point_reached)
	{
		map<int, set<int>> old_follow_sets = follow_sets;

		for (auto i = P.begin(); i != P.end(); i++)
		{
			int A = (*i).first;
			vector<int> beta = (*i).second;

			set<int> trailer = follow_sets[A];

			for (int i = beta.size() - 1; i > -1; i--)
			{
				if (NT.count(beta[i]) > 0)
				{
					follow_sets[beta[i]].insert(trailer.begin(), trailer.end());

					if (first_sets[beta[i]].count(epsilon) > 0)
					{
						set<int> first_beta_i = first_sets[beta[i]];
						first_beta_i.erase(epsilon);
						trailer.insert(first_beta_i.begin(), first_beta_i.end());
					}

					else
					{
						trailer = first_sets[beta[i]];
					}
				}

				else
				{
					trailer = first_sets[beta[i]];
				}
			}
		}

		if (old_follow_sets == follow_sets)
		{
			fixed_point_reached = true; 
		}
	}
}

void ContextFreeGrammar::calculate_first_plus_sets()
{
	for (auto i = P.begin(); i != P.end(); i++)
	{
		int A = (*i).first;
		vector<int> beta = (*i).second;

		set<int> first_plus = first(beta);

		if (first_plus.count(epsilon) > 0)
		{
			first_plus.insert(follow_sets[A].begin(), follow_sets[A].end());
		}
		first_plus_sets.insert(std::pair<std::pair<int, vector<int>>, set<int>>(*i, first_plus));
	}
}

set<int> ContextFreeGrammar::first(const vector<int>& beta)
{
	if (beta.size() == 0)
	{
		throw InvalidFirstSet();
	}

	set<int> out = first_sets[beta[0]];

	int j = 0;
	while (j < beta.size() - 1 && first_sets[beta[j]].count(epsilon) > 0)
	{
		out.insert(first_sets[beta[j + 1]].begin(), first_sets[beta[j + 1]].end());
		j++;
	}

	out.erase(epsilon);

	if (j == beta.size() - 1 && first_sets[beta[j]].count(epsilon) > 0)
	{
		out.insert(epsilon);
	}

	return out;

}

set<int> ContextFreeGrammar::first(const vector<int>& beta, int terminal)
{
	if (T.count(terminal) == 0)
	{
		throw InvalidFirstSet();
	}

	else if (beta.empty() || (beta.size() == 1 && beta[0] == epsilon))
	{
		return { terminal };
	}

	set<int> first_beta = first(beta);

	if (first_beta.count(epsilon) > 0)
	{
		first_beta.insert(terminal);
	}

	return first_beta;
}


void ContextFreeGrammar::print_first_sets()
{
	for (int terminal : T)
	{
		cout << "first(" << terminal << "): { ";

		for (int i : first_sets.at(terminal))
		{
			cout << i << ", ";
		}

		cout << "}" << endl;

	}

	for (int nonterminal : NT)
	{
		cout << "first(" << nonterminal << "): { ";

		for (int i : first_sets.at(nonterminal))
		{
			cout << i << ", ";
		}

		cout << "}" << endl;

	}
}

void ContextFreeGrammar::calculate_all_sets()
{
 	calculate_first_sets();
	calculate_follow_sets();
	calculate_first_plus_sets();
}

void ContextFreeGrammar::LL1_prepare()
{
	eliminate_left_recursion();
	left_factor();
	calculate_all_sets();
	construct_LL_1_table();
}

void ContextFreeGrammar::LR1_prepare()
{
	calculate_all_sets();
	calculate_CC();
}

void ContextFreeGrammar::left_factor()
{
	multimap<int, vector<int>> old_productions = P;
	multimap<int, vector<int>> new_productions = P;

	set<int> check_set = NT;

	while (!check_set.empty())
	{
		old_productions = new_productions;

		set<int> finished_set = {};
		set<int> new_symbol_set = {};

		for (auto i = check_set.begin(); i != check_set.end(); i++)
		{
			bool replacement = false;
			int replace_symbol = -1;
			set<int> found_first_symbols = {};

			for (auto j = old_productions.lower_bound(*i); j != old_productions.upper_bound(*i); j++)
			{
				if (found_first_symbols.count((*j).second[0]) > 0)
				{
					replacement = true;
					replace_symbol = (*j).second[0];
					break;
				}

				else
				{
					found_first_symbols.insert((*j).second[0]);
				}
			}

			if (replacement)
			{
				new_productions.erase(*i);
				bool found_first = false;
				vector<int> prefix_string = {};
				vector<vector<int>> suffixes = {};
				int current_suffix_index = -1;

				for (auto j = old_productions.lower_bound(*i); j != old_productions.upper_bound(*i); j++)
				{
					vector<int> right_side = (*j).second;
				
					if ((*j).second[0] == replace_symbol)
					{
						if (!found_first)
						{
							prefix_string = right_side;
							found_first = true; 
						}

						else
						{
							int k = 0;

							while (k < prefix_string.size() && k < right_side.size() && prefix_string[k] == right_side[k])
							{
								k++;
							}

							prefix_string.resize(k);
						}
					}

					else
					{
						new_productions.insert(*j);
					}
				}

				for (auto j = old_productions.lower_bound(*i); j != old_productions.upper_bound(*i); j++)
				{
					if ((*j).second[0] != replace_symbol)
					{
						continue;
					}

					vector<int> right_side = (*j).second;

					int k = 0;
					while (k < prefix_string.size())
					{
						k++;
					}
					
					suffixes.push_back({});
					current_suffix_index++;

					while (k < right_side.size())
					{
						suffixes[current_suffix_index].push_back(right_side[k]);
						k++;
					}
				}

				int new_symbol = symbol_max + 1;
				prefix_string.push_back(new_symbol);
				NT.insert(new_symbol);
				new_symbol_set.insert(new_symbol);
				check_set.insert(new_symbol);

				symbol_max++;

				new_productions.insert(std::pair<int, vector<int>>(*i, prefix_string));

				for (int k = 0; k < suffixes.size(); k++)
				{
					if (!suffixes[k].empty())
					{
						new_productions.insert(std::pair<int, vector<int>>(new_symbol, suffixes[k]));
					}

					else
					{
						new_productions.insert(std::pair<int, vector<int>>(new_symbol, { epsilon }));
					}
				}
			}
			
			else
			{
				finished_set.insert(*i);
			}
		}

		for (int i : finished_set)
		{
			check_set.erase(i);
		}

		for (int i : new_symbol_set)
		{
			check_set.insert(i);
		}

		finished_set.clear();
	}

	P = new_productions;
}

bool ContextFreeGrammar::construct_LL_1_table()
{
	backtrack_free = true;
	for (auto i = NT.begin(); i != NT.end(); i++)
	{
		for (auto j = T.begin(); j != T.end(); j++)
		{
			LL_1_table[*i][*j] = {};
		}
	}

	for (auto i = P.begin(); i != P.end(); i++)
	{
		int current_lhs = (*i).first;
		vector<int> current_rhs = (*i).second;

		set<int> first_plus = first_plus_sets[{current_lhs, current_rhs}];

		for (int j : first_plus)
		{
			if (!LL_1_table[current_lhs][j].empty())
			{
				backtrack_free = false;
			}

			LL_1_table[current_lhs][j] = current_rhs;
		}
	}
	return backtrack_free;
}

set<LR1Item> ContextFreeGrammar::closure(set<LR1Item> lr_items)
{
	set<LR1Item> check = closure_lookup_table[lr_items];

	if (check.empty())
	{
		set<LR1Item> out_set = lr_items;

		int old_size = 0;
		int new_size = out_set.size();

		while (old_size < new_size)
		{
			old_size = new_size;

			set<LR1Item> new_items = {};

			for (LR1Item item : out_set)
			{
				if (item.pos < item.right.size() && NT.count(item.right[item.pos])> 0)
				{
					int symbol = item.right[item.pos];

					vector<int> delta;

					for (int i = item.pos + 1; i < item.right.size(); i++)
					{
						delta.push_back(item.right[i]);
					}

					set<int> first_delta = first(delta, item.lookahead);

					for (auto i = P.lower_bound(symbol); i != P.upper_bound(symbol); i++)
					{
						for (int b : first_delta)
						{
							if (b != epsilon)
							{
								new_items.insert(LR1Item(*i, 0, b));
							}
						}
					}
				}
			}

			out_set.insert(new_items.begin(), new_items.end());

			new_size = out_set.size();
		}

		closure_lookup_table[lr_items] = out_set;

		return out_set;
	}
	
	else
	{
		return check;
	}
}

set<LR1Item> ContextFreeGrammar::go_to(set<LR1Item> lr_items, int symbol)
{
	set<LR1Item> moved = {};

	for (LR1Item item : lr_items)
	{
		if (item.pos < item.right.size() && item.right[item.pos] == symbol)
		{
			moved.insert(LR1Item(item.left, item.right, item.pos + 1, item.lookahead));
		}
	}
	return closure(moved);	
}

void ContextFreeGrammar::calculate_CC()
{
	vector<set<LR1Item>> CC;
	set<CCSet> CC_set;

	CCSet start_set(closure({ LR1Item(unique_start, {true_start}, 0, EOF) }), 0);

	CC_set.insert(start_set);
	CC.push_back(start_set.item_set);

	int old_size = 0;
	int new_size = 1;

	while (old_size != new_size)
	{
		old_size = new_size;

		set<CCSet> new_sets = {};

		for (CCSet current_set : CC_set)
		{
			if (!current_set.marked)
			{
				for (LR1Item item : current_set.item_set)
				{

					if (item.pos < item.right.size())
					{
						int next_symbol = item.right[item.pos];

						CCSet temp(go_to(current_set.item_set, next_symbol), -1);
					
						if (temp.item_set.empty())
						{
							break;
						}

						auto CC_set_iterator = CC_set.find(temp);
						auto new_set_iterator = new_sets.find(temp);

						if (CC_set_iterator == CC_set.end() && new_set_iterator == new_sets.end())
						{
							temp.index = CC.size();
							new_sets.insert(temp);
							CC.push_back(temp.item_set);

							if (T.count(next_symbol) > 0)
							{
								set_action(current_set.index, next_symbol, Action(temp.index)); //shift
							}

							else if (NT.count(next_symbol) > 0)
							{
								set_go_to(current_set.index, next_symbol, temp.index);
							}

						}

						else
						{
							temp.index = CC_set_iterator == CC_set.end() ? new_set_iterator->index : CC_set_iterator->index;

							if (T.count(next_symbol) > 0)
							{
								set_action(current_set.index, next_symbol, Action(temp.index)); //shift
							}

							else if (NT.count(next_symbol) > 0)
							{
								set_go_to(current_set.index, next_symbol, temp.index);
							}
						}
					}

					else if (item.left == unique_start && item.right.size() == 1 && item.right[0] == true_start && item.lookahead == EOF)
					{
						set_action(current_set.index, EOF, Action(true)); //accept state
					}

					else
					{
						set_action(current_set.index, item.lookahead, Action(item.left, item.right)); //reduce
					}
				}
				current_set.marked = true;
			}
			new_sets.insert(current_set);
		}
		CC_set = new_sets;
		new_size = CC_set.size();
	}
	canonical_collection = CC;
}

void ContextFreeGrammar::print_CC()
{
	for (int i = 0; i < canonical_collection.size(); i++)
	{
		cout << "CC[" << i << "] : ";
		for (LR1Item item : canonical_collection[i])
		{
			item.print();
			cout << " ";
		}

		cout << endl;
	}
}

void ContextFreeGrammar::print_actions()
{
	for (auto i = action_table.begin(); i != action_table.end(); i++)
	{
		for (auto j = (*i).second.begin(); j != (*i).second.end(); j++)
		{
			cout << "action table [" << (*i).first << "][" << (*j).first << "]: ";
			(*j).second.print();
			cout << "| ";
		}

		cout << endl << endl;
	}

	cout << endl;
}

void ContextFreeGrammar::print_go_to()
{
	for (auto i = go_to_table.begin(); i != go_to_table.end(); i++)
	{
		for (auto j = (*i).second.begin(); j != (*i).second.end(); j++)
		{
			cout << " go to table [" << (*i).first << "][" << (*j).first << "]: " << (*j).second;
		}

		cout << endl << endl;
	}

	cout << endl;
}

void ContextFreeGrammar::set_go_to(int start_state, int nonterminal, int end_state)
{
	if (NT.count(nonterminal) == 0)
	{
		throw TableConstructionError();
	}
	
	go_to_table[start_state][nonterminal] = end_state; 

}

void ContextFreeGrammar::set_action(int start_state, int terminal, Action action)
{
	if (T.count(terminal) == 0)
	{
		throw TableConstructionError();
	}

	Action orig_action = action_table[start_state][terminal];

	if(orig_action.action_type != Action::action::_null)
	{
		if (!(orig_action == action))
		{
			//throw TableConstructionError(); //grammar is not LR(1)!
			cout << "action repeat found at  (" << start_state << ", " << terminal << "): old action:";

			switch (orig_action.action_type) 
			{
			case(Action::action::_accept):
				cout << "accept";
				break;

			case(Action::action::_shift):
				cout << "shift " << orig_action.to_state;
				break;

			case(Action::action::_reduce):
				cout << "reduce " << orig_action.left << " -> ";

				for (int i = 0; i < orig_action.right.size(); i++)
				{
					cout << orig_action.right[i] << " ";
				}

				break;
			}

			cout << ", new action:";

			switch (action.action_type)
			{
			case(Action::action::_accept):
				cout << "accept";
				break;

			case(Action::action::_shift):
				cout << "shift " << action.to_state;
				break;

			case(Action::action::_reduce):
				cout << "reduce " << action.left << " -> ";

				for (int i = 0; i < action.right.size(); i++)
				{
					cout << action.right[i] << " ";
				}
				break;

			}

			cout << endl;

			if (orig_action.action_type == Action::action::_reduce && action.action_type == Action::action::_shift) //favor shifts over reductions
			{
				action_table[start_state][terminal] = action;
			}

			else if (orig_action.action_type == Action::action::_shift && action.action_type == Action::action::_reduce)
			{}

			else
			{
				cout << "possible fatal error detected" << endl;;
			}
		}		
	}

	else
	{
		action_table[start_state][terminal] = action;
	}
}

ParseTree ContextFreeGrammar::LL1_parse(vector<Word> word_string)
{
	ParseTreeNode* root = new ParseTreeNode("", true_start, NULL , {});
	ParseTree out_tree(*root);

	if (word_string.empty())
	{
		throw EmptyParserInput();
	}

	else if (word_string.back().terminal != static_cast<token>(EOF))
	{
		word_string.push_back(Word("", token::_EOF));
	}

	int current_word_index = 0;
	Word current_word = word_string[current_word_index];

	stack<int> word_stack;
	stack<ParseTreeNode*> node_stack;

	word_stack.push(EOF);
	word_stack.push(true_start);

	node_stack.push(NULL);
	node_stack.push(root);


	int focus = word_stack.top();

	ParseTreeNode* current_node = node_stack.top();
	while (true)
	{
		if (focus == EOF && current_word.terminal == token::_EOF)
		{
			cout << "parse success" << word_stack.size() << endl;
			return out_tree;
		}

		else if (T.count(focus) > 0 || focus == EOF)
		{
			if (static_cast<token>(focus) == current_word.terminal)
			{
				word_stack.pop();
				node_stack.pop();

				current_node->lexeme = current_word.lexeme;

				current_word_index++;
				current_word = word_string[current_word_index];
			}

			else
			{
				throw ParseError(); //next token does not match grammar
			}
		}

		else
		{
			vector<int> derivation = LL_1_table[focus][static_cast<int>(current_word.terminal)];

			if (!derivation.empty())
			{
				word_stack.pop();
				node_stack.pop();
				for (int i = derivation.size() - 1; i > -1; i--)
				{
					if (derivation[i] != epsilon)
					{
						word_stack.push(derivation[i]);
						ParseTreeNode* new_node = new ParseTreeNode("", derivation[i], current_node, {});
						current_node->children.push_back(new_node);
						node_stack.push(new_node);
					}

					else
					{
						ParseTreeNode* new_node = new ParseTreeNode("", derivation[i], current_node, {});
						current_node->children.push_back(new_node);
					}
				}

				std::reverse(current_node->children.begin(), current_node->children.end());

			}

			else
			{
				throw ParseError(); //empty derivation
			}
		}
		focus = word_stack.top();
		current_node = node_stack.top();
	}
}

ParseTree ContextFreeGrammar::LR1_parse(vector<Word> word_string) const
{
	if (word_string.empty())
	{
		throw EmptyParserInput();
	}

	else if (word_string.back().terminal != token::_EOF)
	{
		word_string.push_back(Word("", token::_EOF));
	}

	int current_word_index = 0;
	Word current_word = word_string[current_word_index];

	stack<int> word_stack;
	stack<ParseTreeNode*> node_stack;

	node_stack.push(NULL);

	word_stack.push(sentinel);
	word_stack.push(0);

	int current_state = -1;

	while (true)
	{
		current_state = word_stack.top();

		Action action; 

		try 
		{
			action = action_table.at(current_state).at(static_cast<int>(current_word.terminal));
		}

		catch (std::out_of_range)
		{
			vector<ParseTreeNode*> completed_node_list;

			while (!node_stack.empty())
			{
				completed_node_list.push_back(node_stack.top());
				node_stack.pop();
			}


			for (ParseTreeNode* node : completed_node_list)
			{
				node->print();
				cout << endl << endl;
			}

			throw ParseError();
		}
		
		if (action.action_type == Action::action::_reduce)
		{
			ParseTreeNode* new_parent = new ParseTreeNode("", action.left, NULL, {});


			for (int i = 0; i < action.right.size(); i++) //create child nodes here
			{
				word_stack.pop();
				word_stack.pop();

				ParseTreeNode* child_node = node_stack.top();
				node_stack.pop();

				child_node->parent = new_parent;
				new_parent->children.push_back(child_node);
			}

			std::reverse(new_parent->children.begin(), new_parent->children.end());

			current_state = word_stack.top();
			node_stack.push(new_parent);
			word_stack.push(action.left); 
			word_stack.push(go_to_table.at(current_state).at(action.left));
		}

		else if (action.action_type == Action::action::_shift)
		{
			word_stack.push(static_cast<int>(current_word.terminal));
			word_stack.push(action.to_state);

			ParseTreeNode* new_node = new ParseTreeNode(current_word.lexeme, static_cast<int>(current_word.terminal), NULL, {});
			node_stack.push(new_node);

			current_word_index++;
			current_word = word_string[current_word_index];
		}

		else if (action.action_type == Action::action::_accept)
		{
			//cout << "Parse success! " << endl;
			ParseTreeNode* root_node = node_stack.top();
			ParseTree out_tree(*root_node);
			//out_tree.print();
			return out_tree;
		}

		else
		{
			throw ParseError();
		}

	}
}

ParseTree ContextFreeGrammar::LR1_C_parse(vector<Word> word_string) const
{
	if (word_string.empty())
	{
		throw EmptyParserInput();
	}

	else if (word_string.back().terminal != token::_EOF)
	{
		word_string.push_back(Word("", token::_EOF));
	}

	int current_word_index = 0;
	Word current_word = word_string[current_word_index];

	stack<int> word_stack;
	stack<ParseTreeNode*> node_stack;

	node_stack.push(NULL);

	word_stack.push(sentinel);
	word_stack.push(0);

	int current_state = -1;

	multiset<string> typedef_names = {};
	stack<set<string>> scope_stack = {};

	scope_stack.push({});

	while (true)
	{
		current_state = word_stack.top();

		Action action;

		if (current_word.terminal == token::_identifier && typedef_names.count(current_word.lexeme) > 0) //check for typedefs here
		{
			try
			{
				action = action_table.at(current_state).at(static_cast<int>(token::_type_identifier));
				current_word.terminal = token::_type_identifier;
			}

			catch (std::out_of_range)
			{
				try
				{
					action = action_table.at(current_state).at(static_cast<int>(current_word.terminal));
				}

				catch (std::out_of_range)
				{
					throw ParseError();
				}
			}
		}

		else
		{
			try
			{
				action = action_table.at(current_state).at(static_cast<int>(current_word.terminal));
			}

			catch (std::out_of_range)
			{
				vector<ParseTreeNode*> completed_node_list;

				while (!node_stack.empty())
				{
					completed_node_list.push_back(node_stack.top());
					node_stack.pop();
				}


				for (ParseTreeNode* node : completed_node_list)
				{
					if (node)
					{
						node->print();
						cout << endl << endl;
					}
				}

				throw ParseError();
			}

		}

		if (action.action_type == Action::action::_reduce)
		{
			ParseTreeNode* new_parent = new ParseTreeNode("", action.left, NULL, {});


			for (int i = 0; i < action.right.size(); i++) //create child nodes here
			{
				word_stack.pop();
				word_stack.pop();

				ParseTreeNode* child_node = node_stack.top();
				node_stack.pop();

				child_node->parent = new_parent;
				new_parent->children.push_back(child_node);
			}

			std::reverse(new_parent->children.begin(), new_parent->children.end());

			current_state = word_stack.top();
			node_stack.push(new_parent);
			word_stack.push(action.left);
			word_stack.push(go_to_table.at(current_state).at(action.left));

			if (new_parent->symbol == __DECLARATION_PREFIX__)
			{
				set<string> new_typedefs = DeclarationProcessor::process_declaration_prefix(new_parent);

				typedef_names.insert(new_typedefs.begin(), new_typedefs.end());
				scope_stack.top().insert(new_typedefs.begin(), new_typedefs.end());

			}

		}

		else if (action.action_type == Action::action::_shift)
		{
			word_stack.push(static_cast<int>(current_word.terminal));
			word_stack.push(action.to_state);

			ParseTreeNode* new_node = new ParseTreeNode(current_word.lexeme, static_cast<int>(current_word.terminal), NULL, {});
			node_stack.push(new_node);


			if (current_word.lexeme == "{") //handle typedef scopes
			{
				scope_stack.push({});
			}

			else if (current_word.lexeme == "}")
			{
				set<string> stale_typedefs = scope_stack.top();
				scope_stack.pop();

				for (string stale_name : stale_typedefs)
				{
					auto find_name = typedef_names.find(stale_name);
					typedef_names.erase(find_name);
				}
			}


			current_word_index++;
			current_word = word_string[current_word_index];
		}

		else if (action.action_type == Action::action::_accept)
		{
			//cout << "Parse success!" << endl;
			ParseTreeNode* root_node = node_stack.top();
			ParseTree out_tree(*root_node);
			return out_tree;
		}

		else
		{
			throw ParseError();
		}
	}
}



void ContextFreeGrammar::write_LR1_tables_to_file(string file_name) const
{
	ofstream file;

	file.open(file_name + ".txt");

	file << "actions: \n {";

	for (auto i = action_table.begin(); i != action_table.end(); i++)
	{
		int outer_key = (*i).first;

		file << "{ " << outer_key << ", {";

		for (auto j = (*i).second.begin(); j != (*i).second.end(); j++)
		{
			file << "{ " << (*j).first << ", A("; //start of int, action pair 

			Action current_action = (*j).second;

			switch (current_action.action_type)
			{

			case(Action::action::_null):
				file << ")";
				break;

			case(Action::action::_accept):

				file << "true)";

				break;

			case(Action::action::_shift):

				file << current_action.to_state << ")";

				break;

			case(Action::action::_reduce):

				file << current_action.left << ", {";

				for (int k = 0; k < current_action.right.size(); k++)
				{
					file << current_action.right[k] << ", ";
				}

				file.seekp(-2, std::ios_base::cur);

				file << "})"; //end of inner vector (for right side)

				break;
			}

			file << "}, "; //end of int, action pair

		}

		file.seekp(-2, std::ios_base::cur);

		file << "}},\n"; //end of outer_key pair
	}

	file.seekp(-3, std::ios_base::cur);

	file << "} \n";

	file << "go_to: \n {";

	for (auto i = go_to_table.begin(); i != go_to_table.end(); i++)
	{
		int outer_key = (*i).first;

		file << "{ " << outer_key << ", {";

		for (auto j = (*i).second.begin(); j != (*i).second.end(); j++)
		{
			file << "{ " << (*j).first << ", " << (*j).second << "}, ";
		}

		file.seekp(-2, std::ios_base::cur);

		file << "}},\n";
	}

	file.seekp(-3, std::ios_base::cur);
	file << "}";
}


bool operator==(const LR1Item& first, const LR1Item& second)
{
	return (first.left == second.left && first.right == second.right && first.pos == second.pos && first.lookahead == second.lookahead);
}

bool operator<(const LR1Item& first, const LR1Item& second)
{
	if (first.left != second.left)
	{
		return first.left < second.left;
	}

	else if (first.right != second.right)
	{
		return first.right < second.right;
	}

	else if (first.pos != second.pos)
	{
		return first.pos < second.pos;
	}

	else
	{
		return first.lookahead < second.lookahead;
	}
}

bool operator==(const CCSet& first, const CCSet& second)
{
	return first.item_set == second.item_set;
}

bool operator<(const CCSet& first, const CCSet& second)
{
	return first.item_set < second.item_set;
}


bool operator==(const Action& first, const Action& second)
{
	return(first.action_type == second.action_type && first.to_state == second.to_state && first.left == second.left && first.right == second.right);
}
