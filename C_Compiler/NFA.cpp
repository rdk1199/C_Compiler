 #include "NFA.h"
#include "DFA.h"
#include <algorithm>
#include <iterator>
#include <iostream>

using std::ostream;
using std::cout; 
using std::endl;
using std::vector;
using std::set;
using std::map;
using std::multimap;
using std::string;

NFA::NFA(const set<char>& alpha, const vector<map<char, set<int>>>& t_table, const set<int>& a_states):
	num_states(t_table.size()),
	alphabet(alpha),
	trans_table(t_table),
	acc_states(a_states),
	categorizer({}),
	epsilon_closures_found(false)
{
	for (int a : acc_states)
	{
		if (a >= num_states)
		{
			throw(NFAConstructionAcceptingStatesError());
		}
	}
	//calculate_epsilon_closures();
}

NFA::NFA(const set<char>& alpha, char c) :
	num_states(2),
	alphabet(alpha),
	acc_states({ 1 }),
	trans_table({ {{c, {1}}},{} }),
	categorizer({}),
	epsilon_closures_found(false)
{
	//calculate_epsilon_closures();
}

NFA::NFA(const set<char>& alpha, const set<char>& chars) :
	num_states(2),
	alphabet(alpha),
	acc_states({ 1 }),
	trans_table({ {},{} }),
	categorizer({}),
	epsilon_closures_found(false)
{
	for (char c : chars)
	{
		trans_table[0][c] = { 1 };
	}

	//calculate_epsilon_closures();

}

NFA::NFA(const set<char>& alpha, const string& s) :
	num_states(s.size() + 1),
	alphabet(alpha),
	acc_states({ num_states -1 }),
	trans_table({}),
	categorizer({}),
	epsilon_closures_found(false)
{
	trans_table.resize(s.size() + 1);

	int current_state = 0;
	for (auto i = s.begin(); i != s.end(); i++)
	{
		trans_table[current_state][*i] = { current_state + 1 };
		current_state++;
	}

	//calculate_epsilon_closures();
}

NFA::NFA(const set<char>& alpha, const set<string>& strs):
	num_states(1),
	alphabet(alpha),
	acc_states({}),
	trans_table({ {} }),
	categorizer({}),
	epsilon_closures_found(false)
{
	for (string str : strs)
	{
		int current_state = 0;

		auto i = str.begin();

		while (i != str.end() && trans_table[current_state][*i].size() > 0)
		{
			current_state = *trans_table[current_state][*i].begin();
			i++;
		}
		
		if (i == str.end())
		{
			acc_states.insert(current_state);
			continue;
		}	

		while (i != str.end())
		{
			trans_table.push_back({});
			int current_size = trans_table.size();
			trans_table[current_state][*i] = { current_size - 1 };	
			current_state = *trans_table[current_state][*i].begin();
			i++;
		}	

		acc_states.insert(current_state);
	}

	num_states = trans_table.size();
	merge_acc_states();

}


NFA::NFA(const set<char>& alpha, const vector<map<char, set<int>>>& t_table, const set<int>& a_states, const map<int, int>& categories) :
	num_states(t_table.size()),
	alphabet(alpha),
	trans_table(t_table),
	acc_states(a_states),
	categorizer(categories),
	epsilon_closures_found(false)
{

	for (auto i = categories.begin(); i != categories.end(); i++)
	{
		if (acc_states.count(i->first) == 0)
		{
			throw new NFACategoryConstructionError();
		}
	}

	for (int a : acc_states)
	{
		if (a >= num_states)
		{
			throw(NFAConstructionAcceptingStatesError());
		}
	}
	//calculate_epsilon_closures();
}

NFA NFA::complement(const set<char>& alpha, const set<char>& letters)
{
	set<char> comp = {};
	for (char c : alpha)
	{
		if (letters.find(c) == letters.end())
		{
			comp.insert(c);
		}
	}

	return NFA(alpha, comp);
}

void const NFA::print_trans_table()
{
	cout << "NFA transition table size: " << num_states << endl;
	for (int i = 0; i < num_states; i++)
	{
		cout << i << ":";
		for (auto j = trans_table[i].begin(); j != trans_table[i].end(); j++)
		{
			cout << j->first << " : ";
			
			cout << "{";
			for (auto k = j->second.begin(); k != j->second.end(); k++)
			{
				cout << *k << " ";
			}
			
			cout << "},";// << endl;

		}
		cout << endl;
	}

	cout << "accepting states: ";

	for (auto i = acc_states.begin(); i != acc_states.end(); i++)
	{
		cout << *i << " ";
	}
	cout << endl; 
}

