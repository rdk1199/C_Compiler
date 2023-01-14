#include "TableParser.h"
#include "TableDrivenScanner.h"
#include "Preprocessor.h"

#include <string>

using std::stoi;
using std::string;
using std::map;
using std::multimap;
using std::vector;
using std::set;

//terminals 
#define __IDENTIFIER__ (static_cast<int>(token::_identifier))
#define __INT_CONST__ (static_cast<int>(token::_integer_constant))
#define __LEFT_PAREN__ (static_cast<int>(token::_left_paren))
#define __RIGHT_PAREN__ (static_cast<int>(token::_right_paren))
#define __LEFT_BRACE__ (static_cast<int>(token::_left_brace))
#define __RIGHT_BRACE__ (static_cast<int>(token::_right_brace))
#define __COLON__ (static_cast<int>(token::_colon))
#define __COMMA__ (static_cast<int> (token::_comma))
#define __MINUS__ (static_cast<int> (token::_minus))

#define __MAX__ 100

//nonterminals
#define __TABLE_SET__ __MAX__ + 1
#define __LABEL__ __MAX__ + 2
#define __ACTION_TABLE__ __MAX__ + 3
#define __GOTO_TABLE__ __MAX__ + 4
#define __OUTER_ACTION_MAP_LIST__ __MAX__ + 5
#define __OUTER_ACTION_PAIR__ __MAX__ + 6
#define __INNER_ACTION_MAP_LIST__ __MAX__ + 7	
#define __INNER_ACTION_PAIR__ __MAX__ + 8
#define __ACTION__ __MAX__ + 9
#define __SHIFT_ACTION__ __MAX__ + 10	
#define __ACCEPT_ACTION__ __MAX__ + 11	
#define __REDUCE_ACTION__ __MAX__ + 12	
#define __REDUCE_PRODUCTION__ __MAX__ + 13	
#define __OUTER_GOTO_MAP_LIST__ __MAX__ + 14
#define __OUTER_GOTO_PAIR__ __MAX__ + 15
#define __INNER_GOTO_MAP_LIST__ __MAX__ + 16	
#define __INNER_GOTO_PAIR__ __MAX__ + 17
#define __SIGNED_INT__ __MAX__ + 18	


const set<int> table_grammar_terminals = { __IDENTIFIER__, __INT_CONST__, __LEFT_PAREN__, __RIGHT_PAREN__, __LEFT_BRACE__, __RIGHT_BRACE__, __COLON__, __COMMA__, __MINUS__ };
	
const set<int> table_grammar_nonterminals = {__TABLE_SET__, __LABEL__, __ACTION_TABLE__, __GOTO_TABLE__, __OUTER_ACTION_MAP_LIST__, __OUTER_ACTION_PAIR__, __INNER_ACTION_MAP_LIST__, __INNER_ACTION_PAIR__, __ACTION__, __SHIFT_ACTION__,
										__ACCEPT_ACTION__, __REDUCE_ACTION__, __REDUCE_PRODUCTION__, __OUTER_GOTO_MAP_LIST__, __OUTER_GOTO_PAIR__, __INNER_GOTO_MAP_LIST__, __INNER_GOTO_PAIR__, __SIGNED_INT__};

