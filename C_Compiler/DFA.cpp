#include "DFA.h"
#include <iostream>
#include <fstream>
#include "NFA.h"
#include "CSyntax.h"

using std::set;
using std::vector;
using std::map;
using std::multimap;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

DFA::DFA(const set<char>& alpha, const vector<map<char, int>>& t_table, const set<int>& a_states) :
	alphabet(alpha),
	num_states(t_table.size()),
	trans_table(t_table),
	acc_states(a_states),
	categorizer({})
{
	for (int i = 0; i < trans_table.size(); i++)
	{
		for (char c : alphabet)
		{
			if (trans_table[i].find(c) == trans_table[i].end())
			{
				trans_table[i][c] = -1; //set default values to -1 (error state)
			}
		}
	}

	for (int i : acc_states)
	{
		categorizer[i] = 0;
	}
}

DFA::DFA(const set<char>& alpha, const vector<map<char, int>>& t_table, const set<int>& a_states, const map<int, int>& categories) :
	alphabet(alpha),
	num_states(t_table.size()),
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

	for (int i = 0; i < trans_table.size(); i++)
	{
		for (char c : alphabet)
		{
			if (trans_table[i].find(c) == trans_table[i].end())
			{
				trans_table[i][c] = -1; //set default values to -1 (error state)
			}
		}
	}

	for (int i : acc_states)
	{
		int j = categorizer[i]; //this will place all non-categorized acc_states in category 0 by default (could cause issues)
	}

}

int DFA::accepts(const string& str) const 
{
	int state = 0;

	for (auto i = str.begin(); i != str.end(); i++)
	{
		if (alphabet.find(*i) == alphabet.end())
		{
			return -1; //symbol from outside alphabet
		}
		state = trans_table[state].at(*i);

		if (state == -1)
		{
			return -1; //reached error state
		}
	}

	if (acc_states.count(state) > 0)
	{
		return categorizer.at(state);
	}

	else
	{
		return -1; //finished in non-accepting state 
	}
}

vector<set<int>> DFA::split(const set<int>& s, const set<set<int>>& P)
{
	if (s.size() <= 1)
	{
		vector<set<int>> out{ s };
		return out;
	}

	vector<set<int>> partition_map(num_states); //maps each state to its partition member
	
	for (auto i = P.begin(); i != P.end(); i++)
	{
		for (auto j = i->begin(); j != i->end(); j++)
		{
			partition_map[*j] = *i;
		}
	}

	bool split = false;
	char split_char = '\0';

	for (char c : alphabet)
	{
		auto i = s.begin();
		auto j = s.begin();
		i++;

		while (i != s.end())
		{

			if ((trans_table[*i][c] == -1) ^ (trans_table[*j][c] == -1))
			{
				split = true; 
				break;
			}

			else if (trans_table[*i][c] == -1 && trans_table[*j][c] == -1)
			{
				i++;
				j++;
				continue;
			}

			if (partition_map[trans_table[*i][c]] != partition_map[trans_table[*j][c]])
			{
				split = true;
				break;
			}
			i++;
			j++;
		}

		if (split)
		{
			split_char = c;
			
			// ========== debug ================
			/*
			cout << " split set: ";
			for (int i : s)
			{
				cout << i << " ";
			}
			cout << " split character: " << split_char << endl;
			
			//==================================================//*/

			break;
		}

	}

	vector<set<int>> out;

	if (split)
	{
		set<int> err_states = {};

		for (auto i = s.begin(); i != s.end(); i++)
		{
			if (trans_table[*i][split_char] == -1)
			{
				err_states.insert(*i);
				continue;
			}

			bool in = false; 
			for (int j = 0; j < out.size(); j++)
			{
				if (partition_map[trans_table[*i][split_char]] == partition_map[trans_table[*out[j].begin()][split_char]])
				{
					out[j].insert(*i);
					in = true;
					break;
				}
			}

			if (!in)
			{
				out.push_back({ *i });
			}
		}

		if (!err_states.empty())
		{
			out.push_back(err_states);
		}
	}

	else
	{
		out = { s };
	}

	return out; 
}

vector<set<int>> DFA::split_acc_states()
{
	vector<set<int>> acc_partition;

	for (int s : acc_states)
	{

		bool matched = false;

		for (int i = 0; i < acc_partition.size(); i++)
		{
			int set_rep = *acc_partition[i].begin();
			if (categorizer[s] == categorizer[set_rep])
			{
				acc_partition[i].insert(s);
				matched = true;
				break;
			}
		}

		if (!matched)
		{
			acc_partition.push_back({ s });
		}
	}
	return acc_partition;
}