void NFA::calculate_epsilon_closures()
{
	epsilon_closures.resize(num_states);
	for (int i = 0; i < num_states; i++)
	{	
		epsilon_closures[i] = { i };
	}

	set<int> work_list;

	for (int i = 0; i < num_states; i++)
	{
		work_list.insert(i);
	}
	
	while (!work_list.empty())
	{
		int n = *work_list.begin();

		work_list.erase(work_list.begin());

		set<int> t = {n};

		for (auto i = trans_table[n]['\0'].begin(); i != trans_table[n]['\0'].end(); i++)
		{
			t.insert(*i);
			for (auto j = epsilon_closures[*i].begin(); j != epsilon_closures[*i].end(); j++)
			{
				t.insert(*j);
			}
		}

		if (t != epsilon_closures[n])
		{
			epsilon_closures[n] = t;
			for (int i = 0; i < num_states; i++)
			{
				if (trans_table[i]['\0'].find(n) != trans_table[i]['\0'].end())
				{
					work_list.insert(i);
				}
			}
		}
		
	}

	epsilon_closures_found = true;
}

set<int> NFA::epsilon_closure(set<int> states)
{
	if (!epsilon_closures_found)
	{
		calculate_epsilon_closures();
	}

	set<int> out = {};

	for (auto i = states.begin(); i != states.end(); i++)
	{
		out.insert(epsilon_closures[*i].begin(), epsilon_closures[*i].end());
	}

	return out; 
}

set<int> NFA::epsilon_closure(int state)
{
	if (!epsilon_closures_found)
	{
		calculate_epsilon_closures();
	}


	if (state < 0 || state >= num_states)
	{
		throw NFAEpsilonClosureOutOfBounds();
	}

	return epsilon_closures[state];
}

set<int> NFA::trans_table_union(set<int> states, char c)
{
	set<int> out = {};

	for (auto i = states.begin(); i != states.end(); i++)
	{
		out.insert(trans_table[*i][c].begin(), trans_table[*i][c].end());
	}

	return out; 
}

void NFA::print_epsilon_closures()
{
	if (!epsilon_closures_found)
	{
		calculate_epsilon_closures();
	}


	for (int i = 0; i < epsilon_closures.size(); i++)
	{
		cout << "epsilon closure of node " << i << ": ";
		for (auto j = epsilon_closures[i].begin(); j != epsilon_closures[i].end(); j++)
		{
			cout << *j << " ";
		}
		cout << endl;
	}
}

DFA const NFA::subset_construct_DFA() //TO DO: add error checking
{
	if (!epsilon_closures_found)
	{
		calculate_epsilon_closures();
	}

	map<set<int>, int> dfa_state_map; //Q; maps subset states to new state numbers
	set<set<int>> work_list;
	vector<map<char, int>> dfa_trans_table;
	
	set<int> dfa_start = epsilon_closure(0);

	dfa_state_map[dfa_start] = 0;
	work_list.insert(dfa_start);
	dfa_trans_table.push_back({});

	int state_num = 1;

	while (!work_list.empty())
	{
		set<int> q = *work_list.begin();
		work_list.erase(work_list.begin());

		for (char c : alphabet)
		{
			set<int> t = epsilon_closure(trans_table_union(q, c));
			
			if (t.empty())
			{
				dfa_trans_table[dfa_state_map[q]][c] = -1;
				continue;
			}

			auto i = dfa_state_map.find(t);

			if (i != dfa_state_map.end())
			{
				dfa_trans_table[dfa_state_map[q]][c] = dfa_state_map[t];
			}
			
			else
			{
				dfa_state_map[t] = state_num;
				state_num++;
				dfa_trans_table.push_back({});

				dfa_trans_table[dfa_state_map[q]][c] = dfa_state_map[t];

				work_list.insert(t);
			}	
		}
	}

	set<int> dfa_accepting_states = {};
	map<int, int> dfa_categories;

	for (auto i = dfa_state_map.begin(); i != dfa_state_map.end(); i++)
	{
		set<int> acc_intersection;
		
		std::set_intersection(i->first.begin(), i->first.end(), acc_states.begin(), acc_states.end(), std::inserter(acc_intersection, acc_intersection.end()));

		if (!acc_intersection.empty())
		{
			int state_rep = *(acc_intersection.begin());
			dfa_accepting_states.insert(i->second);
			dfa_categories[i->second]= categorizer[state_rep];
		}
	}

	DFA out(alphabet, dfa_trans_table, dfa_accepting_states, dfa_categories);

	return out; 
}

set<int> shift_set(set<int> s, int amt)
{
	set<int> out = {};
	for (int i : s)
	{
		out.insert(i + amt);
	}
	return out;
}

