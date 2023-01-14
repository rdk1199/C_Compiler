#include "ExpressionGrammar.h"
#include <ctype.h>
#include <stdlib.h>
//not allowed to have assignment, increment, decrement, function call, or comma operators
//all constants must be integer or character

using std::multimap;
using std::set;
using std::map;
using std::vector;
using std::string;

//terminals
#define __INTEGER_CONSTANT__ (static_cast<int>(token::_integer_constant))
#define __CHARACTER_CONSTANT__ (static_cast<int>(token::_character_constant))
#define __QUESTION_MARK__ (static_cast<int> (token::_question))
#define __COLON__ (static_cast<int>(token::_colon))
#define __OR__ (static_cast<int>(token::_bar_bar))
#define __AND_AND__ (static_cast<int>(token::_and_and))
#define __INCLUSIVE_OR__ (static_cast<int>(token::_bar))
#define __EXCLUSIVE_OR__ (static_cast<int>(token::_caret))
#define __AND__ (static_cast<int>(token::_and))
#define __EQUAL_EQUAL__ (static_cast<int> (token::_equal_equal))
#define __NOT_EQUAL__ (static_cast<int>(token::_not_equal))
#define __LESS_THAN__ (static_cast<int> (token::_less_than))
#define __GREATER_THAN__ (static_cast<int> (token::_greater_than))
#define __LEQ__ (static_cast<int> (token::_leq))
#define __GEQ__ (static_cast<int> (token::_geq))
#define __LEFT_SHIFT__ (static_cast<int> (token::_shift_left))
#define __RIGHT_SHIFT__ (static_cast<int> (token::_shift_right))
#define __PLUS__ (static_cast<int> (token::_plus))
#define __MINUS__ (static_cast<int> (token::_minus))
#define __MULTIPLY__ (static_cast<int> (token::_asterisk))
#define __DIVIDE__ (static_cast<int>(token::_forward_slash))
#define __MOD__ (static_cast<int> (token::_percent))
#define __TILDE__ (static_cast<int> (token::_tilde))
#define __NOT__ (static_cast<int> (token::_exclamation))
#define __LEFT_PAREN__ (static_cast<int> (token::_left_paren))
#define __RIGHT_PAREN__ (static_cast<int> (token::_right_paren))


//non-terminals
#define __CONDITIONAL_EXPRESSION__ 54
#define __LOGICAL_OR_EXPRESSION__ 55
#define __LOGICAL_AND_EXPRESSION__ 56
#define __INCLUSIVE_OR_EXPRESSION__ 57
#define __EXCLUSIVE_OR_EXPRESSION__ 58
#define __AND_EXPRESSION__ 59
#define __EQUALITY_EXPRESSION__ 60
#define __RELATIONAL_EXPRESSION__ 61
#define __SHIFT_EXPRESSION__ 62
#define __ADDITIVE_EXPRESSION__ 63
#define __MULTIPLICATIVE_EXPRESSION__ 64
#define __UNARY_EXPRESSION__ 65
#define __UNARY_OPERATOR__ 66
#define __PRIMARY_EXPRESSION__ 67


set<int> terminals = { __INTEGER_CONSTANT__, __CHARACTER_CONSTANT__, __QUESTION_MARK__, __COLON__, __OR__, __AND_AND__, __INCLUSIVE_OR__, __EXCLUSIVE_OR__, __AND__, __EQUAL_EQUAL__, __NOT_EQUAL__,
  __LESS_THAN__, __GREATER_THAN__, __LEQ__, __GEQ__, __LEFT_SHIFT__, __RIGHT_SHIFT__, __PLUS__, __MINUS__, __MULTIPLY__, __DIVIDE__, __MOD__, __TILDE__, __NOT__, __LEFT_PAREN__, __RIGHT_PAREN__ };

set<int> nonterminals = { __CONDITIONAL_EXPRESSION__, __LOGICAL_OR_EXPRESSION__, __LOGICAL_AND_EXPRESSION__, __INCLUSIVE_OR_EXPRESSION__,__EXCLUSIVE_OR_EXPRESSION__, __AND_EXPRESSION__, __EQUALITY_EXPRESSION__, __RELATIONAL_EXPRESSION__,
  __SHIFT_EXPRESSION__, __ADDITIVE_EXPRESSION__, __MULTIPLICATIVE_EXPRESSION__, __UNARY_EXPRESSION__, __UNARY_OPERATOR__, __PRIMARY_EXPRESSION__ };

multimap<int, vector<int>> productions = {
	  {__CONDITIONAL_EXPRESSION__, {__LOGICAL_OR_EXPRESSION__}},
	  {__CONDITIONAL_EXPRESSION__, {__LOGICAL_OR_EXPRESSION__, __QUESTION_MARK__, __CONDITIONAL_EXPRESSION__, __COLON__, __CONDITIONAL_EXPRESSION__}},
	  {__LOGICAL_OR_EXPRESSION__, {__LOGICAL_AND_EXPRESSION__}},
	  {__LOGICAL_OR_EXPRESSION__, {__LOGICAL_OR_EXPRESSION__, __OR__, __LOGICAL_AND_EXPRESSION__}},
	  {__LOGICAL_AND_EXPRESSION__, {__INCLUSIVE_OR_EXPRESSION__}},
	  {__LOGICAL_AND_EXPRESSION__, { __LOGICAL_AND_EXPRESSION__, __AND_AND__, __INCLUSIVE_OR_EXPRESSION__}},
	  {__INCLUSIVE_OR_EXPRESSION__, {__EXCLUSIVE_OR_EXPRESSION__}},
	  {__INCLUSIVE_OR_EXPRESSION__, {__INCLUSIVE_OR_EXPRESSION__, __INCLUSIVE_OR__, __EXCLUSIVE_OR_EXPRESSION__ }},
	  {__EXCLUSIVE_OR_EXPRESSION__, {__AND_EXPRESSION__}},
	  {__EXCLUSIVE_OR_EXPRESSION__, { __EXCLUSIVE_OR_EXPRESSION__, __EXCLUSIVE_OR__, __AND_EXPRESSION__ }},
	  {__AND_EXPRESSION__, {__EQUALITY_EXPRESSION__}},
	  {__AND_EXPRESSION__, {__AND_EXPRESSION__,  __AND__, __EQUALITY_EXPRESSION__}},
	  {__EQUALITY_EXPRESSION__, {__RELATIONAL_EXPRESSION__}},
	  {__EQUALITY_EXPRESSION__, {__EQUALITY_EXPRESSION__, __EQUAL_EQUAL__,  __RELATIONAL_EXPRESSION__ }},
	  {__EQUALITY_EXPRESSION__, {__EQUALITY_EXPRESSION__, __NOT_EQUAL__, __RELATIONAL_EXPRESSION__}},
	  {__RELATIONAL_EXPRESSION__, {__SHIFT_EXPRESSION__}},
	  {__RELATIONAL_EXPRESSION__, {__RELATIONAL_EXPRESSION__, __LESS_THAN__, __SHIFT_EXPRESSION__}},
	  {__RELATIONAL_EXPRESSION__, {__RELATIONAL_EXPRESSION__, __GREATER_THAN__, __SHIFT_EXPRESSION__}},
	  {__RELATIONAL_EXPRESSION__, {__RELATIONAL_EXPRESSION__, __LEQ__, __SHIFT_EXPRESSION__}},
	  {__RELATIONAL_EXPRESSION__, {__RELATIONAL_EXPRESSION__, __GEQ__, __SHIFT_EXPRESSION__}},
	  {__SHIFT_EXPRESSION__, {__ADDITIVE_EXPRESSION__}},
	  {__SHIFT_EXPRESSION__, { __SHIFT_EXPRESSION__ , __LEFT_SHIFT__,__ADDITIVE_EXPRESSION__}},
	  {__SHIFT_EXPRESSION__, {__SHIFT_EXPRESSION__ , __RIGHT_SHIFT__, __ADDITIVE_EXPRESSION__ }},
	  {__ADDITIVE_EXPRESSION__, {__MULTIPLICATIVE_EXPRESSION__}},
	  {__ADDITIVE_EXPRESSION__, {__ADDITIVE_EXPRESSION__ , __PLUS__, __MULTIPLICATIVE_EXPRESSION__}},
	  {__ADDITIVE_EXPRESSION__, {__ADDITIVE_EXPRESSION__ , __MINUS__, __MULTIPLICATIVE_EXPRESSION__}},
	  {__MULTIPLICATIVE_EXPRESSION__, {__UNARY_EXPRESSION__}},
	  {__MULTIPLICATIVE_EXPRESSION__, {__MULTIPLICATIVE_EXPRESSION__ , __MULTIPLY__, __UNARY_EXPRESSION__ }},
	  {__MULTIPLICATIVE_EXPRESSION__, {__MULTIPLICATIVE_EXPRESSION__ , __DIVIDE__, __UNARY_EXPRESSION__}},
	  {__MULTIPLICATIVE_EXPRESSION__, {__MULTIPLICATIVE_EXPRESSION__, __MOD__,  __UNARY_EXPRESSION__}},
	  {__UNARY_EXPRESSION__, {__PRIMARY_EXPRESSION__}},
	  {__UNARY_EXPRESSION__, {__UNARY_OPERATOR__, __UNARY_EXPRESSION__}},
	  {__UNARY_OPERATOR__, {__PLUS__}},
	  {__UNARY_OPERATOR__, {__MINUS__}},
	  {__UNARY_OPERATOR__, {__TILDE__}},
	  {__UNARY_OPERATOR__, {__NOT__}},
	  {__PRIMARY_EXPRESSION__, {__INTEGER_CONSTANT__}},
	  {__PRIMARY_EXPRESSION__, {__CHARACTER_CONSTANT__}},
	  {__PRIMARY_EXPRESSION__, {__LEFT_PAREN__, __CONDITIONAL_EXPRESSION__, __RIGHT_PAREN__}}
};

int start = __CONDITIONAL_EXPRESSION__;

class ExpressionError : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Expression error found";
	}
};