void DFA::minimize() //Hopcroft's alg
{
	set<set<int>> T;
	set<set<int>> P;

	set<int> initial_acc_states = acc_states;
	set<int> initial_acc_states_comp = {};

	for (int i = 0; i < num_states; i++)
	{
		if (acc_states.find(i) == acc_states.end())
		{
			initial_acc_states_comp.insert(i);
		}
	}

	//T = { initial_acc_states_comp, acc_states};

	

	vector<set<int>> acc_partition = split_acc_states();

	T = { initial_acc_states_comp };
	T.insert(acc_partition.begin(), acc_partition.end());
	
	

	while (T != P)
	{
		//====================== debug ========================//
		/*
		cout << "current partition: " << endl;

		for (auto i = T.begin(); i != T.end(); i++)
		{
			for (auto j = i->begin(); j != i->end(); j++)
			{
				cout << *j << " ";
			}

			cout << endl;
		}
		*/
		//======================== debug ==========================//

		P = T;
		T = {};

		for (auto p : P)
		{
			vector<set<int>> split_p = split(p, P);
			T.insert(split_p.begin(), split_p.end());
		}
	}
	//========================= debug =================================//
	/*
	cout << "final partition: " << endl;

	for (auto i = T.begin(); i != T.end(); i++)
	{
		for (auto j = i->begin(); j != i->end(); j++)
		{
			cout << *j << " ";
		}

		cout << endl;
	}

	*/

	//===================================================================//

	map<set<int>, int> set_state_map; //maps set (representing a new state) to its new state number
	vector<set<int>> old_state_map(num_states); //maps old state to new state set
	vector<set<int>> new_state_map; //will map new state number to corresponding new state set

	for (set<int> s : P)
	{
		if (s.find(0) != s.end())
		{
			set_state_map[s] = 0;
			for (auto i = s.begin(); i != s.end(); i++)
			{
				old_state_map[*i] = s;
			}
			P.erase(s);
			new_state_map.push_back(s);
			break;
		}
	}

	int state_num = 1;

	for (set<int> s : P)
	{
		if (s.empty()) //empty sets are ignored
		{
			continue;
		}

		int set_rep = *s.begin();
		bool fail_state = true;

		if (acc_states.find(set_rep) != acc_states.end()) //if s is a subset of acc states, then it's not a fail state
		{
			fail_state = false;
		}

		else // if t_table of set representative is always -1, then, once state is reached, acceptance is not possible => fail state
		{
			for (char c : alphabet)
			{
				int next_state = trans_table[set_rep][c];

				if (next_state != -1 && s.find(next_state) == s.end())
				{
					fail_state = false;
					break;
				}
			}

		}

		if (fail_state)
		{
			continue;
		}

		for (auto i = s.begin(); i != s.end(); i++)
		{
			old_state_map[*i] = s;
		}
		set_state_map[s] = state_num;
		new_state_map.push_back(s);

		state_num++;
	}

	vector<map<char, int>> new_trans_table(state_num);
	set<int> new_acc_states;
	map<int, int> new_categorizer;

	for (int i = 0; i < state_num; i++)
	{
		int state_rep = *new_state_map[i].begin();

		if (acc_states.find(state_rep) != acc_states.end())
		{
			new_acc_states.insert(i);
			new_categorizer[i] = categorizer[state_rep];
		}

		for (char c : alphabet)
		{
			int out_state = trans_table[state_rep][c];
			if (out_state == -1)
			{
				new_trans_table[i][c] = -1;
			}

			else
			{
				new_trans_table[i][c] = set_state_map[old_state_map[out_state]];
			}
		}
	}

	num_states = new_state_map.size();
	trans_table = new_trans_table;
	acc_states = new_acc_states;
	categorizer = new_categorizer;
}

int const DFA::get_size()
{
	return trans_table.size();
}

set<int> const DFA::get_acc_states()
{
	return acc_states;
}

void const DFA::print_transition_table()
{
	cout << "DFA transition table: " << endl;

	int row = 0;

	for (auto i = trans_table.begin(); i != trans_table.end(); i++)
	{

		cout << row << " | ";

		for (auto j = i->begin(); j != i->end(); j++)
		{
			cout << j->first << " : " << j->second << ", ";
		}
		cout << endl;

		row++;
	}

	cout << "number of states: " << num_states << endl;
	cout << "acc states: ";
	for (auto i = acc_states.begin(); i != acc_states.end(); i++)
	{
		cout << "(" << *i << ": " << categorizer[*i] << ")";
	}

	cout << endl;

}