NFA operator+(const NFA& a, const NFA& b)
{
	set<char> alph = a.get_alphabet();
	set<int> a_acc_states = a.get_acc_states();
	set<int> b_acc_states = b.get_acc_states();

	if (a_acc_states.size() != 1 || b_acc_states.size() != 1)
	{
		throw new IllegalNFAOperation();
	}
	
	set<char> b_alph = b.get_alphabet();

	alph.insert(b_alph.begin(), b_alph.end());
	alph.insert('\0');
	
	vector<map<char, set<int>>> a_trans_table = a.get_trans_table();
	vector<map<char, set<int>>> b_trans_table = b.get_trans_table(); 

	int a_size = a.get_size();
	int b_size = b.get_size();

	vector<map<char, set<int>>> new_trans_table(a_size + b_size + 2); 

	for (int i = 1; i < 1 + a_size; i++)
	{
		for (char c : alph)
		{
			new_trans_table[i][c] = shift_set(a_trans_table[i - 1][c], 1);
		}
	}

	for (int i = a_size + 1; i < a_size + b_size + 1; i++)
	{
		for (char c : alph)
		{
			new_trans_table[i][c] = shift_set(b_trans_table[i - a_size - 1][c], a_size + 1);
		}
	}

	int a_start_state = 1;
	int b_start_state = a_size + 1;

	int a_acc_state = *a_acc_states.begin() + 1;
	int b_acc_state = *b_acc_states.begin() + a_size + 1;

	new_trans_table[0] = { {'\0', {a_start_state, b_start_state}} };
	new_trans_table[a_acc_state]['\0'] = { a_size + b_size + 1 };
	new_trans_table[b_acc_state]['\0'] = { a_size + b_size + 1 };

	alph.erase('\0');

	return NFA(alph, new_trans_table, { a_size + b_size + 1 });
}

NFA operator*(const NFA& a, const NFA& b)
{
	set<char> alph = a.get_alphabet();

	set<int> a_acc_states = a.get_acc_states();
	set<int> b_acc_states = b.get_acc_states();

	if (a_acc_states.size() != 1 || b_acc_states.size() != 1)
	{
		throw new IllegalNFAOperation();
	}

	set<char> b_alph = b.get_alphabet();

	alph.insert(b_alph.begin(), b_alph.end());
	alph.insert('\0');

	vector<map<char, set<int>>> a_trans_table = a.get_trans_table();
	vector<map<char, set<int>>> b_trans_table = b.get_trans_table();

	int a_size = a.get_size();
	int b_size = b.get_size();

	vector<map<char, set<int>>> new_trans_table(a_size + b_size);

	for (int i = 0; i < a_size; i++)
	{
		for (char c : alph)
		{
			new_trans_table[i][c] = a_trans_table[i][c];
		}
	}

	for (int i = a_size; i < a_size + b_size; i++)
	{
		for (char c : alph)
		{
			new_trans_table[i][c] = shift_set(b_trans_table[i - a_size][c], a_size);
		}
	}

	int b_start_state = a_size;
	int a_acc_state = *a_acc_states.begin();
	int b_acc_state = *b_acc_states.begin() + a_size;

	new_trans_table[a_acc_state]['\0'] = { b_start_state };

	alph.erase('\0');

	return NFA(alph, new_trans_table, { b_acc_state });

}


NFA NFA::kleene_closure(const NFA& nfa)
{
	int num_states = nfa.num_states;
	set<int> acc_states = nfa.get_acc_states();
	set<char> alphabet = nfa.get_alphabet();
	vector<map<char, set<int>>> trans_table  = nfa.get_trans_table();

	alphabet.insert('\0');


	if (acc_states.size() != 1)
	{
		throw new IllegalNFAOperation();
	}
		 

	vector<map<char, set<int>>> new_trans_table(num_states +2);

	for (int i = 1; i < 1 + num_states; i++)
	{
		for (char c : alphabet)
		{
			new_trans_table[i][c] = shift_set(trans_table[i - 1][c], 1);
		}
	}

	int acc_state = *acc_states.begin() + 1;

	new_trans_table[0]['\0'] = { 1, num_states + 1 };
	new_trans_table[acc_state]['\0'] = { 1, num_states + 1 };

	alphabet.erase('\0');

	return NFA(alphabet, new_trans_table, { num_states + 1 });
}

DFA NFA::subset_construct_DFA(NFA& nfa)
{
	return nfa.subset_construct_DFA();
}