const multimap<int, vector<int>> table_grammar_productions =
{
	{__TABLE_SET__, {__LABEL__, __COLON__, __ACTION_TABLE__, __LABEL__, __COLON__, __GOTO_TABLE__}},

	{__LABEL__, {__IDENTIFIER__}},

	{__ACTION_TABLE__, {__LEFT_BRACE__, __OUTER_ACTION_MAP_LIST__, __RIGHT_BRACE__}},
	
	{__OUTER_ACTION_MAP_LIST__, {__OUTER_ACTION_PAIR__}},
	{__OUTER_ACTION_MAP_LIST__, {__OUTER_ACTION_PAIR__, __COMMA__, __OUTER_ACTION_MAP_LIST__}},

	{__OUTER_ACTION_PAIR__, {__LEFT_BRACE__, __SIGNED_INT__, __COMMA__, __LEFT_BRACE__, __INNER_ACTION_MAP_LIST__, __RIGHT_BRACE__, __RIGHT_BRACE__}},

	{__INNER_ACTION_MAP_LIST__, { __INNER_ACTION_PAIR__, __COMMA__, __INNER_ACTION_MAP_LIST__ }},
	{__INNER_ACTION_MAP_LIST__, {__INNER_ACTION_PAIR__}},

	{__INNER_ACTION_PAIR__, {__LEFT_BRACE__, __SIGNED_INT__, __COMMA__, __ACTION__, __RIGHT_BRACE__}},

	{__ACTION__, {__SHIFT_ACTION__}},
	{__ACTION__, {__ACCEPT_ACTION__}},
	{__ACTION__, {__REDUCE_ACTION__}},

	{__SHIFT_ACTION__, {__IDENTIFIER__, __LEFT_PAREN__, __SIGNED_INT__, __RIGHT_PAREN__}},

	{__ACCEPT_ACTION__, {__IDENTIFIER__, __LEFT_PAREN__, __IDENTIFIER__, __RIGHT_PAREN__}},

	{__REDUCE_ACTION__, {__IDENTIFIER__, __LEFT_PAREN__, __SIGNED_INT__, __COMMA__, __LEFT_BRACE__, __REDUCE_PRODUCTION__, __RIGHT_BRACE__, __RIGHT_PAREN__}},

	{__REDUCE_PRODUCTION__, {__SIGNED_INT__}},
	{__REDUCE_PRODUCTION__, {__SIGNED_INT__, __COMMA__, __REDUCE_PRODUCTION__}},

	{__GOTO_TABLE__, {__LEFT_BRACE__, __OUTER_GOTO_MAP_LIST__, __RIGHT_BRACE__}},

	{__OUTER_GOTO_MAP_LIST__, {__OUTER_GOTO_PAIR__}},
	{__OUTER_GOTO_MAP_LIST__, {__OUTER_GOTO_PAIR__, __COMMA__, __OUTER_GOTO_MAP_LIST__}},

	{__OUTER_GOTO_PAIR__, {__LEFT_BRACE__, __SIGNED_INT__, __COMMA__, __LEFT_BRACE__, __INNER_GOTO_MAP_LIST__, __RIGHT_BRACE__, __RIGHT_BRACE__}},

	{__INNER_GOTO_MAP_LIST__, { __INNER_GOTO_PAIR__, __COMMA__, __INNER_GOTO_MAP_LIST__ }},
	{__INNER_GOTO_MAP_LIST__, {__INNER_GOTO_PAIR__}},

	{__INNER_GOTO_PAIR__, {__LEFT_BRACE__, __SIGNED_INT__, __COMMA__, __SIGNED_INT__, __RIGHT_BRACE__}},

	{__SIGNED_INT__, {__INT_CONST__}},
	{__SIGNED_INT__, {__MINUS__, __INT_CONST__}}
};

int table_start = __TABLE_SET__;

map<int, map<int, Action>> table_grammar_actions =

