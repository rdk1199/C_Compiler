#include "CompressedDFA.h"

#include <iostream>

using std::cout;
using std::endl;
using std::set;
using std::map;
using std::vector;
using std::multimap;
using std::string;
CompressedDFA::CompressedDFA(const set<char>& alph, const map<char, int>& c_cat, const vector<vector<int>>& t_table,const set<int>& a_states, const map<int, int>& categories) :
	num_states(t_table.size()),
	alphabet(alph),
	char_cat(c_cat),
	trans_table(t_table),
	acc_states(a_states),
	categorizer(categories)
{
	for (auto i = categories.begin(); i != categories.end(); i++)
	{
		if (acc_states.count(i->first) == 0)
		{
			throw new DFACategoryConstructionError();
		}
	}

}

CompressedDFA CompressedDFA::compress(DFA dfa)
{
	int num_states = dfa.get_size();
	set<char> alphabet = dfa.get_alphabet();
	set<int> acc_states = dfa.get_acc_states();
	
	vector<map<char, int>> old_trans_table = dfa.get_trans_table();

	vector<set<char>> comp_t_table;

	for (char c : alphabet)
	{
		bool matched = false;

		for (int i = 0; i < comp_t_table.size(); i++)
		{
			char set_rep = *comp_t_table[i].begin();
			bool match = true;

			for (int j = 0; j < num_states; j++)
			{
				if (old_trans_table[j][c] != old_trans_table[j][set_rep])
				{
					match = false;
					break;
				}
			}

			if (match)
			{
				matched = true; 
				comp_t_table[i].insert(c);
				break;
			}
		}

		if (!matched)
		{
			comp_t_table.push_back({ c });
		}
	}

	map<char, int> char_cat_map;

	for (int i = 0; i < comp_t_table.size(); i++)
	{
		for (auto j = comp_t_table[i].begin(); j != comp_t_table[i].end(); j++)
		{
			char_cat_map[*j] = i;
		}
	}
	
	vector<vector<int>> final_trans_table(num_states);

	for (int i = 0; i < num_states; i++)
	{
		for (int j = 0; j < comp_t_table.size(); j++)
		{
			final_trans_table[i].push_back(old_trans_table[i][*comp_t_table[j].begin()]);
		}
	}

	return CompressedDFA(alphabet, char_cat_map, final_trans_table, acc_states, dfa.get_categorizer());
}

int const CompressedDFA::accepts(const string& str)
{
	int state = 0;

	for (auto i = str.begin(); i != str.end(); i++)
	{
		if (alphabet.find(*i) == alphabet.end())
		{
			return -1;
		}

		state = trans_table[state][char_cat[*i]];

		if (state == -1)
		{
			return -1;
		}
	}

	if (acc_states.count(state) > 0)
	{
		return categorizer[state];
	}

	else
	{
		return -1; //finished in non-accepting state 
	}
}

void const CompressedDFA::print_transition_table()
{
	cout << "DFA transition table: " << endl;
	for (auto i = trans_table.begin(); i != trans_table.end(); i++)
	{
		for (auto j = i->begin(); j != i->end(); j++)
		{
			cout << *j << ", ";
		}
		cout << endl;
	}

	cout << "acc states: ";
	for (auto i = acc_states.begin(); i != acc_states.end(); i++)
	{
		cout << *i << " ";
	}

	cout << endl;
}

bool const CompressedDFA::accept_test(const vector<string>& strs, const vector<bool>& expected)
{
	int n = strs.size();
	if (n != expected.size())
	{
		throw new InvalidDFATest();
	}

	bool passed = true;

	for (int i = 0; i < n; i++)
	{
		bool accept = (accepts(strs[i]) != -1);

		if (accept != expected[i])
		{
			passed = false;
		}
		cout << strs[i] << ": " << accept << ", expected: " << expected[i] << endl;
	}

	if (passed)
	{
		cout << "DFA ACCEPTANCE TEST PASSED" << endl;
		return true;
	}

	else
	{
		cout << "DFA ACCEPTANCE TEST FAILED" << endl;
		return false;
	}

}

bool const CompressedDFA::categorize_test(const vector<string>& strs, const vector<int>& expected)
{
	int n = strs.size();
	if (n != expected.size())
	{
		throw new InvalidDFATest();
	}

	bool passed = true;

	for (int i = 0; i < n; i++)
	{
		int accept = accepts(strs[i]);

		if (accept != expected[i])
		{
			passed = false;
		}
		cout << strs[i] << ": " << accept << ", expected: " << expected[i] << endl;
	}

	if (passed)
	{
		cout << "DFA CATEGORIZE TEST PASSED" << endl;
		return true;
	}

	else
	{
		cout << "DFA CATEGORIZE TEST FAILED" << endl;
		return false;
	}

}