DFA NFA::combine_categorize(const vector<NFA>& nfas, const vector<int>& categories)
{
	if (nfas.size() != categories.size())
	{
		throw new NFACategoryConstructionError();
	}

	int size_sum = 1;

	for (int i = 0; i < nfas.size(); i++)
	{
		size_sum += nfas[i].get_size();
	}

	vector<map<char, set<int>>> new_trans_table(size_sum);
	
	int current_start = 1;

	set<char> combined_alphabet = {};
	set<int> combined_acc_states = {};

	map<int, int> new_categorizer; 

	for (int i = 0; i < nfas.size(); i++)
	{
		vector<map<char, set<int>>> current_trans_table = nfas[i].get_trans_table();
		set<char> current_alphabet = nfas[i].get_alphabet();
		
		combined_alphabet.insert(current_alphabet.begin(), current_alphabet.end());

		current_alphabet.insert('\0');

		for (int j = current_start; j < current_start + nfas[i].get_size(); j++)
		{
			for (char c : current_alphabet)
			{
				new_trans_table[j][c] = shift_set(current_trans_table[j - current_start][c], current_start);
			}
		}

		set<int> current_acc_states = shift_set(nfas[i].get_acc_states(), current_start);

		combined_acc_states.insert(current_acc_states.begin(), current_acc_states.end());

		for (int s : current_acc_states)
		{
			new_categorizer[s] = categories[i];
		}

		new_trans_table[0]['\0'].insert(current_start);

		current_start += nfas[i].get_size();
	}

	NFA new_NFA(combined_alphabet, new_trans_table, combined_acc_states, new_categorizer);
	return subset_construct_DFA(new_NFA);
}

void NFA::merge_acc_states()
{
	if (acc_states.size() <= 1)
		return;

	trans_table.push_back({});

	int end_state = trans_table.size() - 1;

	for (int state : acc_states)
	{
		trans_table[state]['\0'] = { end_state };
	}

	acc_states = { end_state };
	num_states++;
}

NFA NFA::multi_union(const vector<NFA>& terms)
{
	if (terms.size() == 0)
	{
		throw new IllegalNFAOperation();
	}

	else if (terms.size() == 1)
	{
		return terms[0];
	}

	int size_sum = 1;

	for (int i = 0; i < terms.size(); i++)
	{
		size_sum += terms[i].get_size();
	}

	vector<map<char, set<int>>> new_trans_table(size_sum);

	int current_start = 1;

	set<char> combined_alphabet = {};
	set<int> combined_acc_states = {};

	for (int i = 0; i < terms.size(); i++)
	{
		vector<map<char, set<int>>> current_trans_table = terms[i].get_trans_table();
		set<char> current_alphabet = terms[i].get_alphabet();

		combined_alphabet.insert(current_alphabet.begin(), current_alphabet.end());

		current_alphabet.insert('\0');

		for (int j = current_start; j < current_start + terms[i].get_size(); j++)
		{
			for (char c : current_alphabet)
			{
				new_trans_table[j][c] = shift_set(current_trans_table[j - current_start][c], current_start);
			}
		}

		set<int> current_acc_states = shift_set(terms[i].get_acc_states(), current_start);

		combined_acc_states.insert(current_acc_states.begin(), current_acc_states.end());

		new_trans_table[0]['\0'].insert(current_start);

		current_start += terms[i].get_size();
	}

	NFA new_NFA(combined_alphabet, new_trans_table, combined_acc_states);

	new_NFA.merge_acc_states();

	return new_NFA;
}

NFA NFA::multi_concat(const vector<NFA>& terms)
{
	if (terms.size() == 0)
	{
		throw new IllegalNFAOperation();
	}

	else if (terms.size() == 1)
	{
		return terms[0];
	}

	int size_sum = 0;

	for (int i = 0; i < terms.size(); i++)
	{
		size_sum += terms[i].get_size();
	}

	vector<map<char, set<int>>> new_trans_table(size_sum+1);

	int current_start = 0;

	set<char> combined_alphabet = {};

	for (int i = 0; i < terms.size(); i++)
	{
		vector<map<char, set<int>>> current_trans_table = terms[i].get_trans_table();
		set<char> current_alphabet = terms[i].get_alphabet();
		int current_size = terms[i].get_size();

		combined_alphabet.insert(current_alphabet.begin(), current_alphabet.end());

		current_alphabet.insert('\0');

		for (int j = current_start; j < current_start + current_size; j++)
		{
			for (char c : current_alphabet)
			{
				new_trans_table[j][c] = shift_set(current_trans_table[j - current_start][c], current_start);
			}
		}

		set<int> current_acc_states = shift_set(terms[i].get_acc_states(), current_start);

		for (int state : current_acc_states)
		{
			new_trans_table[state]['\0'] = {current_start + current_size};
		}

		current_start += current_size;
	}

	NFA new_NFA(combined_alphabet, new_trans_table, {size_sum});

	return new_NFA;
}

NFA NFA::multi_concat(const NFA& nfa, int n)
{
	vector<NFA> nfas;
	for (int i = 0; i < n; i++)
	{
		nfas.push_back(nfa);
	}

	return multi_concat(nfas);
}