{ { 0, {{ 0, Action(3)}}},
{ 1, {{ -1, Action(true)}}},
{ 2, {{ 37, Action(4)}}},
{ 3, {{ 37, Action(102, {0})}}},
{ 4, {{ 10, Action(6)}}},
{ 5, {{ 0, Action(3)}}},
{ 6, {{ 10, Action(10)}}},
{ 7, {{ 37, Action(11)}}},
{ 8, {{ 11, Action(12)}}},
{ 9, {{ 11, Action(105, {106})}, { 51, Action(13)}}},
{ 10, {{ 2, Action(15)}, { 19, Action(16)}}},
{ 11, {{ 10, Action(18)}}},
{ 12, {{ 0, Action(103, {10, 105, 11})}}},
{ 13, {{ 10, Action(10)}}},
{ 14, {{ 51, Action(20)}}},
{ 15, {{ 51, Action(118, {2})}}},
{ 16, {{ 2, Action(21)}}},
{ 17, {{ -1, Action(101, {102, 37, 103, 102, 37, 104})}}},
{ 18, {{ 10, Action(24)}}},
{ 19, {{ 11, Action(105, {106, 51, 105})}}},
{ 20, {{ 10, Action(25)}}},
{ 21, {{ 51, Action(118, {19, 2})}}},
{ 22, {{ 11, Action(26)}}},
{ 23, {{ 11, Action(114, {115})}, { 51, Action(30)}}},
{ 24, {{ 2, Action(15)}, { 19, Action(16)}}},
{ 25, {{ 10, Action(29)}}},
{ 26, {{ -1, Action(104, {10, 114, 11})}}},
{ 27, {{ 11, Action(32)}}},
{ 28, {{ 11, Action(107, {108})}, { 51, Action(33)}}},
{ 29, {{ 2, Action(15)}, { 19, Action(16)}}},
{ 30, {{ 10, Action(24)}}},
{ 31, {{ 51, Action(36)}}},
{ 32, {{ 11, Action(37)}}},
{ 33, {{ 10, Action(29)}}},
{ 34, {{ 51, Action(39)}}},
{ 35, {{ 11, Action(114, {115, 51, 114})}}},
{ 36, {{ 10, Action(40)}}},
{ 37, {{ 11, Action(106, {10, 118, 51, 10, 107, 11, 11})}, { 51, Action(106, {10, 118, 51, 10, 107, 11, 11})}}},
{ 38, {{ 11, Action(107, {108, 51, 107})}}},
{ 39, {{ 0, Action(45)}}},
{ 40, {{ 10, Action(48)}}},
{ 41, {{ 11, Action(49)}}},
{ 42, {{ 11, Action(109, {110})}}},
{ 43, {{ 11, Action(109, {111})}}},
{ 44, {{ 11, Action(109, {112})}}},
{ 45, {{ 8, Action(50)}}},
{ 46, {{ 11, Action(51)}}},
{ 47, {{ 11, Action(116, {117})}, { 51, Action(52)}}},
{ 48, {{ 2, Action(15)}, { 19, Action(16)}}},
{ 49, {{ 11, Action(108, {10, 118, 51, 109, 11})}, { 51, Action(108, {10, 118, 51, 109, 11})}}},
{ 50, {{ 0, Action(55)}, { 2, Action(56)}, { 19, Action(57)}}},
{ 51, {{ 11, Action(58)}}},
{ 52, {{ 10, Action(48)}}},
{ 53, {{ 51, Action(60)}}},
{ 54, {{ 9, Action(61)}, { 51, Action(62)}}},
{ 55, {{ 9, Action(63)}}},
{ 56, {{ 9, Action(118, {2})}, { 51, Action(118, {2})}}},
{ 57, {{ 2, Action(67)}}},
{ 58, {{ 11, Action(115, {10, 118, 51, 10, 116, 11, 11})}, { 51, Action(115, {10, 118, 51, 10, 116, 11, 11})}}},
{ 59, {{ 11, Action(116, {117, 51, 116})}}},
{ 60, {{ 2, Action(65)}, { 19, Action(66)}}},
{ 61, {{ 11, Action(110, {0, 8, 118, 9})}}},
{ 62, {{ 10, Action(68)}}},
{ 63, {{ 11, Action(111, {0, 8, 0, 9})}}},
{ 64, {{ 11, Action(69)}}},
{ 65, {{ 11, Action(118, {2})}}},
{ 66, {{ 2, Action(70)}}},
{ 67, {{ 9, Action(118, {19, 2})}, { 51, Action(118, {19, 2})}}},
{ 68, {{ 2, Action(73)}, { 19, Action(74)}}},
{ 69, {{ 11, Action(117, {10, 118, 51, 118, 11})}, { 51, Action(117, {10, 118, 51, 118, 11})}}},
{ 70, {{ 11, Action(118, {19, 2})}}},
{ 71, {{ 11, Action(75)}}},
{ 72, {{ 11, Action(113, {118})}, { 51, Action(76)}}},
{ 73, {{ 11, Action(118, {2})}, { 51, Action(118, {2})}}},
{ 74, {{ 2, Action(77)}}},
{ 75, {{ 9, Action(78)}}},
{ 76, {{ 2, Action(73)}, { 19, Action(74)}}},
{ 77, {{ 11, Action(118, {19, 2})}, { 51, Action(118, {19, 2})}}},
{ 78, {{ 11, Action(112, {0, 8, 118, 51, 10, 113, 11, 9})}}},
{ 79, {{ 11, Action(113, {118, 51, 113})}}} };