NFA DFA::to_NFA()
{
	vector<map<char, set<int>>> nfa_trans_table(num_states);

	for (int i = 0; i < num_states; i++)
	{
		for (char c : alphabet)
		{
			if (trans_table[i][c] != -1)
			{
				nfa_trans_table[i][c] = { trans_table[i][c] };
			}
		}
	}

	return NFA(alphabet, nfa_trans_table, acc_states);
}

bool const DFA::accept_test(const vector<string>& strs, const vector<bool>& expected, string test_name)
{
	int n = strs.size();
	if (n != expected.size())
	{
		throw new InvalidDFATest();
	}

	bool passed = true;
	int passed_num = n;

	for (int i = 0; i < n; i++)
	{
		bool accept = (accepts(strs[i]) != -1);

		if (accept != expected[i])
		{
			passed = false;
			passed_num--;
		}
		cout << strs[i] << ": " << accept << ", expected: " << expected[i] << endl;
	}

	if (passed)
	{
		cout << test_name << " PASSED" << endl;
		return true;
	}

	else
	{
		cout << test_name << " FAILED, "<< passed_num << "/" << n << " TESTS PASSED" << endl;
		return false;
	}

}

bool const DFA::categorize_test(const vector<string>& strs, const vector<int>& expected, string test_name)
{
	int n = strs.size();
	if (n != expected.size())
	{
		throw new InvalidDFATest();
	}

	bool passed = true;
	int passed_num = n;

	cout << test_name << ": " << endl;

	for (int i = 0; i < n; i++)
	{
		int accept = accepts(strs[i]);

		if (accept != expected[i])
		{
			passed = false;
			passed_num--;
		}
		cout << strs[i] << ": " << accept << ", expected: " << expected[i] << endl;
	}

	if (passed)
	{
		cout << test_name << " CATEGORIZATION PASSED" << endl;
		return true; 
	}

	else
	{
		cout << test_name << "CATEGORIZATION FAILED, " << passed_num << "/" << n << " TESTS PASSED" << endl;
		return false;
	}

}

int DFA::categorize(int i) const
{
	if (acc_states.find(i) != acc_states.end())
	{
		return categorizer.at(i);
	}
	else
	{
		return -1;
	}
}

void DFA::print_acc_states() 
{
	cout << "acc states: ";
	for (auto i = acc_states.begin(); i != acc_states.end(); i++)
	{
		cout << "(" << *i << ": " << categorizer[*i] << ")";
	}

	cout << endl;
}

void DFA::remove_dead_states()
{
	set<int> dead_states = {};
	for (int i = 0; i < num_states; i++)
	{
		bool dead_state = true;

		for (char c : alphabet)
		{
		}
	}
}

void DFA::write_to_file(string file_name) const 
{
	ofstream file;

	file.open(file_name + ".txt");

	file << "accepting states: \n {";

	auto acc_i = acc_states.begin();

	while (acc_i != acc_states.end())
	{
		file << *acc_i << ", ";
		acc_i++;
	}

	file.seekp(-2, std::ios_base::cur); //erase final comma

	file << "} \n";

	file << "categorizer: \n {";

	auto cat_i = categorizer.begin();

	while (cat_i != categorizer.end())
	{
		file << "{" << (*cat_i).first << " , " << (*cat_i).second << "},\n";
		cat_i++;
	}

	file.seekp(-3, std::ios_base::cur);

	file << "} \n";

	file << "trans table: \n {";

	for (int i = 0; i < trans_table.size(); i++)
	{
		file << "{";
		
		auto trans_i = trans_table[i].begin();

		while (trans_i != trans_table[i].end())
		{
			char key = (*trans_i).first;

			if (escape_sequence_map.count(key) == 0)
			{
				file << "{ \'" << key << "\', " << (*trans_i).second << "}, ";
			}

			else
			{
				file << "{ \'" << escape_sequence_map.at(key) << "\', " << (*trans_i).second << "}, ";
			}

			trans_i++;
		}

		file.seekp(-2, std::ios_base::cur);

		file << "},\n";

		//cout << i << endl;

	}
	
	file.seekp(-3, std::ios_base::cur);

	file << "} \n";
}