map<int, map<int, Action>> expression_actions = { { 0, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 1, {{ -1, Action(true)}}},
{ 2, {{ -1, Action(54, {55})}, { 35, Action(40)}, { 36, Action(39)}}},
{ 3, {{ -1, Action(55, {56})}, { 34, Action(41)}, { 35, Action(55, {56})}, { 36, Action(55, {56})}}},
{ 4, {{ -1, Action(56, {57})}, { 33, Action(42)}, { 34, Action(56, {57})}, { 35, Action(56, {57})}, { 36, Action(56, {57})}}},
{ 5, {{ -1, Action(57, {58})}, { 32, Action(43)}, { 33, Action(57, {58})}, { 34, Action(57, {58})}, { 35, Action(57, {58})}, { 36, Action(57, {58})}}},
{ 6, {{ -1, Action(58, {59})}, { 16, Action(44)}, { 32, Action(58, {59})}, { 33, Action(58, {59})}, { 34, Action(58, {59})}, { 35, Action(58, {59})}, { 36, Action(58, {59})}}},
{ 7, {{ -1, Action(59, {60})}, { 16, Action(59, {60})}, { 30, Action(45)}, { 31, Action(46)}, { 32, Action(59, {60})}, { 33, Action(59, {60})}, { 34, Action(59, {60})}, { 35, Action(59, {60})}, { 36, Action(59, {60})}}},
{ 8, {{ -1, Action(60, {61})}, { 16, Action(60, {61})}, { 26, Action(47)}, { 27, Action(48)}, { 28, Action(49)}, { 29, Action(50)}, { 30, Action(60, {61})}, { 31, Action(60, {61})}, { 32, Action(60, {61})}, { 33, Action(60, {61})}, { 34, Action(60, {61})}, { 35, Action(60, {61})}, { 36, Action(60, {61})}}},
{ 9, {{ -1, Action(61, {62})}, { 16, Action(61, {62})}, { 24, Action(51)}, { 25, Action(52)}, { 26, Action(61, {62})}, { 27, Action(61, {62})}, { 28, Action(61, {62})}, { 29, Action(61, {62})}, { 30, Action(61, {62})}, { 31, Action(61, {62})}, { 32, Action(61, {62})}, { 33, Action(61, {62})}, { 34, Action(61, {62})}, { 35, Action(61, {62})}, { 36, Action(61, {62})}}},
{ 10, {{ -1, Action(62, {63})}, { 16, Action(62, {63})}, { 18, Action(53)}, { 19, Action(54)}, { 24, Action(62, {63})}, { 25, Action(62, {63})}, { 26, Action(62, {63})}, { 27, Action(62, {63})}, { 28, Action(62, {63})}, { 29, Action(62, {63})}, { 30, Action(62, {63})}, { 31, Action(62, {63})}, { 32, Action(62, {63})}, { 33, Action(62, {63})}, { 34, Action(62, {63})}, { 35, Action(62, {63})}, { 36, Action(62, {63})}}},
{ 11, {{ -1, Action(63, {64})}, { 16, Action(63, {64})}, { 17, Action(55)}, { 18, Action(63, {64})}, { 19, Action(63, {64})}, { 22, Action(56)}, { 23, Action(57)}, { 24, Action(63, {64})}, { 25, Action(63, {64})}, { 26, Action(63, {64})}, { 27, Action(63, {64})}, { 28, Action(63, {64})}, { 29, Action(63, {64})}, { 30, Action(63, {64})}, { 31, Action(63, {64})}, { 32, Action(63, {64})}, { 33, Action(63, {64})}, { 34, Action(63, {64})}, { 35, Action(63, {64})}, { 36, Action(63, {64})}}},
{ 12, {{ -1, Action(64, {65})}, { 16, Action(64, {65})}, { 17, Action(64, {65})}, { 18, Action(64, {65})}, { 19, Action(64, {65})}, { 22, Action(64, {65})}, { 23, Action(64, {65})}, { 24, Action(64, {65})}, { 25, Action(64, {65})}, { 26, Action(64, {65})}, { 27, Action(64, {65})}, { 28, Action(64, {65})}, { 29, Action(64, {65})}, { 30, Action(64, {65})}, { 31, Action(64, {65})}, { 32, Action(64, {65})}, { 33, Action(64, {65})}, { 34, Action(64, {65})}, { 35, Action(64, {65})}, { 36, Action(64, {65})}}},
{ 13, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 14, {{ -1, Action(65, {67})}, { 16, Action(65, {67})}, { 17, Action(65, {67})}, { 18, Action(65, {67})}, { 19, Action(65, {67})}, { 22, Action(65, {67})}, { 23, Action(65, {67})}, { 24, Action(65, {67})}, { 25, Action(65, {67})}, { 26, Action(65, {67})}, { 27, Action(65, {67})}, { 28, Action(65, {67})}, { 29, Action(65, {67})}, { 30, Action(65, {67})}, { 31, Action(65, {67})}, { 32, Action(65, {67})}, { 33, Action(65, {67})}, { 34, Action(65, {67})}, { 35, Action(65, {67})}, { 36, Action(65, {67})}}},
{ 15, {{ 2, Action(66, {18})}, { 4, Action(66, {18})}, { 8, Action(66, {18})}, { 18, Action(66, {18})}, { 19, Action(66, {18})}, { 20, Action(66, {18})}, { 21, Action(66, {18})}}},
{ 16, {{ 2, Action(66, {19})}, { 4, Action(66, {19})}, { 8, Action(66, {19})}, { 18, Action(66, {19})}, { 19, Action(66, {19})}, { 20, Action(66, {19})}, { 21, Action(66, {19})}}},
{ 17, {{ 2, Action(66, {20})}, { 4, Action(66, {20})}, { 8, Action(66, {20})}, { 18, Action(66, {20})}, { 19, Action(66, {20})}, { 20, Action(66, {20})}, { 21, Action(66, {20})}}},
{ 18, {{ 2, Action(66, {21})}, { 4, Action(66, {21})}, { 8, Action(66, {21})}, { 18, Action(66, {21})}, { 19, Action(66, {21})}, { 20, Action(66, {21})}, { 21, Action(66, {21})}}},
{ 19, {{ -1, Action(67, {2})}, { 16, Action(67, {2})}, { 17, Action(67, {2})}, { 18, Action(67, {2})}, { 19, Action(67, {2})}, { 22, Action(67, {2})}, { 23, Action(67, {2})}, { 24, Action(67, {2})}, { 25, Action(67, {2})}, { 26, Action(67, {2})}, { 27, Action(67, {2})}, { 28, Action(67, {2})}, { 29, Action(67, {2})}, { 30, Action(67, {2})}, { 31, Action(67, {2})}, { 32, Action(67, {2})}, { 33, Action(67, {2})}, { 34, Action(67, {2})}, { 35, Action(67, {2})}, { 36, Action(67, {2})}}},
{ 20, {{ -1, Action(67, {4})}, { 16, Action(67, {4})}, { 17, Action(67, {4})}, { 18, Action(67, {4})}, { 19, Action(67, {4})}, { 22, Action(67, {4})}, { 23, Action(67, {4})}, { 24, Action(67, {4})}, { 25, Action(67, {4})}, { 26, Action(67, {4})}, { 27, Action(67, {4})}, { 28, Action(67, {4})}, { 29, Action(67, {4})}, { 30, Action(67, {4})}, { 31, Action(67, {4})}, { 32, Action(67, {4})}, { 33, Action(67, {4})}, { 34, Action(67, {4})}, { 35, Action(67, {4})}, { 36, Action(67, {4})}}},
{ 21, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 22, {{ 9, Action(54, {55})}, { 35, Action(78)}, { 36, Action(77)}}},
{ 23, {{ 9, Action(55, {56})}, { 34, Action(80)}, { 35, Action(55, {56})}, { 36, Action(55, {56})}}},
{ 24, {{ 9, Action(56, {57})}, { 33, Action(82)}, { 34, Action(56, {57})}, { 35, Action(56, {57})}, { 36, Action(56, {57})}}},
{ 25, {{ 9, Action(57, {58})}, { 32, Action(84)}, { 33, Action(57, {58})}, { 34, Action(57, {58})}, { 35, Action(57, {58})}, { 36, Action(57, {58})}}},
{ 26, {{ 9, Action(58, {59})}, { 16, Action(86)}, { 32, Action(58, {59})}, { 33, Action(58, {59})}, { 34, Action(58, {59})}, { 35, Action(58, {59})}, { 36, Action(58, {59})}}},
{ 27, {{ 9, Action(59, {60})}, { 16, Action(59, {60})}, { 30, Action(88)}, { 31, Action(89)}, { 32, Action(59, {60})}, { 33, Action(59, {60})}, { 34, Action(59, {60})}, { 35, Action(59, {60})}, { 36, Action(59, {60})}}},
{ 28, {{ 9, Action(60, {61})}, { 16, Action(60, {61})}, { 26, Action(92)}, { 27, Action(93)}, { 28, Action(94)}, { 29, Action(95)}, { 30, Action(60, {61})}, { 31, Action(60, {61})}, { 32, Action(60, {61})}, { 33, Action(60, {61})}, { 34, Action(60, {61})}, { 35, Action(60, {61})}, { 36, Action(60, {61})}}},
{ 29, {{ 9, Action(61, {62})}, { 16, Action(61, {62})}, { 24, Action(100)}, { 25, Action(101)}, { 26, Action(61, {62})}, { 27, Action(61, {62})}, { 28, Action(61, {62})}, { 29, Action(61, {62})}, { 30, Action(61, {62})}, { 31, Action(61, {62})}, { 32, Action(61, {62})}, { 33, Action(61, {62})}, { 34, Action(61, {62})}, { 35, Action(61, {62})}, { 36, Action(61, {62})}}},
{ 30, {{ 9, Action(62, {63})}, { 16, Action(62, {63})}, { 18, Action(104)}, { 19, Action(105)}, { 24, Action(62, {63})}, { 25, Action(62, {63})}, { 26, Action(62, {63})}, { 27, Action(62, {63})}, { 28, Action(62, {63})}, { 29, Action(62, {63})}, { 30, Action(62, {63})}, { 31, Action(62, {63})}, { 32, Action(62, {63})}, { 33, Action(62, {63})}, { 34, Action(62, {63})}, { 35, Action(62, {63})}, { 36, Action(62, {63})}}},
{ 31, {{ 9, Action(63, {64})}, { 16, Action(63, {64})}, { 17, Action(108)}, { 18, Action(63, {64})}, { 19, Action(63, {64})}, { 22, Action(109)}, { 23, Action(110)}, { 24, Action(63, {64})}, { 25, Action(63, {64})}, { 26, Action(63, {64})}, { 27, Action(63, {64})}, { 28, Action(63, {64})}, { 29, Action(63, {64})}, { 30, Action(63, {64})}, { 31, Action(63, {64})}, { 32, Action(63, {64})}, { 33, Action(63, {64})}, { 34, Action(63, {64})}, { 35, Action(63, {64})}, { 36, Action(63, {64})}}},
{ 32, {{ 9, Action(64, {65})}, { 16, Action(64, {65})}, { 17, Action(64, {65})}, { 18, Action(64, {65})}, { 19, Action(64, {65})}, { 22, Action(64, {65})}, { 23, Action(64, {65})}, { 24, Action(64, {65})}, { 25, Action(64, {65})}, { 26, Action(64, {65})}, { 27, Action(64, {65})}, { 28, Action(64, {65})}, { 29, Action(64, {65})}, { 30, Action(64, {65})}, { 31, Action(64, {65})}, { 32, Action(64, {65})}, { 33, Action(64, {65})}, { 34, Action(64, {65})}, { 35, Action(64, {65})}, { 36, Action(64, {65})}}},
{ 33, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 34, {{ 9, Action(65, {67})}, { 16, Action(65, {67})}, { 17, Action(65, {67})}, { 18, Action(65, {67})}, { 19, Action(65, {67})}, { 22, Action(65, {67})}, { 23, Action(65, {67})}, { 24, Action(65, {67})}, { 25, Action(65, {67})}, { 26, Action(65, {67})}, { 27, Action(65, {67})}, { 28, Action(65, {67})}, { 29, Action(65, {67})}, { 30, Action(65, {67})}, { 31, Action(65, {67})}, { 32, Action(65, {67})}, { 33, Action(65, {67})}, { 34, Action(65, {67})}, { 35, Action(65, {67})}, { 36, Action(65, {67})}}},
{ 35, {{ 9, Action(67, {2})}, { 16, Action(67, {2})}, { 17, Action(67, {2})}, { 18, Action(67, {2})}, { 19, Action(67, {2})}, { 22, Action(67, {2})}, { 23, Action(67, {2})}, { 24, Action(67, {2})}, { 25, Action(67, {2})}, { 26, Action(67, {2})}, { 27, Action(67, {2})}, { 28, Action(67, {2})}, { 29, Action(67, {2})}, { 30, Action(67, {2})}, { 31, Action(67, {2})}, { 32, Action(67, {2})}, { 33, Action(67, {2})}, { 34, Action(67, {2})}, { 35, Action(67, {2})}, { 36, Action(67, {2})}}},
{ 36, {{ 9, Action(67, {4})}, { 16, Action(67, {4})}, { 17, Action(67, {4})}, { 18, Action(67, {4})}, { 19, Action(67, {4})}, { 22, Action(67, {4})}, { 23, Action(67, {4})}, { 24, Action(67, {4})}, { 25, Action(67, {4})}, { 26, Action(67, {4})}, { 27, Action(67, {4})}, { 28, Action(67, {4})}, { 29, Action(67, {4})}, { 30, Action(67, {4})}, { 31, Action(67, {4})}, { 32, Action(67, {4})}, { 33, Action(67, {4})}, { 34, Action(67, {4})}, { 35, Action(67, {4})}, { 36, Action(67, {4})}}},
{ 37, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 38, {{ 9, Action(115)}}},
{ 39, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 40, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 41, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 42, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 43, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 44, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 45, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 46, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 47, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 48, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 49, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 50, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 51, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 52, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 53, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 54, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 55, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 56, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 57, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 58, {{ -1, Action(65, {66, 65})}, { 16, Action(65, {66, 65})}, { 17, Action(65, {66, 65})}, { 18, Action(65, {66, 65})}, { 19, Action(65, {66, 65})}, { 22, Action(65, {66, 65})}, { 23, Action(65, {66, 65})}, { 24, Action(65, {66, 65})}, { 25, Action(65, {66, 65})}, { 26, Action(65, {66, 65})}, { 27, Action(65, {66, 65})}, { 28, Action(65, {66, 65})}, { 29, Action(65, {66, 65})}, { 30, Action(65, {66, 65})}, { 31, Action(65, {66, 65})}, { 32, Action(65, {66, 65})}, { 33, Action(65, {66, 65})}, { 34, Action(65, {66, 65})}, { 35, Action(65, {66, 65})}, { 36, Action(65, {66, 65})}}},
{ 59, {{ 9, Action(157)}}},
{ 60, {{ 35, Action(119)}, { 36, Action(118)}, { 37, Action(54, {55})}}},
{ 61, {{ 37, Action(120)}}},
{ 62, {{ 34, Action(122)}, { 35, Action(55, {56})}, { 36, Action(55, {56})}, { 37, Action(55, {56})}}},
{ 63, {{ 33, Action(124)}, { 34, Action(56, {57})}, { 35, Action(56, {57})}, { 36, Action(56, {57})}, { 37, Action(56, {57})}}},
{ 64, {{ 32, Action(126)}, { 33, Action(57, {58})}, { 34, Action(57, {58})}, { 35, Action(57, {58})}, { 36, Action(57, {58})}, { 37, Action(57, {58})}}},
{ 65, {{ 16, Action(128)}, { 32, Action(58, {59})}, { 33, Action(58, {59})}, { 34, Action(58, {59})}, { 35, Action(58, {59})}, { 36, Action(58, {59})}, { 37, Action(58, {59})}}},
{ 66, {{ 16, Action(59, {60})}, { 30, Action(130)}, { 31, Action(131)}, { 32, Action(59, {60})}, { 33, Action(59, {60})}, { 34, Action(59, {60})}, { 35, Action(59, {60})}, { 36, Action(59, {60})}, { 37, Action(59, {60})}}},
{ 67, {{ 16, Action(60, {61})}, { 26, Action(134)}, { 27, Action(135)}, { 28, Action(136)}, { 29, Action(137)}, { 30, Action(60, {61})}, { 31, Action(60, {61})}, { 32, Action(60, {61})}, { 33, Action(60, {61})}, { 34, Action(60, {61})}, { 35, Action(60, {61})}, { 36, Action(60, {61})}, { 37, Action(60, {61})}}},
{ 68, {{ 16, Action(61, {62})}, { 24, Action(142)}, { 25, Action(143)}, { 26, Action(61, {62})}, { 27, Action(61, {62})}, { 28, Action(61, {62})}, { 29, Action(61, {62})}, { 30, Action(61, {62})}, { 31, Action(61, {62})}, { 32, Action(61, {62})}, { 33, Action(61, {62})}, { 34, Action(61, {62})}, { 35, Action(61, {62})}, { 36, Action(61, {62})}, { 37, Action(61, {62})}}},
{ 69, {{ 16, Action(62, {63})}, { 18, Action(146)}, { 19, Action(147)}, { 24, Action(62, {63})}, { 25, Action(62, {63})}, { 26, Action(62, {63})}, { 27, Action(62, {63})}, { 28, Action(62, {63})}, { 29, Action(62, {63})}, { 30, Action(62, {63})}, { 31, Action(62, {63})}, { 32, Action(62, {63})}, { 33, Action(62, {63})}, { 34, Action(62, {63})}, { 35, Action(62, {63})}, { 36, Action(62, {63})}, { 37, Action(62, {63})}}},
{ 70, {{ 16, Action(63, {64})}, { 17, Action(150)}, { 18, Action(63, {64})}, { 19, Action(63, {64})}, { 22, Action(151)}, { 23, Action(152)}, { 24, Action(63, {64})}, { 25, Action(63, {64})}, { 26, Action(63, {64})}, { 27, Action(63, {64})}, { 28, Action(63, {64})}, { 29, Action(63, {64})}, { 30, Action(63, {64})}, { 31, Action(63, {64})}, { 32, Action(63, {64})}, { 33, Action(63, {64})}, { 34, Action(63, {64})}, { 35, Action(63, {64})}, { 36, Action(63, {64})}, { 37, Action(63, {64})}}},
{ 71, {{ 16, Action(64, {65})}, { 17, Action(64, {65})}, { 18, Action(64, {65})}, { 19, Action(64, {65})}, { 22, Action(64, {65})}, { 23, Action(64, {65})}, { 24, Action(64, {65})}, { 25, Action(64, {65})}, { 26, Action(64, {65})}, { 27, Action(64, {65})}, { 28, Action(64, {65})}, { 29, Action(64, {65})}, { 30, Action(64, {65})}, { 31, Action(64, {65})}, { 32, Action(64, {65})}, { 33, Action(64, {65})}, { 34, Action(64, {65})}, { 35, Action(64, {65})}, { 36, Action(64, {65})}, { 37, Action(64, {65})}}},
{ 72, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 73, {{ 16, Action(65, {67})}, { 17, Action(65, {67})}, { 18, Action(65, {67})}, { 19, Action(65, {67})}, { 22, Action(65, {67})}, { 23, Action(65, {67})}, { 24, Action(65, {67})}, { 25, Action(65, {67})}, { 26, Action(65, {67})}, { 27, Action(65, {67})}, { 28, Action(65, {67})}, { 29, Action(65, {67})}, { 30, Action(65, {67})}, { 31, Action(65, {67})}, { 32, Action(65, {67})}, { 33, Action(65, {67})}, { 34, Action(65, {67})}, { 35, Action(65, {67})}, { 36, Action(65, {67})}, { 37, Action(65, {67})}}},
{ 74, {{ 16, Action(67, {2})}, { 17, Action(67, {2})}, { 18, Action(67, {2})}, { 19, Action(67, {2})}, { 22, Action(67, {2})}, { 23, Action(67, {2})}, { 24, Action(67, {2})}, { 25, Action(67, {2})}, { 26, Action(67, {2})}, { 27, Action(67, {2})}, { 28, Action(67, {2})}, { 29, Action(67, {2})}, { 30, Action(67, {2})}, { 31, Action(67, {2})}, { 32, Action(67, {2})}, { 33, Action(67, {2})}, { 34, Action(67, {2})}, { 35, Action(67, {2})}, { 36, Action(67, {2})}, { 37, Action(67, {2})}}},
{ 75, {{ 16, Action(67, {4})}, { 17, Action(67, {4})}, { 18, Action(67, {4})}, { 19, Action(67, {4})}, { 22, Action(67, {4})}, { 23, Action(67, {4})}, { 24, Action(67, {4})}, { 25, Action(67, {4})}, { 26, Action(67, {4})}, { 27, Action(67, {4})}, { 28, Action(67, {4})}, { 29, Action(67, {4})}, { 30, Action(67, {4})}, { 31, Action(67, {4})}, { 32, Action(67, {4})}, { 33, Action(67, {4})}, { 34, Action(67, {4})}, { 35, Action(67, {4})}, { 36, Action(67, {4})}, { 37, Action(67, {4})}}},
{ 76, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 77, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 78, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 79, {{ -1, Action(55, {55, 35, 56})}, { 34, Action(41)}, { 35, Action(55, {55, 35, 56})}, { 36, Action(55, {55, 35, 56})}}},
{ 80, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 81, {{ -1, Action(56, {56, 34, 57})}, { 33, Action(42)}, { 34, Action(56, {56, 34, 57})}, { 35, Action(56, {56, 34, 57})}, { 36, Action(56, {56, 34, 57})}}},
{ 82, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 83, {{ -1, Action(57, {57, 33, 58})}, { 32, Action(43)}, { 33, Action(57, {57, 33, 58})}, { 34, Action(57, {57, 33, 58})}, { 35, Action(57, {57, 33, 58})}, { 36, Action(57, {57, 33, 58})}}},
{ 84, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 85, {{ -1, Action(58, {58, 32, 59})}, { 16, Action(44)}, { 32, Action(58, {58, 32, 59})}, { 33, Action(58, {58, 32, 59})}, { 34, Action(58, {58, 32, 59})}, { 35, Action(58, {58, 32, 59})}, { 36, Action(58, {58, 32, 59})}}},
{ 86, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 87, {{ -1, Action(59, {59, 16, 60})}, { 16, Action(59, {59, 16, 60})}, { 30, Action(45)}, { 31, Action(46)}, { 32, Action(59, {59, 16, 60})}, { 33, Action(59, {59, 16, 60})}, { 34, Action(59, {59, 16, 60})}, { 35, Action(59, {59, 16, 60})}, { 36, Action(59, {59, 16, 60})}}},
{ 88, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 89, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 90, {{ -1, Action(60, {60, 30, 61})}, { 16, Action(60, {60, 30, 61})}, { 26, Action(47)}, { 27, Action(48)}, { 28, Action(49)}, { 29, Action(50)}, { 30, Action(60, {60, 30, 61})}, { 31, Action(60, {60, 30, 61})}, { 32, Action(60, {60, 30, 61})}, { 33, Action(60, {60, 30, 61})}, { 34, Action(60, {60, 30, 61})}, { 35, Action(60, {60, 30, 61})}, { 36, Action(60, {60, 30, 61})}}},
{ 91, {{ -1, Action(60, {60, 31, 61})}, { 16, Action(60, {60, 31, 61})}, { 26, Action(47)}, { 27, Action(48)}, { 28, Action(49)}, { 29, Action(50)}, { 30, Action(60, {60, 31, 61})}, { 31, Action(60, {60, 31, 61})}, { 32, Action(60, {60, 31, 61})}, { 33, Action(60, {60, 31, 61})}, { 34, Action(60, {60, 31, 61})}, { 35, Action(60, {60, 31, 61})}, { 36, Action(60, {60, 31, 61})}}},
{ 92, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 93, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 94, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 95, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 96, {{ -1, Action(61, {61, 26, 62})}, { 16, Action(61, {61, 26, 62})}, { 24, Action(51)}, { 25, Action(52)}, { 26, Action(61, {61, 26, 62})}, { 27, Action(61, {61, 26, 62})}, { 28, Action(61, {61, 26, 62})}, { 29, Action(61, {61, 26, 62})}, { 30, Action(61, {61, 26, 62})}, { 31, Action(61, {61, 26, 62})}, { 32, Action(61, {61, 26, 62})}, { 33, Action(61, {61, 26, 62})}, { 34, Action(61, {61, 26, 62})}, { 35, Action(61, {61, 26, 62})}, { 36, Action(61, {61, 26, 62})}}},
{ 97, {{ -1, Action(61, {61, 27, 62})}, { 16, Action(61, {61, 27, 62})}, { 24, Action(51)}, { 25, Action(52)}, { 26, Action(61, {61, 27, 62})}, { 27, Action(61, {61, 27, 62})}, { 28, Action(61, {61, 27, 62})}, { 29, Action(61, {61, 27, 62})}, { 30, Action(61, {61, 27, 62})}, { 31, Action(61, {61, 27, 62})}, { 32, Action(61, {61, 27, 62})}, { 33, Action(61, {61, 27, 62})}, { 34, Action(61, {61, 27, 62})}, { 35, Action(61, {61, 27, 62})}, { 36, Action(61, {61, 27, 62})}}},
{ 98, {{ -1, Action(61, {61, 28, 62})}, { 16, Action(61, {61, 28, 62})}, { 24, Action(51)}, { 25, Action(52)}, { 26, Action(61, {61, 28, 62})}, { 27, Action(61, {61, 28, 62})}, { 28, Action(61, {61, 28, 62})}, { 29, Action(61, {61, 28, 62})}, { 30, Action(61, {61, 28, 62})}, { 31, Action(61, {61, 28, 62})}, { 32, Action(61, {61, 28, 62})}, { 33, Action(61, {61, 28, 62})}, { 34, Action(61, {61, 28, 62})}, { 35, Action(61, {61, 28, 62})}, { 36, Action(61, {61, 28, 62})}}},
{ 99, {{ -1, Action(61, {61, 29, 62})}, { 16, Action(61, {61, 29, 62})}, { 24, Action(51)}, { 25, Action(52)}, { 26, Action(61, {61, 29, 62})}, { 27, Action(61, {61, 29, 62})}, { 28, Action(61, {61, 29, 62})}, { 29, Action(61, {61, 29, 62})}, { 30, Action(61, {61, 29, 62})}, { 31, Action(61, {61, 29, 62})}, { 32, Action(61, {61, 29, 62})}, { 33, Action(61, {61, 29, 62})}, { 34, Action(61, {61, 29, 62})}, { 35, Action(61, {61, 29, 62})}, { 36, Action(61, {61, 29, 62})}}},
{ 100, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 101, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 102, {{ -1, Action(62, {62, 24, 63})}, { 16, Action(62, {62, 24, 63})}, { 18, Action(53)}, { 19, Action(54)}, { 24, Action(62, {62, 24, 63})}, { 25, Action(62, {62, 24, 63})}, { 26, Action(62, {62, 24, 63})}, { 27, Action(62, {62, 24, 63})}, { 28, Action(62, {62, 24, 63})}, { 29, Action(62, {62, 24, 63})}, { 30, Action(62, {62, 24, 63})}, { 31, Action(62, {62, 24, 63})}, { 32, Action(62, {62, 24, 63})}, { 33, Action(62, {62, 24, 63})}, { 34, Action(62, {62, 24, 63})}, { 35, Action(62, {62, 24, 63})}, { 36, Action(62, {62, 24, 63})}}},
{ 103, {{ -1, Action(62, {62, 25, 63})}, { 16, Action(62, {62, 25, 63})}, { 18, Action(53)}, { 19, Action(54)}, { 24, Action(62, {62, 25, 63})}, { 25, Action(62, {62, 25, 63})}, { 26, Action(62, {62, 25, 63})}, { 27, Action(62, {62, 25, 63})}, { 28, Action(62, {62, 25, 63})}, { 29, Action(62, {62, 25, 63})}, { 30, Action(62, {62, 25, 63})}, { 31, Action(62, {62, 25, 63})}, { 32, Action(62, {62, 25, 63})}, { 33, Action(62, {62, 25, 63})}, { 34, Action(62, {62, 25, 63})}, { 35, Action(62, {62, 25, 63})}, { 36, Action(62, {62, 25, 63})}}},
{ 104, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 105, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 106, {{ -1, Action(63, {63, 18, 64})}, { 16, Action(63, {63, 18, 64})}, { 17, Action(55)}, { 18, Action(63, {63, 18, 64})}, { 19, Action(63, {63, 18, 64})}, { 22, Action(56)}, { 23, Action(57)}, { 24, Action(63, {63, 18, 64})}, { 25, Action(63, {63, 18, 64})}, { 26, Action(63, {63, 18, 64})}, { 27, Action(63, {63, 18, 64})}, { 28, Action(63, {63, 18, 64})}, { 29, Action(63, {63, 18, 64})}, { 30, Action(63, {63, 18, 64})}, { 31, Action(63, {63, 18, 64})}, { 32, Action(63, {63, 18, 64})}, { 33, Action(63, {63, 18, 64})}, { 34, Action(63, {63, 18, 64})}, { 35, Action(63, {63, 18, 64})}, { 36, Action(63, {63, 18, 64})}}},
{ 107, {{ -1, Action(63, {63, 19, 64})}, { 16, Action(63, {63, 19, 64})}, { 17, Action(55)}, { 18, Action(63, {63, 19, 64})}, { 19, Action(63, {63, 19, 64})}, { 22, Action(56)}, { 23, Action(57)}, { 24, Action(63, {63, 19, 64})}, { 25, Action(63, {63, 19, 64})}, { 26, Action(63, {63, 19, 64})}, { 27, Action(63, {63, 19, 64})}, { 28, Action(63, {63, 19, 64})}, { 29, Action(63, {63, 19, 64})}, { 30, Action(63, {63, 19, 64})}, { 31, Action(63, {63, 19, 64})}, { 32, Action(63, {63, 19, 64})}, { 33, Action(63, {63, 19, 64})}, { 34, Action(63, {63, 19, 64})}, { 35, Action(63, {63, 19, 64})}, { 36, Action(63, {63, 19, 64})}}},
{ 108, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 109, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 110, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 111, {{ -1, Action(64, {64, 17, 65})}, { 16, Action(64, {64, 17, 65})}, { 17, Action(64, {64, 17, 65})}, { 18, Action(64, {64, 17, 65})}, { 19, Action(64, {64, 17, 65})}, { 22, Action(64, {64, 17, 65})}, { 23, Action(64, {64, 17, 65})}, { 24, Action(64, {64, 17, 65})}, { 25, Action(64, {64, 17, 65})}, { 26, Action(64, {64, 17, 65})}, { 27, Action(64, {64, 17, 65})}, { 28, Action(64, {64, 17, 65})}, { 29, Action(64, {64, 17, 65})}, { 30, Action(64, {64, 17, 65})}, { 31, Action(64, {64, 17, 65})}, { 32, Action(64, {64, 17, 65})}, { 33, Action(64, {64, 17, 65})}, { 34, Action(64, {64, 17, 65})}, { 35, Action(64, {64, 17, 65})}, { 36, Action(64, {64, 17, 65})}}},
{ 112, {{ -1, Action(64, {64, 22, 65})}, { 16, Action(64, {64, 22, 65})}, { 17, Action(64, {64, 22, 65})}, { 18, Action(64, {64, 22, 65})}, { 19, Action(64, {64, 22, 65})}, { 22, Action(64, {64, 22, 65})}, { 23, Action(64, {64, 22, 65})}, { 24, Action(64, {64, 22, 65})}, { 25, Action(64, {64, 22, 65})}, { 26, Action(64, {64, 22, 65})}, { 27, Action(64, {64, 22, 65})}, { 28, Action(64, {64, 22, 65})}, { 29, Action(64, {64, 22, 65})}, { 30, Action(64, {64, 22, 65})}, { 31, Action(64, {64, 22, 65})}, { 32, Action(64, {64, 22, 65})}, { 33, Action(64, {64, 22, 65})}, { 34, Action(64, {64, 22, 65})}, { 35, Action(64, {64, 22, 65})}, { 36, Action(64, {64, 22, 65})}}},
{ 113, {{ -1, Action(64, {64, 23, 65})}, { 16, Action(64, {64, 23, 65})}, { 17, Action(64, {64, 23, 65})}, { 18, Action(64, {64, 23, 65})}, { 19, Action(64, {64, 23, 65})}, { 22, Action(64, {64, 23, 65})}, { 23, Action(64, {64, 23, 65})}, { 24, Action(64, {64, 23, 65})}, { 25, Action(64, {64, 23, 65})}, { 26, Action(64, {64, 23, 65})}, { 27, Action(64, {64, 23, 65})}, { 28, Action(64, {64, 23, 65})}, { 29, Action(64, {64, 23, 65})}, { 30, Action(64, {64, 23, 65})}, { 31, Action(64, {64, 23, 65})}, { 32, Action(64, {64, 23, 65})}, { 33, Action(64, {64, 23, 65})}, { 34, Action(64, {64, 23, 65})}, { 35, Action(64, {64, 23, 65})}, { 36, Action(64, {64, 23, 65})}}},
{ 114, {{ 9, Action(65, {66, 65})}, { 16, Action(65, {66, 65})}, { 17, Action(65, {66, 65})}, { 18, Action(65, {66, 65})}, { 19, Action(65, {66, 65})}, { 22, Action(65, {66, 65})}, { 23, Action(65, {66, 65})}, { 24, Action(65, {66, 65})}, { 25, Action(65, {66, 65})}, { 26, Action(65, {66, 65})}, { 27, Action(65, {66, 65})}, { 28, Action(65, {66, 65})}, { 29, Action(65, {66, 65})}, { 30, Action(65, {66, 65})}, { 31, Action(65, {66, 65})}, { 32, Action(65, {66, 65})}, { 33, Action(65, {66, 65})}, { 34, Action(65, {66, 65})}, { 35, Action(65, {66, 65})}, { 36, Action(65, {66, 65})}}},
{ 115, {{ -1, Action(67, {8, 54, 9})}, { 16, Action(67, {8, 54, 9})}, { 17, Action(67, {8, 54, 9})}, { 18, Action(67, {8, 54, 9})}, { 19, Action(67, {8, 54, 9})}, { 22, Action(67, {8, 54, 9})}, { 23, Action(67, {8, 54, 9})}, { 24, Action(67, {8, 54, 9})}, { 25, Action(67, {8, 54, 9})}, { 26, Action(67, {8, 54, 9})}, { 27, Action(67, {8, 54, 9})}, { 28, Action(67, {8, 54, 9})}, { 29, Action(67, {8, 54, 9})}, { 30, Action(67, {8, 54, 9})}, { 31, Action(67, {8, 54, 9})}, { 32, Action(67, {8, 54, 9})}, { 33, Action(67, {8, 54, 9})}, { 34, Action(67, {8, 54, 9})}, { 35, Action(67, {8, 54, 9})}, { 36, Action(67, {8, 54, 9})}}},
{ 116, {{ 9, Action(179)}}},
{ 117, {{ 37, Action(160)}}},
{ 118, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 119, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 120, {{ 2, Action(19)}, { 4, Action(20)}, { 8, Action(21)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 121, {{ 9, Action(55, {55, 35, 56})}, { 34, Action(80)}, { 35, Action(55, {55, 35, 56})}, { 36, Action(55, {55, 35, 56})}}},
{ 122, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 123, {{ 9, Action(56, {56, 34, 57})}, { 33, Action(82)}, { 34, Action(56, {56, 34, 57})}, { 35, Action(56, {56, 34, 57})}, { 36, Action(56, {56, 34, 57})}}},
{ 124, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 125, {{ 9, Action(57, {57, 33, 58})}, { 32, Action(84)}, { 33, Action(57, {57, 33, 58})}, { 34, Action(57, {57, 33, 58})}, { 35, Action(57, {57, 33, 58})}, { 36, Action(57, {57, 33, 58})}}},
{ 126, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 127, {{ 9, Action(58, {58, 32, 59})}, { 16, Action(86)}, { 32, Action(58, {58, 32, 59})}, { 33, Action(58, {58, 32, 59})}, { 34, Action(58, {58, 32, 59})}, { 35, Action(58, {58, 32, 59})}, { 36, Action(58, {58, 32, 59})}}},
{ 128, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 129, {{ 9, Action(59, {59, 16, 60})}, { 16, Action(59, {59, 16, 60})}, { 30, Action(88)}, { 31, Action(89)}, { 32, Action(59, {59, 16, 60})}, { 33, Action(59, {59, 16, 60})}, { 34, Action(59, {59, 16, 60})}, { 35, Action(59, {59, 16, 60})}, { 36, Action(59, {59, 16, 60})}}},
{ 130, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 131, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 132, {{ 9, Action(60, {60, 30, 61})}, { 16, Action(60, {60, 30, 61})}, { 26, Action(92)}, { 27, Action(93)}, { 28, Action(94)}, { 29, Action(95)}, { 30, Action(60, {60, 30, 61})}, { 31, Action(60, {60, 30, 61})}, { 32, Action(60, {60, 30, 61})}, { 33, Action(60, {60, 30, 61})}, { 34, Action(60, {60, 30, 61})}, { 35, Action(60, {60, 30, 61})}, { 36, Action(60, {60, 30, 61})}}},
{ 133, {{ 9, Action(60, {60, 31, 61})}, { 16, Action(60, {60, 31, 61})}, { 26, Action(92)}, { 27, Action(93)}, { 28, Action(94)}, { 29, Action(95)}, { 30, Action(60, {60, 31, 61})}, { 31, Action(60, {60, 31, 61})}, { 32, Action(60, {60, 31, 61})}, { 33, Action(60, {60, 31, 61})}, { 34, Action(60, {60, 31, 61})}, { 35, Action(60, {60, 31, 61})}, { 36, Action(60, {60, 31, 61})}}},
{ 134, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 135, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 136, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 137, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 138, {{ 9, Action(61, {61, 26, 62})}, { 16, Action(61, {61, 26, 62})}, { 24, Action(100)}, { 25, Action(101)}, { 26, Action(61, {61, 26, 62})}, { 27, Action(61, {61, 26, 62})}, { 28, Action(61, {61, 26, 62})}, { 29, Action(61, {61, 26, 62})}, { 30, Action(61, {61, 26, 62})}, { 31, Action(61, {61, 26, 62})}, { 32, Action(61, {61, 26, 62})}, { 33, Action(61, {61, 26, 62})}, { 34, Action(61, {61, 26, 62})}, { 35, Action(61, {61, 26, 62})}, { 36, Action(61, {61, 26, 62})}}},
{ 139, {{ 9, Action(61, {61, 27, 62})}, { 16, Action(61, {61, 27, 62})}, { 24, Action(100)}, { 25, Action(101)}, { 26, Action(61, {61, 27, 62})}, { 27, Action(61, {61, 27, 62})}, { 28, Action(61, {61, 27, 62})}, { 29, Action(61, {61, 27, 62})}, { 30, Action(61, {61, 27, 62})}, { 31, Action(61, {61, 27, 62})}, { 32, Action(61, {61, 27, 62})}, { 33, Action(61, {61, 27, 62})}, { 34, Action(61, {61, 27, 62})}, { 35, Action(61, {61, 27, 62})}, { 36, Action(61, {61, 27, 62})}}},
{ 140, {{ 9, Action(61, {61, 28, 62})}, { 16, Action(61, {61, 28, 62})}, { 24, Action(100)}, { 25, Action(101)}, { 26, Action(61, {61, 28, 62})}, { 27, Action(61, {61, 28, 62})}, { 28, Action(61, {61, 28, 62})}, { 29, Action(61, {61, 28, 62})}, { 30, Action(61, {61, 28, 62})}, { 31, Action(61, {61, 28, 62})}, { 32, Action(61, {61, 28, 62})}, { 33, Action(61, {61, 28, 62})}, { 34, Action(61, {61, 28, 62})}, { 35, Action(61, {61, 28, 62})}, { 36, Action(61, {61, 28, 62})}}},
{ 141, {{ 9, Action(61, {61, 29, 62})}, { 16, Action(61, {61, 29, 62})}, { 24, Action(100)}, { 25, Action(101)}, { 26, Action(61, {61, 29, 62})}, { 27, Action(61, {61, 29, 62})}, { 28, Action(61, {61, 29, 62})}, { 29, Action(61, {61, 29, 62})}, { 30, Action(61, {61, 29, 62})}, { 31, Action(61, {61, 29, 62})}, { 32, Action(61, {61, 29, 62})}, { 33, Action(61, {61, 29, 62})}, { 34, Action(61, {61, 29, 62})}, { 35, Action(61, {61, 29, 62})}, { 36, Action(61, {61, 29, 62})}}},
{ 142, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 143, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 144, {{ 9, Action(62, {62, 24, 63})}, { 16, Action(62, {62, 24, 63})}, { 18, Action(104)}, { 19, Action(105)}, { 24, Action(62, {62, 24, 63})}, { 25, Action(62, {62, 24, 63})}, { 26, Action(62, {62, 24, 63})}, { 27, Action(62, {62, 24, 63})}, { 28, Action(62, {62, 24, 63})}, { 29, Action(62, {62, 24, 63})}, { 30, Action(62, {62, 24, 63})}, { 31, Action(62, {62, 24, 63})}, { 32, Action(62, {62, 24, 63})}, { 33, Action(62, {62, 24, 63})}, { 34, Action(62, {62, 24, 63})}, { 35, Action(62, {62, 24, 63})}, { 36, Action(62, {62, 24, 63})}}},
{ 145, {{ 9, Action(62, {62, 25, 63})}, { 16, Action(62, {62, 25, 63})}, { 18, Action(104)}, { 19, Action(105)}, { 24, Action(62, {62, 25, 63})}, { 25, Action(62, {62, 25, 63})}, { 26, Action(62, {62, 25, 63})}, { 27, Action(62, {62, 25, 63})}, { 28, Action(62, {62, 25, 63})}, { 29, Action(62, {62, 25, 63})}, { 30, Action(62, {62, 25, 63})}, { 31, Action(62, {62, 25, 63})}, { 32, Action(62, {62, 25, 63})}, { 33, Action(62, {62, 25, 63})}, { 34, Action(62, {62, 25, 63})}, { 35, Action(62, {62, 25, 63})}, { 36, Action(62, {62, 25, 63})}}},
{ 146, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 147, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 148, {{ 9, Action(63, {63, 18, 64})}, { 16, Action(63, {63, 18, 64})}, { 17, Action(108)}, { 18, Action(63, {63, 18, 64})}, { 19, Action(63, {63, 18, 64})}, { 22, Action(109)}, { 23, Action(110)}, { 24, Action(63, {63, 18, 64})}, { 25, Action(63, {63, 18, 64})}, { 26, Action(63, {63, 18, 64})}, { 27, Action(63, {63, 18, 64})}, { 28, Action(63, {63, 18, 64})}, { 29, Action(63, {63, 18, 64})}, { 30, Action(63, {63, 18, 64})}, { 31, Action(63, {63, 18, 64})}, { 32, Action(63, {63, 18, 64})}, { 33, Action(63, {63, 18, 64})}, { 34, Action(63, {63, 18, 64})}, { 35, Action(63, {63, 18, 64})}, { 36, Action(63, {63, 18, 64})}}},
{ 149, {{ 9, Action(63, {63, 19, 64})}, { 16, Action(63, {63, 19, 64})}, { 17, Action(108)}, { 18, Action(63, {63, 19, 64})}, { 19, Action(63, {63, 19, 64})}, { 22, Action(109)}, { 23, Action(110)}, { 24, Action(63, {63, 19, 64})}, { 25, Action(63, {63, 19, 64})}, { 26, Action(63, {63, 19, 64})}, { 27, Action(63, {63, 19, 64})}, { 28, Action(63, {63, 19, 64})}, { 29, Action(63, {63, 19, 64})}, { 30, Action(63, {63, 19, 64})}, { 31, Action(63, {63, 19, 64})}, { 32, Action(63, {63, 19, 64})}, { 33, Action(63, {63, 19, 64})}, { 34, Action(63, {63, 19, 64})}, { 35, Action(63, {63, 19, 64})}, { 36, Action(63, {63, 19, 64})}}},
{ 150, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 151, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 152, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 153, {{ 9, Action(64, {64, 17, 65})}, { 16, Action(64, {64, 17, 65})}, { 17, Action(64, {64, 17, 65})}, { 18, Action(64, {64, 17, 65})}, { 19, Action(64, {64, 17, 65})}, { 22, Action(64, {64, 17, 65})}, { 23, Action(64, {64, 17, 65})}, { 24, Action(64, {64, 17, 65})}, { 25, Action(64, {64, 17, 65})}, { 26, Action(64, {64, 17, 65})}, { 27, Action(64, {64, 17, 65})}, { 28, Action(64, {64, 17, 65})}, { 29, Action(64, {64, 17, 65})}, { 30, Action(64, {64, 17, 65})}, { 31, Action(64, {64, 17, 65})}, { 32, Action(64, {64, 17, 65})}, { 33, Action(64, {64, 17, 65})}, { 34, Action(64, {64, 17, 65})}, { 35, Action(64, {64, 17, 65})}, { 36, Action(64, {64, 17, 65})}}},
{ 154, {{ 9, Action(64, {64, 22, 65})}, { 16, Action(64, {64, 22, 65})}, { 17, Action(64, {64, 22, 65})}, { 18, Action(64, {64, 22, 65})}, { 19, Action(64, {64, 22, 65})}, { 22, Action(64, {64, 22, 65})}, { 23, Action(64, {64, 22, 65})}, { 24, Action(64, {64, 22, 65})}, { 25, Action(64, {64, 22, 65})}, { 26, Action(64, {64, 22, 65})}, { 27, Action(64, {64, 22, 65})}, { 28, Action(64, {64, 22, 65})}, { 29, Action(64, {64, 22, 65})}, { 30, Action(64, {64, 22, 65})}, { 31, Action(64, {64, 22, 65})}, { 32, Action(64, {64, 22, 65})}, { 33, Action(64, {64, 22, 65})}, { 34, Action(64, {64, 22, 65})}, { 35, Action(64, {64, 22, 65})}, { 36, Action(64, {64, 22, 65})}}},
{ 155, {{ 9, Action(64, {64, 23, 65})}, { 16, Action(64, {64, 23, 65})}, { 17, Action(64, {64, 23, 65})}, { 18, Action(64, {64, 23, 65})}, { 19, Action(64, {64, 23, 65})}, { 22, Action(64, {64, 23, 65})}, { 23, Action(64, {64, 23, 65})}, { 24, Action(64, {64, 23, 65})}, { 25, Action(64, {64, 23, 65})}, { 26, Action(64, {64, 23, 65})}, { 27, Action(64, {64, 23, 65})}, { 28, Action(64, {64, 23, 65})}, { 29, Action(64, {64, 23, 65})}, { 30, Action(64, {64, 23, 65})}, { 31, Action(64, {64, 23, 65})}, { 32, Action(64, {64, 23, 65})}, { 33, Action(64, {64, 23, 65})}, { 34, Action(64, {64, 23, 65})}, { 35, Action(64, {64, 23, 65})}, { 36, Action(64, {64, 23, 65})}}},
{ 156, {{ 16, Action(65, {66, 65})}, { 17, Action(65, {66, 65})}, { 18, Action(65, {66, 65})}, { 19, Action(65, {66, 65})}, { 22, Action(65, {66, 65})}, { 23, Action(65, {66, 65})}, { 24, Action(65, {66, 65})}, { 25, Action(65, {66, 65})}, { 26, Action(65, {66, 65})}, { 27, Action(65, {66, 65})}, { 28, Action(65, {66, 65})}, { 29, Action(65, {66, 65})}, { 30, Action(65, {66, 65})}, { 31, Action(65, {66, 65})}, { 32, Action(65, {66, 65})}, { 33, Action(65, {66, 65})}, { 34, Action(65, {66, 65})}, { 35, Action(65, {66, 65})}, { 36, Action(65, {66, 65})}, { 37, Action(65, {66, 65})}}},
{ 157, {{ 9, Action(67, {8, 54, 9})}, { 16, Action(67, {8, 54, 9})}, { 17, Action(67, {8, 54, 9})}, { 18, Action(67, {8, 54, 9})}, { 19, Action(67, {8, 54, 9})}, { 22, Action(67, {8, 54, 9})}, { 23, Action(67, {8, 54, 9})}, { 24, Action(67, {8, 54, 9})}, { 25, Action(67, {8, 54, 9})}, { 26, Action(67, {8, 54, 9})}, { 27, Action(67, {8, 54, 9})}, { 28, Action(67, {8, 54, 9})}, { 29, Action(67, {8, 54, 9})}, { 30, Action(67, {8, 54, 9})}, { 31, Action(67, {8, 54, 9})}, { 32, Action(67, {8, 54, 9})}, { 33, Action(67, {8, 54, 9})}, { 34, Action(67, {8, 54, 9})}, { 35, Action(67, {8, 54, 9})}, { 36, Action(67, {8, 54, 9})}}},
{ 158, {{ -1, Action(54, {55, 36, 54, 37, 54})}}},
{ 159, {{ 37, Action(181)}}},
{ 160, {{ 2, Action(35)}, { 4, Action(36)}, { 8, Action(37)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 161, {{ 34, Action(122)}, { 35, Action(55, {55, 35, 56})}, { 36, Action(55, {55, 35, 56})}, { 37, Action(55, {55, 35, 56})}}},
{ 162, {{ 33, Action(124)}, { 34, Action(56, {56, 34, 57})}, { 35, Action(56, {56, 34, 57})}, { 36, Action(56, {56, 34, 57})}, { 37, Action(56, {56, 34, 57})}}},
{ 163, {{ 32, Action(126)}, { 33, Action(57, {57, 33, 58})}, { 34, Action(57, {57, 33, 58})}, { 35, Action(57, {57, 33, 58})}, { 36, Action(57, {57, 33, 58})}, { 37, Action(57, {57, 33, 58})}}},
{ 164, {{ 16, Action(128)}, { 32, Action(58, {58, 32, 59})}, { 33, Action(58, {58, 32, 59})}, { 34, Action(58, {58, 32, 59})}, { 35, Action(58, {58, 32, 59})}, { 36, Action(58, {58, 32, 59})}, { 37, Action(58, {58, 32, 59})}}},
{ 165, {{ 16, Action(59, {59, 16, 60})}, { 30, Action(130)}, { 31, Action(131)}, { 32, Action(59, {59, 16, 60})}, { 33, Action(59, {59, 16, 60})}, { 34, Action(59, {59, 16, 60})}, { 35, Action(59, {59, 16, 60})}, { 36, Action(59, {59, 16, 60})}, { 37, Action(59, {59, 16, 60})}}},
{ 166, {{ 16, Action(60, {60, 30, 61})}, { 26, Action(134)}, { 27, Action(135)}, { 28, Action(136)}, { 29, Action(137)}, { 30, Action(60, {60, 30, 61})}, { 31, Action(60, {60, 30, 61})}, { 32, Action(60, {60, 30, 61})}, { 33, Action(60, {60, 30, 61})}, { 34, Action(60, {60, 30, 61})}, { 35, Action(60, {60, 30, 61})}, { 36, Action(60, {60, 30, 61})}, { 37, Action(60, {60, 30, 61})}}},
{ 167, {{ 16, Action(60, {60, 31, 61})}, { 26, Action(134)}, { 27, Action(135)}, { 28, Action(136)}, { 29, Action(137)}, { 30, Action(60, {60, 31, 61})}, { 31, Action(60, {60, 31, 61})}, { 32, Action(60, {60, 31, 61})}, { 33, Action(60, {60, 31, 61})}, { 34, Action(60, {60, 31, 61})}, { 35, Action(60, {60, 31, 61})}, { 36, Action(60, {60, 31, 61})}, { 37, Action(60, {60, 31, 61})}}},
{ 168, {{ 16, Action(61, {61, 26, 62})}, { 24, Action(142)}, { 25, Action(143)}, { 26, Action(61, {61, 26, 62})}, { 27, Action(61, {61, 26, 62})}, { 28, Action(61, {61, 26, 62})}, { 29, Action(61, {61, 26, 62})}, { 30, Action(61, {61, 26, 62})}, { 31, Action(61, {61, 26, 62})}, { 32, Action(61, {61, 26, 62})}, { 33, Action(61, {61, 26, 62})}, { 34, Action(61, {61, 26, 62})}, { 35, Action(61, {61, 26, 62})}, { 36, Action(61, {61, 26, 62})}, { 37, Action(61, {61, 26, 62})}}},
{ 169, {{ 16, Action(61, {61, 27, 62})}, { 24, Action(142)}, { 25, Action(143)}, { 26, Action(61, {61, 27, 62})}, { 27, Action(61, {61, 27, 62})}, { 28, Action(61, {61, 27, 62})}, { 29, Action(61, {61, 27, 62})}, { 30, Action(61, {61, 27, 62})}, { 31, Action(61, {61, 27, 62})}, { 32, Action(61, {61, 27, 62})}, { 33, Action(61, {61, 27, 62})}, { 34, Action(61, {61, 27, 62})}, { 35, Action(61, {61, 27, 62})}, { 36, Action(61, {61, 27, 62})}, { 37, Action(61, {61, 27, 62})}}},
{ 170, {{ 16, Action(61, {61, 28, 62})}, { 24, Action(142)}, { 25, Action(143)}, { 26, Action(61, {61, 28, 62})}, { 27, Action(61, {61, 28, 62})}, { 28, Action(61, {61, 28, 62})}, { 29, Action(61, {61, 28, 62})}, { 30, Action(61, {61, 28, 62})}, { 31, Action(61, {61, 28, 62})}, { 32, Action(61, {61, 28, 62})}, { 33, Action(61, {61, 28, 62})}, { 34, Action(61, {61, 28, 62})}, { 35, Action(61, {61, 28, 62})}, { 36, Action(61, {61, 28, 62})}, { 37, Action(61, {61, 28, 62})}}},
{ 171, {{ 16, Action(61, {61, 29, 62})}, { 24, Action(142)}, { 25, Action(143)}, { 26, Action(61, {61, 29, 62})}, { 27, Action(61, {61, 29, 62})}, { 28, Action(61, {61, 29, 62})}, { 29, Action(61, {61, 29, 62})}, { 30, Action(61, {61, 29, 62})}, { 31, Action(61, {61, 29, 62})}, { 32, Action(61, {61, 29, 62})}, { 33, Action(61, {61, 29, 62})}, { 34, Action(61, {61, 29, 62})}, { 35, Action(61, {61, 29, 62})}, { 36, Action(61, {61, 29, 62})}, { 37, Action(61, {61, 29, 62})}}},
{ 172, {{ 16, Action(62, {62, 24, 63})}, { 18, Action(146)}, { 19, Action(147)}, { 24, Action(62, {62, 24, 63})}, { 25, Action(62, {62, 24, 63})}, { 26, Action(62, {62, 24, 63})}, { 27, Action(62, {62, 24, 63})}, { 28, Action(62, {62, 24, 63})}, { 29, Action(62, {62, 24, 63})}, { 30, Action(62, {62, 24, 63})}, { 31, Action(62, {62, 24, 63})}, { 32, Action(62, {62, 24, 63})}, { 33, Action(62, {62, 24, 63})}, { 34, Action(62, {62, 24, 63})}, { 35, Action(62, {62, 24, 63})}, { 36, Action(62, {62, 24, 63})}, { 37, Action(62, {62, 24, 63})}}},
{ 173, {{ 16, Action(62, {62, 25, 63})}, { 18, Action(146)}, { 19, Action(147)}, { 24, Action(62, {62, 25, 63})}, { 25, Action(62, {62, 25, 63})}, { 26, Action(62, {62, 25, 63})}, { 27, Action(62, {62, 25, 63})}, { 28, Action(62, {62, 25, 63})}, { 29, Action(62, {62, 25, 63})}, { 30, Action(62, {62, 25, 63})}, { 31, Action(62, {62, 25, 63})}, { 32, Action(62, {62, 25, 63})}, { 33, Action(62, {62, 25, 63})}, { 34, Action(62, {62, 25, 63})}, { 35, Action(62, {62, 25, 63})}, { 36, Action(62, {62, 25, 63})}, { 37, Action(62, {62, 25, 63})}}},
{ 174, {{ 16, Action(63, {63, 18, 64})}, { 17, Action(150)}, { 18, Action(63, {63, 18, 64})}, { 19, Action(63, {63, 18, 64})}, { 22, Action(151)}, { 23, Action(152)}, { 24, Action(63, {63, 18, 64})}, { 25, Action(63, {63, 18, 64})}, { 26, Action(63, {63, 18, 64})}, { 27, Action(63, {63, 18, 64})}, { 28, Action(63, {63, 18, 64})}, { 29, Action(63, {63, 18, 64})}, { 30, Action(63, {63, 18, 64})}, { 31, Action(63, {63, 18, 64})}, { 32, Action(63, {63, 18, 64})}, { 33, Action(63, {63, 18, 64})}, { 34, Action(63, {63, 18, 64})}, { 35, Action(63, {63, 18, 64})}, { 36, Action(63, {63, 18, 64})}, { 37, Action(63, {63, 18, 64})}}},
{ 175, {{ 16, Action(63, {63, 19, 64})}, { 17, Action(150)}, { 18, Action(63, {63, 19, 64})}, { 19, Action(63, {63, 19, 64})}, { 22, Action(151)}, { 23, Action(152)}, { 24, Action(63, {63, 19, 64})}, { 25, Action(63, {63, 19, 64})}, { 26, Action(63, {63, 19, 64})}, { 27, Action(63, {63, 19, 64})}, { 28, Action(63, {63, 19, 64})}, { 29, Action(63, {63, 19, 64})}, { 30, Action(63, {63, 19, 64})}, { 31, Action(63, {63, 19, 64})}, { 32, Action(63, {63, 19, 64})}, { 33, Action(63, {63, 19, 64})}, { 34, Action(63, {63, 19, 64})}, { 35, Action(63, {63, 19, 64})}, { 36, Action(63, {63, 19, 64})}, { 37, Action(63, {63, 19, 64})}}},
{ 176, {{ 16, Action(64, {64, 17, 65})}, { 17, Action(64, {64, 17, 65})}, { 18, Action(64, {64, 17, 65})}, { 19, Action(64, {64, 17, 65})}, { 22, Action(64, {64, 17, 65})}, { 23, Action(64, {64, 17, 65})}, { 24, Action(64, {64, 17, 65})}, { 25, Action(64, {64, 17, 65})}, { 26, Action(64, {64, 17, 65})}, { 27, Action(64, {64, 17, 65})}, { 28, Action(64, {64, 17, 65})}, { 29, Action(64, {64, 17, 65})}, { 30, Action(64, {64, 17, 65})}, { 31, Action(64, {64, 17, 65})}, { 32, Action(64, {64, 17, 65})}, { 33, Action(64, {64, 17, 65})}, { 34, Action(64, {64, 17, 65})}, { 35, Action(64, {64, 17, 65})}, { 36, Action(64, {64, 17, 65})}, { 37, Action(64, {64, 17, 65})}}},
{ 177, {{ 16, Action(64, {64, 22, 65})}, { 17, Action(64, {64, 22, 65})}, { 18, Action(64, {64, 22, 65})}, { 19, Action(64, {64, 22, 65})}, { 22, Action(64, {64, 22, 65})}, { 23, Action(64, {64, 22, 65})}, { 24, Action(64, {64, 22, 65})}, { 25, Action(64, {64, 22, 65})}, { 26, Action(64, {64, 22, 65})}, { 27, Action(64, {64, 22, 65})}, { 28, Action(64, {64, 22, 65})}, { 29, Action(64, {64, 22, 65})}, { 30, Action(64, {64, 22, 65})}, { 31, Action(64, {64, 22, 65})}, { 32, Action(64, {64, 22, 65})}, { 33, Action(64, {64, 22, 65})}, { 34, Action(64, {64, 22, 65})}, { 35, Action(64, {64, 22, 65})}, { 36, Action(64, {64, 22, 65})}, { 37, Action(64, {64, 22, 65})}}},
{ 178, {{ 16, Action(64, {64, 23, 65})}, { 17, Action(64, {64, 23, 65})}, { 18, Action(64, {64, 23, 65})}, { 19, Action(64, {64, 23, 65})}, { 22, Action(64, {64, 23, 65})}, { 23, Action(64, {64, 23, 65})}, { 24, Action(64, {64, 23, 65})}, { 25, Action(64, {64, 23, 65})}, { 26, Action(64, {64, 23, 65})}, { 27, Action(64, {64, 23, 65})}, { 28, Action(64, {64, 23, 65})}, { 29, Action(64, {64, 23, 65})}, { 30, Action(64, {64, 23, 65})}, { 31, Action(64, {64, 23, 65})}, { 32, Action(64, {64, 23, 65})}, { 33, Action(64, {64, 23, 65})}, { 34, Action(64, {64, 23, 65})}, { 35, Action(64, {64, 23, 65})}, { 36, Action(64, {64, 23, 65})}, { 37, Action(64, {64, 23, 65})}}},
{ 179, {{ 16, Action(67, {8, 54, 9})}, { 17, Action(67, {8, 54, 9})}, { 18, Action(67, {8, 54, 9})}, { 19, Action(67, {8, 54, 9})}, { 22, Action(67, {8, 54, 9})}, { 23, Action(67, {8, 54, 9})}, { 24, Action(67, {8, 54, 9})}, { 25, Action(67, {8, 54, 9})}, { 26, Action(67, {8, 54, 9})}, { 27, Action(67, {8, 54, 9})}, { 28, Action(67, {8, 54, 9})}, { 29, Action(67, {8, 54, 9})}, { 30, Action(67, {8, 54, 9})}, { 31, Action(67, {8, 54, 9})}, { 32, Action(67, {8, 54, 9})}, { 33, Action(67, {8, 54, 9})}, { 34, Action(67, {8, 54, 9})}, { 35, Action(67, {8, 54, 9})}, { 36, Action(67, {8, 54, 9})}, { 37, Action(67, {8, 54, 9})}}},
{ 180, {{ 9, Action(54, {55, 36, 54, 37, 54})}}},
{ 181, {{ 2, Action(74)}, { 4, Action(75)}, { 8, Action(76)}, { 18, Action(15)}, { 19, Action(16)}, { 20, Action(17)}, { 21, Action(18)}}},
{ 182, {{ 37, Action(54, {55, 36, 54, 37, 54})}} } };

map<int, map<int, int>> expression_go_to = { { 0, {{ 54, 1}, { 55, 2}, { 56, 3}, { 57, 4}, { 58, 5}, { 59, 6}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 13, {{ 65, 58}, { 66, 13}, { 67, 14}}},
{ 21, {{ 54, 38}, { 55, 22}, { 56, 23}, { 57, 24}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 33, {{ 65, 114}, { 66, 33}, { 67, 34}}},
{ 37, {{ 54, 59}, { 55, 22}, { 56, 23}, { 57, 24}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 39, {{ 54, 61}, { 55, 60}, { 56, 62}, { 57, 63}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 40, {{ 56, 79}, { 57, 4}, { 58, 5}, { 59, 6}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 41, {{ 57, 81}, { 58, 5}, { 59, 6}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 42, {{ 58, 83}, { 59, 6}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 43, {{ 59, 85}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 44, {{ 60, 87}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 45, {{ 61, 90}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 46, {{ 61, 91}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 47, {{ 62, 96}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 48, {{ 62, 97}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 49, {{ 62, 98}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 50, {{ 62, 99}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 51, {{ 63, 102}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 52, {{ 63, 103}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 53, {{ 64, 106}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 54, {{ 64, 107}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 55, {{ 65, 111}, { 66, 13}, { 67, 14}}},
{ 56, {{ 65, 112}, { 66, 13}, { 67, 14}}},
{ 57, {{ 65, 113}, { 66, 13}, { 67, 14}}},
{ 72, {{ 65, 156}, { 66, 72}, { 67, 73}}},
{ 76, {{ 54, 116}, { 55, 22}, { 56, 23}, { 57, 24}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 77, {{ 54, 117}, { 55, 60}, { 56, 62}, { 57, 63}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 78, {{ 56, 121}, { 57, 24}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 80, {{ 57, 123}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 82, {{ 58, 125}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 84, {{ 59, 127}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 86, {{ 60, 129}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 88, {{ 61, 132}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 89, {{ 61, 133}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 92, {{ 62, 138}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 93, {{ 62, 139}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 94, {{ 62, 140}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 95, {{ 62, 141}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 100, {{ 63, 144}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 101, {{ 63, 145}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 104, {{ 64, 148}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 105, {{ 64, 149}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 108, {{ 65, 153}, { 66, 33}, { 67, 34}}},
{ 109, {{ 65, 154}, { 66, 33}, { 67, 34}}},
{ 110, {{ 65, 155}, { 66, 33}, { 67, 34}}},
{ 118, {{ 54, 159}, { 55, 60}, { 56, 62}, { 57, 63}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 119, {{ 56, 161}, { 57, 63}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 120, {{ 54, 158}, { 55, 2}, { 56, 3}, { 57, 4}, { 58, 5}, { 59, 6}, { 60, 7}, { 61, 8}, { 62, 9}, { 63, 10}, { 64, 11}, { 65, 12}, { 66, 13}, { 67, 14}}},
{ 122, {{ 57, 162}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 124, {{ 58, 163}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 126, {{ 59, 164}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 128, {{ 60, 165}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 130, {{ 61, 166}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 131, {{ 61, 167}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 134, {{ 62, 168}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 135, {{ 62, 169}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 136, {{ 62, 170}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 137, {{ 62, 171}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 142, {{ 63, 172}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 143, {{ 63, 173}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 146, {{ 64, 174}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 147, {{ 64, 175}, { 65, 71}, { 66, 72}, { 67, 73}}},
{ 150, {{ 65, 176}, { 66, 72}, { 67, 73}}},
{ 151, {{ 65, 177}, { 66, 72}, { 67, 73}}},
{ 152, {{ 65, 178}, { 66, 72}, { 67, 73}}},
{ 160, {{ 54, 180}, { 55, 22}, { 56, 23}, { 57, 24}, { 58, 25}, { 59, 26}, { 60, 27}, { 61, 28}, { 62, 29}, { 63, 30}, { 64, 31}, { 65, 32}, { 66, 33}, { 67, 34}}},
{ 181, {{ 54, 182}, { 55, 60}, { 56, 62}, { 57, 63}, { 58, 64}, { 59, 65}, { 60, 66}, { 61, 67}, { 62, 68}, { 63, 69}, { 64, 70}, { 65, 71}, { 66, 72}, { 67, 73}}} };



ContextFreeGrammar expression_grammar(terminals, nonterminals, productions, start);

#define __LIST__ 10
#define __PAIR__ 11

set<int> test_terminals = {__LEFT_PAREN__, __RIGHT_PAREN__};

set<int> test_nonterminals = {__LIST__, __PAIR__ };



multimap<int, vector<int>> test_productions = {
	  {__LIST__, {__PAIR__}},
	  {__LIST__, {__LIST__, __PAIR__}},
	  {__PAIR__, {__LEFT_PAREN__, __RIGHT_PAREN__}},
	  {__PAIR__, {__LEFT_PAREN__,__PAIR__, __RIGHT_PAREN__}},
};

int test_start = __LIST__;

#undef __LIST__
#undef __PAIR__


ContextFreeGrammar test_grammar(test_terminals, test_nonterminals, test_productions, test_start);

int evaluate_expression_tree(ParseTreeNode* start_node)
{
	if (start_node->children.size() == 1)
	{
		return evaluate_expression_tree(start_node->children[0]);
	}

	switch (start_node->symbol)
	{
	case(__CONDITIONAL_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[0]) ? evaluate_expression_tree(start_node->children[2]) : evaluate_expression_tree(start_node->children[4]);

		break;

	case(__LOGICAL_OR_EXPRESSION__):
		return evaluate_expression_tree(start_node->children[0]) || evaluate_expression_tree(start_node->children[2]);

		break;

	case(__LOGICAL_AND_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[0]) && evaluate_expression_tree(start_node->children[2]);

		break;

	case(__INCLUSIVE_OR_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[0]) | evaluate_expression_tree(start_node->children[2]);

		break;

	case(__EXCLUSIVE_OR_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[0]) ^ evaluate_expression_tree(start_node->children[2]);

		break;

	case(__AND_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[0]) & evaluate_expression_tree(start_node->children[2]);

		break;

	case(__EQUALITY_EXPRESSION__):

		if (start_node->children[1]->symbol == __EQUAL_EQUAL__)
		{
			return evaluate_expression_tree(start_node->children[0]) == evaluate_expression_tree(start_node->children[2]);
		}

		else if (start_node->children[1]->symbol == __NOT_EQUAL__)
		{
			return evaluate_expression_tree(start_node->children[0]) != evaluate_expression_tree(start_node->children[2]);
		}

		else
		{
			throw ExpressionError();
		}

		break;

	case(__RELATIONAL_EXPRESSION__):

		if (start_node->children[1]->symbol == __LESS_THAN__)
		{
			return evaluate_expression_tree(start_node->children[0]) < evaluate_expression_tree(start_node->children[2]);
		}

		else if (start_node->children[1]->symbol == __LEQ__)
		{
			return evaluate_expression_tree(start_node->children[0]) <= evaluate_expression_tree(start_node->children[2]);
		}

		else if (start_node->children[1]->symbol == __GREATER_THAN__)
		{
			return evaluate_expression_tree(start_node->children[0]) > evaluate_expression_tree(start_node->children[2]);
		}

		else if (start_node->children[1]->symbol == __GEQ__)
		{
			return evaluate_expression_tree(start_node->children[0]) >= evaluate_expression_tree(start_node->children[2]);
		}

		else
		{
			throw ExpressionError();
		}

		break;

	case(__SHIFT_EXPRESSION__):

		if (start_node->children[1]->symbol == __LEFT_SHIFT__)
		{
			return (evaluate_expression_tree(start_node->children[0]) << evaluate_expression_tree(start_node->children[2]));
		}

		else if (start_node->children[1]->symbol == __RIGHT_SHIFT__)
		{
			return (evaluate_expression_tree(start_node->children[0]) >> evaluate_expression_tree(start_node->children[2]));
		}

		else
		{
			throw ExpressionError();
		}

		break;
		
	case(__ADDITIVE_EXPRESSION__):

		if (start_node->children[1]->symbol == __PLUS__)
		{
			return (evaluate_expression_tree(start_node->children[0]) + evaluate_expression_tree(start_node->children[2]));
		}

		else if (start_node->children[1]->symbol == __MINUS__)
		{
			return (evaluate_expression_tree(start_node->children[0]) - evaluate_expression_tree(start_node->children[2]));
		}

		else
		{
			throw ExpressionError();
		}

		break;

	case(__MULTIPLICATIVE_EXPRESSION__):

		if (start_node->children[1]->symbol == __MULTIPLY__)
		{
			return (evaluate_expression_tree(start_node->children[0]) * evaluate_expression_tree(start_node->children[2]));
		}

		else if (start_node->children[1]->symbol == __DIVIDE__)
		{
			int divisor = evaluate_expression_tree(start_node->children[2]);

			if (divisor != 0)
			{
				return (evaluate_expression_tree(start_node->children[0]) / evaluate_expression_tree(start_node->children[2]));
			}

			else
			{
				throw ExpressionError();
			}
			
		}

		else if (start_node->children[1]->symbol == __MOD__)
		{
			return (evaluate_expression_tree(start_node->children[0]) % evaluate_expression_tree(start_node->children[2]));
		}

		else
		{
			throw ExpressionError();
		}

		break;

	case(__UNARY_EXPRESSION__):

		if (start_node->children[0]->children[0]->symbol == __PLUS__)
		{
			return evaluate_expression_tree(start_node->children[1]);
		}

		else if (start_node->children[0]->children[0]->symbol == __MINUS__)
		{
			return -evaluate_expression_tree(start_node->children[1]);
		}

		else if (start_node->children[0]->children[0]->symbol == __TILDE__)
		{
			return ~evaluate_expression_tree(start_node->children[1]);
		}

		else if (start_node->children[0]->children[0]->symbol == __NOT__)
		{
			return !evaluate_expression_tree(start_node->children[1]);
		}

		else
		{
			throw ExpressionError();
		}

		break;

	case(__PRIMARY_EXPRESSION__):

		return evaluate_expression_tree(start_node->children[1]);

		break;

	case(__INTEGER_CONSTANT__):

		return get_int_value(start_node->lexeme);

		break;

	case(__CHARACTER_CONSTANT__):

		return get_char_value(start_node->lexeme);

		break;
	}

	return 0;
}

map<char, int> val_map{ {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10}, {'A', 10}, {'b', 11}, {'B' ,11}, {'c', 12}, {'C', 12}, {'d', 13}, {'D', 13}, {'e', 14}, {'E', 14}, {'f', 15}, {'F', 15}};

int get_int_value(string int_string)
{
	if (int_string.size() == 0)
	{
		throw ExpressionError();
	}

	int start_index = 0;

	int base = 10;

	if (int_string[0] == 0)
	{
		if (int_string.size() > 1 && (int_string[1] == 'x' || int_string[1] == 'X'))
		{
			start_index = 2;
			base = 16;
		}

		else
		{
			base = 8;
		}
	}

	else if(isdigit(int_string[0]))
	{
		base = 10;
	}

	else
	{
		throw ExpressionError(); //invalid int constant
	}

	int end_index = int_string.size() - 1;

	while (end_index > -1 && val_map.count(int_string[end_index]) == 0)
	{
		end_index--;
	}

	int value = 0;

	int index = end_index;
	int base_power = 1;

	while (index != start_index - 1)
	{
		int coeff = val_map.at(int_string[index]);
		value += coeff * base_power;
		base_power *= base;
		index--;
	}

	return value; 
}

int get_char_value(string char_string)
{
	if (char_string.size() == 0)
	{
		throw ExpressionError();
	}

	int index = 0;

	while (char_string[index] != '\'')
	{
		index++;
	}

	index++;

	int start_index = index;

	while (char_string[index] != '\'')
	{
		index++;
	}

	int end_index = index - 1;

	int base = 256;
	int base_power = 1;

	int val = 0;

	for (int i = end_index; i >= start_index; i--)
	{
		val += static_cast<int>(char_string[i]) * base_power;
		base_power *= base;
	}

	return val; 
}



#undef __INTEGER_CONSTANT__
#undef __FLOATING_POINT_CONSTANT__ 
#undef __QUESTION_MARK__ 
#undef __COLON__
#undef __OR__ 
#undef __AND_AND__ 
#undef __INCLUSIVE_OR__ 
#undef __EXCLUSIVE_OR__ 
#undef __AND__
#undef __EQUAL_EQUAL__ 
#undef __NOT_EQUAL__ 
#undef __LESS_THAN__
#undef __GREATER_THAN__ 
#undef __LEQ__
#undef __GEQ__ 
#undef __LEFT_SHIFT__ 
#undef __RIGHT_SHIFT__ 
#undef __PLUS__ 
#undef __MINUS__ 
#undef __MULTIPLY__ 
#undef __DIVIDE__
#undef __MOD__
#undef __TILDE__ 
#undef __NOT__ 
#undef __LEFT_PAREN__ 
#undef __RIGHT_PAREN__ 

#undef __CONDITIONAL_EXPRESSION__ 
#undef __LOGICAL_OR_EXPRESSION__ 
#undef __LOGICAL_AND_EXPRESSION__ 
#undef __INCLUSIVE_OR_EXPRESSION__ 
#undef __EXCLUSIVE_OR_EXPRESSION__ 
#undef __AND_EXPRESSION__ 
#undef __EQUALITY_EXPRESSION__ 
#undef __RELATIONAL_EXPRESSION__ 
#undef __SHIFT_EXPRESSION__ 
#undef __ADDITIVE_EXPRESSION__ 
#undef __MULTIPLICATIVE_EXPRESSION__ 
#undef __UNARY_EXPRESSION__ 
#undef __UNARY_OPERATOR__ 
#undef __PRIMARY_EXPRESSION__ 