map<int, map<int, int>> table_go_to =
{ { 0, {{ 101, 1}, { 102, 2}}},
{ 4, {{ 103, 5}}},
{ 5, {{ 102, 7}}},
{ 6, {{ 105, 8}, { 106, 9}}},
{ 10, {{ 118, 14}}},
{ 11, {{ 104, 17}}},
{ 13, {{ 105, 19}, { 106, 9}}},
{ 18, {{ 114, 22}, { 115, 23}}},
{ 24, {{ 118, 31}}},
{ 25, {{ 107, 27}, { 108, 28}}},
{ 29, {{ 118, 34}}},
{ 30, {{ 114, 35}, { 115, 23}}},
{ 33, {{ 107, 38}, { 108, 28}}},
{ 39, {{ 109, 41}, { 110, 42}, { 111, 43}, { 112, 44}}},
{ 40, {{ 116, 46}, { 117, 47}}},
{ 48, {{ 118, 53}}},
{ 50, {{ 118, 54}}},
{ 52, {{ 116, 59}, { 117, 47}}},
{ 60, {{ 118, 64}}},
{ 68, {{ 113, 71}, { 118, 72}}},
{ 76, {{ 113, 79}, { 118, 72}}} };


ContextFreeGrammar table_grammar{table_grammar_terminals, table_grammar_nonterminals, table_grammar_productions, table_start, table_grammar_actions, table_go_to};

map<int, map<int, Action>> get_action_table(ParseTreeNode* action_table_node);
map<int, map<int, int>> get_go_to_table(ParseTreeNode* go_to_table_node);

void process_outer_action_map_list(ParseTreeNode* outer_action_list_node, vector<std::pair<int, map<int, Action>>>& out_vector);
std::pair<int, map<int, Action>> get_outer_action_pair(ParseTreeNode* outer_action_pair_node);
void process_inner_action_map_list(ParseTreeNode* inner_action_list_node, vector<std::pair<int, Action>>& out_vector);
std::pair<int, Action> get_inner_action_pair(ParseTreeNode* inner_action_pair_node);
Action get_action(ParseTreeNode* action_node);
Action get_shift_action(ParseTreeNode* shift_action_node);
Action get_reduce_action(ParseTreeNode* reduce_action_node);
void process_production(ParseTreeNode* reduce_production_node, vector<int>& out_production);

void process_outer_go_to_map_list(ParseTreeNode* outer_go_to_list_node, vector<std::pair<int, map<int, int>>>& out_vector);
std::pair<int, map<int, int>> get_outer_go_to_pair(ParseTreeNode* outer_go_to_pair_node);
void process_inner_go_to_map_list(ParseTreeNode* inner_go_to_list_node, vector<std::pair<int, int>>& out_vector);
std::pair<int, int> get_inner_go_to_pair(ParseTreeNode* inner_go_to_pair_node);

int get_signed_int_value(ParseTreeNode* signed_int_node);

std::pair<map<int, map<int, Action>>, map<int, map<int, int>>> get_tables_from_file(string file_name)
{
	TableDrivenScanner scanner(file_name, C_tokenizer, 500);

	vector<Word> file_tokens = Preprocessor::convert_to_tokens(scanner.scan());

	ParseTree table_tree = table_grammar.LR1_parse(file_tokens);

	ParseTreeNode root = table_tree.get_root();

	if (root.symbol != __TABLE_SET__)
	{
		throw TableParseError();
	}

	else return std::pair<map<int, map<int, Action>>, map<int, map<int, int>>>{get_action_table(root.children[2]) , get_go_to_table(root.children[5])};
}

map<int, map<int, Action>> get_action_table(ParseTreeNode* action_table_node)
{
	if (action_table_node->symbol != __ACTION_TABLE__)
	{
		throw TableParseError();
	}

	map<int, map<int, Action>> out = {};

	vector<std::pair<int, map<int, Action>>> map_entries = {};
	process_outer_action_map_list(action_table_node->children[1], map_entries);

	for (int i = 0; i < map_entries.size(); i++)
	{
		out.insert(map_entries[i]);
	}


	return out; 
}

void process_outer_action_map_list(ParseTreeNode* outer_action_list_node, vector<std::pair<int, map<int, Action>>>& out_vector)
{
	if (outer_action_list_node->symbol != __OUTER_ACTION_MAP_LIST__)
	{
		throw TableParseError();
	}

	out_vector.push_back(get_outer_action_pair(outer_action_list_node->children[0]));

	if (outer_action_list_node->children.size() == 3)
	{
		process_outer_action_map_list(outer_action_list_node->children[2], out_vector);
	}

}

std::pair<int, map<int, Action>> get_outer_action_pair(ParseTreeNode* outer_action_pair_node)
{

	if (outer_action_pair_node->symbol != __OUTER_ACTION_PAIR__)
	{
		throw TableParseError();
	}

	std::pair<int, map<int, Action>> out_pair;

	out_pair.first = get_signed_int_value(outer_action_pair_node->children[1]);

	map<int, Action > inner_action_map = {};

	vector<std::pair<int, Action>> inner_map_entries = {};
	process_inner_action_map_list(outer_action_pair_node->children[4], inner_map_entries);

	for (int i = 0; i < inner_map_entries.size(); i++)
	{
		inner_action_map.insert(inner_map_entries[i]);
	}

	out_pair.second = inner_action_map;

	return out_pair;
}

void process_inner_action_map_list(ParseTreeNode* inner_action_list_node, vector<std::pair<int, Action>>& out_vector)
{
	if (inner_action_list_node->symbol != __INNER_ACTION_MAP_LIST__)
	{
		throw TableParseError();
	}

	out_vector.push_back(get_inner_action_pair(inner_action_list_node->children[0]));

	if (inner_action_list_node-> children.size() == 3)
	{
		process_inner_action_map_list(inner_action_list_node->children[2], out_vector);
	}

}

std::pair<int, Action> get_inner_action_pair(ParseTreeNode* inner_action_pair_node)
{
	if (inner_action_pair_node->symbol != __INNER_ACTION_PAIR__)
	{
		throw TableParseError();
	}

	std::pair<int, Action> out_pair;

	out_pair.first = get_signed_int_value(inner_action_pair_node->children[1]);

	out_pair.second = get_action(inner_action_pair_node->children[3]);

	return out_pair;

}

Action get_action(ParseTreeNode* action_node)
{
	if (action_node->symbol != __ACTION__)
	{
		throw TableParseError();
	}

	ParseTreeNode* action_type_node = action_node->children[0];

	switch (action_type_node->symbol)
	{
	case(__ACCEPT_ACTION__):
		return Action(true); 
		break;

	case(__SHIFT_ACTION__):
		return get_shift_action(action_type_node);
		break;

	case(__REDUCE_ACTION__):
		return get_reduce_action(action_type_node);
		break;

	default:
		return Action();
		break;
	}
}

Action get_shift_action(ParseTreeNode* shift_action_node)
{
	if (shift_action_node->symbol != __SHIFT_ACTION__)
	{
		throw TableParseError();
	}

	int shift_value = get_signed_int_value(shift_action_node->children[2]);

	return Action(shift_value);
}

Action get_reduce_action(ParseTreeNode* reduce_action_node)
{
	if (reduce_action_node->symbol != __REDUCE_ACTION__)
	{
		throw TableParseError();
	}

	int production_left = get_signed_int_value(reduce_action_node->children[2]);

	vector<int> production_right = {};

	process_production(reduce_action_node->children[5], production_right);

	return Action(production_left, production_right);
}

void process_production(ParseTreeNode* reduce_production_node, vector<int>& out_production)
{
	if (reduce_production_node->symbol != __REDUCE_PRODUCTION__)
	{
		throw TableParseError();
	}

	out_production.push_back(get_signed_int_value(reduce_production_node->children[0]));

	if (reduce_production_node->children.size() == 3)
	{
		process_production(reduce_production_node->children[2], out_production);
	}
}


map<int, map<int, int>> get_go_to_table(ParseTreeNode* go_to_table_node)
{
	if (go_to_table_node->symbol != __GOTO_TABLE__)
	{
		throw TableParseError();
	}

	map<int, map<int, int>> out = {};

	vector<std::pair<int, map<int, int>>> map_entries = {};
	process_outer_go_to_map_list(go_to_table_node->children[1], map_entries);

	for (int i = 0; i < map_entries.size(); i++)
	{
		out.insert(map_entries[i]);
	}

	return out;
}

void process_outer_go_to_map_list(ParseTreeNode* outer_go_to_list_node, vector<std::pair<int, map<int, int>>>& out_vector)
{
	if (outer_go_to_list_node->symbol != __OUTER_GOTO_MAP_LIST__)
	{
		throw TableParseError();
	}

	out_vector.push_back(get_outer_go_to_pair(outer_go_to_list_node->children[0]));

	if (outer_go_to_list_node->children.size() == 3)
	{
		process_outer_go_to_map_list(outer_go_to_list_node->children[2], out_vector);
	}

}

std::pair<int, map<int, int>> get_outer_go_to_pair(ParseTreeNode* outer_go_to_pair_node)
{

	if (outer_go_to_pair_node->symbol != __OUTER_GOTO_PAIR__)
	{
		throw TableParseError();
	}

	std::pair<int, map<int, int>> out_pair;

	out_pair.first = get_signed_int_value(outer_go_to_pair_node->children[1]);

	map<int, int > inner_go_to_map = {};

	vector<std::pair<int, int>> inner_map_entries = {};
	process_inner_go_to_map_list(outer_go_to_pair_node->children[4], inner_map_entries);

	for (int i = 0; i < inner_map_entries.size(); i++)
	{
		inner_go_to_map.insert(inner_map_entries[i]);
	}

	out_pair.second = inner_go_to_map;

	return out_pair;
}

void process_inner_go_to_map_list(ParseTreeNode* inner_go_to_list_node, vector<std::pair<int, int>>& out_vector)
{
	if (inner_go_to_list_node->symbol != __INNER_GOTO_MAP_LIST__)
	{
		throw TableParseError();
	}

	out_vector.push_back(get_inner_go_to_pair(inner_go_to_list_node->children[0]));

	if (inner_go_to_list_node->children.size() == 3)
	{
		process_inner_go_to_map_list(inner_go_to_list_node->children[2], out_vector);
	}

}

std::pair<int, int> get_inner_go_to_pair(ParseTreeNode* inner_go_to_pair_node)
{
	if (inner_go_to_pair_node->symbol != __INNER_GOTO_PAIR__)
	{
		throw TableParseError();
	}

	std::pair<int, int> out_pair;

	out_pair.first = get_signed_int_value(inner_go_to_pair_node->children[1]);

	out_pair.second = get_signed_int_value(inner_go_to_pair_node->children[3]);

	return out_pair;

}

int get_signed_int_value(ParseTreeNode* signed_int_node)
{
	if (signed_int_node->symbol != __SIGNED_INT__)
	{
		throw TableParseError();
	}

	string int_lexeme;

	if (signed_int_node->children.size() == 2)
	{
		int_lexeme = signed_int_node->children.back()->lexeme;

		return -stoi(int_lexeme);
	}

	else if (signed_int_node->children.size() == 1)
	{
		int_lexeme = signed_int_node->children.back()->lexeme;

		return stoi(int_lexeme);
	}

	else
	{
		throw TableParseError();
	}

	return 0;
}