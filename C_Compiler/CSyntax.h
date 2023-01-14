#pragma once

#include <iostream>
#include "NFA.h"
#include "DFA.h"



const std::set<char> C_alphabet{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
							'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
							'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
							'!', '\"', '#', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?' ,'[', '\\' ,']', '^', '_', '{', '|', '}', '~',
							' ', '\t', '\v', '\f', '\a', '\b', '\r', '\n' };

const std::set<std::string> keywords{ "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto",
								"if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
								"union", "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
								"_Static_assert", "_Thread_local" };

const std::set<char> nondigit = { '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
								  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

const std::set<char> digits = { '0', '1', '2', '3', '4', '5', '6' ,'7' ,'8' ,'9' };

const std::set<char> nonzero_digits = { '1', '2', '3', '4', '5', '6' ,'7' ,'8' ,'9' };

const std::set<char> identifier_chars = { '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
										  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
										  '0', '1', '2', '3', '4', '5', '6' ,'7' ,'8' ,'9' };

const std::set<char> hex_digits = { '0', '1', '2', '3', '4', '5', '6' ,'7' ,'8' ,'9', 'a', 'b', 'c' ,'d', 'e' ,'f', 'A','B','C','D','E','F' };

const std::set<char> octal_digits = { '0', '1', '2', '3', '4', '5', '6' ,'7' };

const std::set<std::string> integer_suffixes = {"", "u", "U", "ul", "uL", "Ul", "UL", "ull", "uLL", "Ull", "ULL", "l", "L", "lu", "lU", "Lu", "LU", "ll", "LL", "llu", "llU", "LLu", "LLU"};

const std::set<std::string> punctuators = { "[", "]", "(", ")", "{", "}", ".", "->", "++", "--", "&", "*", "+", "-", "~", "!",
									"/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=" ,"^", "|", "&&", "||", "?",
									":", ";", "...", "=", "*=", "/=", "%=", "+=" ,"-=", "<<=", ">>=", "&=", "^=", "|=",
									",", "#", "##", "<:" ,":>", "<%", "%>", "%:", "%:%:" };

const std::set<std::string> string_encoding_prefixes = { "u8", "u", "U", "L", "" };

const std::set<std::string> univ_character_prefixes = { "\\u", "\\U" };

const std::set<std::string> simple_escape_sequences = { "\\\'", "\\\"", "\\?", "\\\\", "\\a", "\\b", "\\f", "\\n", "\\r" ,"\\t" , "\\v", "\\0" };

const std::set<std::string> char_constant_sequence_prefixes = { "L", "u", "U", "" };

const std::set<std::string> hex_prefixes = { "0X", "0x" };

const std::set<std::string> fp_suffixes = { "", "f", "l", "F", "L" };

const std::set<char> white_space_chars = {' ', '\f', '\n', '\t', '\v'};

const std::set<std::string> exponents{ "e+", "e-", "E+", "E-", "p+", "p-", "P+", "P-"};

const std::set<std::string> dec_exponent_parts{ "e", "E",  "e+", "e-", "E+", "E-"};
const std::set<std::string> bin_exponent_parts{ "p", "P", "p+", "p-", "P+", "P-" };

const std::set<std::string> pp_number_prefix{ "", "." };
/*
const NFA hex_digits_NFA(C_alphabet, hex_digits);
const NFA octal_digits_NFA(C_alphabet, octal_digits);
const NFA hex_quad_NFA = NFA::multi_concat(hex_digits_NFA, 4);
const NFA universal_character_small = NFA(C_alphabet, "\\u") * hex_quad_NFA;
const NFA universal_character_large = NFA::multi_concat({ NFA(C_alphabet, "\\U"), hex_quad_NFA , hex_quad_NFA });
const NFA universal_characters = universal_character_small + universal_character_large;
const NFA non_digit(C_alphabet, nondigit);
const NFA non_zero_digits(C_alphabet, nonzero_digits);
const NFA digit(C_alphabet, digits);
const NFA identifier_non_digit = non_digit + universal_characters;
const NFA identifier_all = digit + identifier_non_digit;
const NFA punctuator_NFA(C_alphabet, punctuators);
const NFA simple_escape_sequences_NFA(C_alphabet, simple_escape_sequences);
const NFA back_slash(C_alphabet, '\\');
const NFA octal_escape_sequence = back_slash * (NFA::multi_union({ octal_digits_NFA, octal_digits_NFA * octal_digits_NFA, NFA::multi_concat(octal_digits_NFA, 3) }));
const NFA hex_digits_closure = NFA::kleene_closure(hex_digits_NFA);
const NFA hex_escape_sequence = NFA::multi_concat({ back_slash , NFA(C_alphabet, 'x') , hex_digits_NFA, hex_digits_closure });
const NFA escape_sequences = NFA::multi_union({ simple_escape_sequences_NFA, octal_escape_sequence, hex_escape_sequence, universal_characters });
const NFA legal_string_chars_nfa = NFA::complement(C_alphabet, { '\"','\\', '\n'});
const NFA legal_sequence_chars_nfa = NFA::complement(C_alphabet, {'\'', '\\', '\n'});
const NFA string_char_sequence_nfa = NFA::kleene_closure(legal_string_chars_nfa + escape_sequences);
const NFA char_constant_sequence_nfa = (legal_sequence_chars_nfa + escape_sequences) * NFA::kleene_closure(legal_sequence_chars_nfa + escape_sequences);
const NFA exponents_nfa(C_alphabet, exponents);
const NFA pp_number_prefix_nfa(C_alphabet, pp_number_prefix);
const NFA period(C_alphabet, '.');
const NFA pp_number_end = NFA::multi_union({identifier_all, exponents_nfa, period});
const NFA quotation_mark(C_alphabet, '\"');
const NFA h_header_start(C_alphabet, '<');
const NFA h_header_finish(C_alphabet, '>');
const NFA forward_slash(C_alphabet, '/');
const NFA h_forward_slash_suffix = NFA::complement(C_alphabet, { '\n', '>', '\'', '\\', '\"', '/', '*' });
const NFA q_forward_slash_suffix = NFA::complement(C_alphabet, { '\n', '\'', '\\', '\"', '/' , '*'});
const NFA h_header_single_chars = NFA::complement(C_alphabet, {'\n', '>', '\'', '\\', '\"', '/'});
const NFA q_header_single_chars = NFA::complement(C_alphabet, { '\n', '\'', '\\', '\"', '/' }); 
const NFA white_space_chars_nfa(C_alphabet, white_space_chars);
const NFA non_new_line_chars = NFA::kleene_closure(NFA::complement(C_alphabet, { '\n' }));
const NFA new_line(C_alphabet, '\n');
const NFA asterisk_complement = NFA::complement(C_alphabet, {'*'});
const NFA forward_slash_complement = NFA::complement(C_alphabet, { '/', '*'});
const NFA zero(C_alphabet, '0');
const NFA point(C_alphabet, '.');
const NFA integer_suffixes_NFA(C_alphabet, integer_suffixes);
const NFA hexadecimal_prefix_NFA(C_alphabet, hex_prefixes);
const NFA decimal_constant_NFA = NFA::multi_concat({ non_zero_digits , NFA::kleene_closure(digit), integer_suffixes_NFA});
const NFA hexadecimal_constant_NFA = NFA::multi_concat({ hexadecimal_prefix_NFA, hex_digits_NFA, NFA::kleene_closure(hex_digits_NFA), integer_suffixes_NFA });
const NFA octal_constant_NFA = NFA::multi_concat({ zero, NFA::kleene_closure(octal_digits_NFA), integer_suffixes_NFA});
const NFA integer_constant_NFA = NFA::multi_union({decimal_constant_NFA, hexadecimal_constant_NFA, octal_constant_NFA});
const NFA digit_sequence = digit * NFA::kleene_closure(digit);
const NFA hex_digit_sequence = hex_digits_NFA * NFA::kleene_closure(hex_digits_NFA);
const NFA fractional_constant = NFA::multi_union({ NFA::multi_concat({NFA::kleene_closure(digit), point, digit_sequence}), digit_sequence * point});
const NFA hex_fractional_constant = NFA::multi_union({ NFA::multi_concat({NFA::kleene_closure(hex_digits_NFA), point, hex_digit_sequence}) , hex_digit_sequence * point});
const NFA dec_exponents(C_alphabet, dec_exponent_parts);
const NFA bin_exponents(C_alphabet, bin_exponent_parts);
const NFA dec_exp_part = dec_exponents * digit_sequence;
const NFA bin_exp_part = bin_exponents * digit_sequence;
const NFA floating_suffixes(C_alphabet, fp_suffixes);
const NFA decimal_floating_constant = NFA::multi_union({ fractional_constant * floating_suffixes, NFA::multi_concat({fractional_constant, dec_exp_part, floating_suffixes}), NFA::multi_concat({digit_sequence, dec_exp_part, floating_suffixes})});
const NFA hex_floating_constant = NFA::multi_concat({hexadecimal_prefix_NFA, hex_fractional_constant, bin_exp_part, floating_suffixes}) + NFA::multi_concat({ hexadecimal_prefix_NFA, hex_digit_sequence, bin_exp_part, floating_suffixes });
const NFA floating_constant_NFA = decimal_floating_constant + hex_floating_constant;
*/

class CSyntax
{
	/*
private:
	static NFA create_identifier_NFA();
	static NFA create_string_literal_NFA();
	static NFA create_punctuator_NFA();
	static NFA create_header_names_NFA();
	static NFA create_character_constants_NFA();
	static NFA create_pp_number_NFA();
	static NFA create_white_space_NFA();
	static NFA create_multi_line_comment_NFA();

public:
	static DFA create_C_categorizer();
	static DFA create_identifier_DFA();
	static DFA create_string_literal_DFA();
	static DFA create_punctuator_DFA();
	static DFA create_header_names_DFA();
	static DFA create_character_constants_DFA();
	static DFA create_pp_number_DFA();
	static DFA create_white_space_DFA();
	static DFA create_multi_line_comment_DFA();
	static void test_multi_line_comment_DFAs();
	static DFA create_number_categorizer();
	*/
};

extern const DFA C_tokenizer;
extern const DFA C_num_categorizer;


enum class pp_token {
	_EOF = EOF, _header_name, _identifier, _string_literal, _punctuator, _character_constant, _pp_number, _white_space,
	_left_bracket, _right_bracket, _left_paren, _right_paren, _left_brace, _right_brace, _period, _arrow, _plus_plus, _minus_minus,
	_and, _asterisk, _plus, _minus, _tilde, _exclamation, _forward_slash, _percent, _shift_left, _shift_right, _less_than,
	_greater_than, _leq, _geq, _equal_equal, _not_equal, _caret, _bar, _and_and, _bar_bar, _question, _colon, _semicolon, _ellipsis,
	_equal, _asterisk_equal, _slash_equal, _percent_equal, _plus_equal, _minus_equal, _shift_left_equal, _shift_right_equal, _and_equal,
	_caret_equal, _bar_equal, _comma, _hashtag, _hashtag_hashtag, _place_marker, _invalid
};


const std::vector<std::string> pp_token_names = { "header name", "identifier", "string literal", "punctuator", "character constant", "pre-processing number", "white space", "invalid", "EOF" };

enum class token {
	_EOF = EOF, _identifier, _type_identifier, _integer_constant, _fp_constant, _character_constant, _string_literal, _left_bracket, _right_bracket, _left_paren,
	_right_paren, _left_brace, _right_brace, _period, _arrow, _plus_plus, _minus_minus,
	_and, _asterisk, _plus, _minus, _tilde, _exclamation, _forward_slash, _percent, _shift_left, _shift_right, _less_than,
	_greater_than, _leq, _geq, _equal_equal, _not_equal, _caret, _bar, _and_and, _bar_bar, _question, _colon, _semicolon, _ellipsis,
	_equal, _asterisk_equal, _slash_equal, _percent_equal, _plus_equal, _minus_equal, _shift_left_equal, _shift_right_equal, _and_equal,
	_caret_equal, _bar_equal, _comma, _hashtag, _hashtag_hashtag, _auto, _break, _case, _char, _const, _continue, _default, _do, _double,
	_else, _enum, _extern, _float, _for, _goto, _if, _in_line, _int, _long, _register, _restrict_, _return, _short, _signed, _sizeof, _static,
	_struct, _switch, _typedef, _union, _unsigned, _void, _volatile, _while, _align_as, _align_of, _atomic, _bool, _complex, _generic, 
	_imaginary, _no_return, _static_assert, _thread_local
};

extern const std::map<char, char> trigraph_map;
extern const std::map<std::string, std::string> digraph_map;
extern const std::map<std::string, pp_token> punctuator_map;
extern const std::map<pp_token, token> token_conversion_map;
extern const std::map<char, std::string> escape_sequence_map;
extern const std::map<std::string, token> keyword_token_map;

struct PPWord
{
	std::string lexeme;
	pp_token token;
	int line_number;
	std::set<std::string> macro_exclude = {};
	bool macro_replacable = true;

	PPWord(std::string lex, pp_token tok, int line) :
		lexeme(lex),
		token(tok),
		line_number(line),
		macro_replacable(true)
	{}

	PPWord(std::string lex, pp_token tok) :
		lexeme(lex),
		token(tok),
		line_number(-1),
		macro_replacable(true)
	{}

	PPWord(std::string lex, pp_token tok, int line, std::set<std::string> exclude) :
		lexeme(lex),
		token(tok),
		line_number(line),
		macro_exclude(exclude)
	{}

	PPWord(std::string lex, pp_token tok, std::set<std::string> exclude) :
		lexeme(lex),
		token(tok),
		line_number(-1),
		macro_exclude(exclude)
	{}

	void print()
	{
		if (static_cast<int>(token) < 7 && static_cast<int>(token) >= 0)
		{
			std::cout << lexeme << " : " << pp_token_names[static_cast<int>(token)];
		}

		else if (static_cast<int>(token) >= 0)
		{
			std::cout << lexeme << " : " << "punctuator";
		}

		std::cout << ", line " << line_number;

	}

};

bool operator==(const PPWord& first, const PPWord second);

struct Word
{
	std::string lexeme;
	token terminal;

	Word(std::string lex, token tok) :
		lexeme(lex),
		terminal(tok)
	{}
};


/*
	std::vector<string> categorizer_test_strings = { "_0apple", "<cmath.h>", ".46_p-.3", "u\'char ][;;,.,./] sequence\'", "&=", "\"escape sequences: \\a, \\b, \\f, \\r, \\t, \\v, \\n, \\?, \\\', \\\", \\\\ \"",
												"//single line comment \n  /* multi \n line \n comment 134;][,]./]'.][][135\\\"****/ // <-remember to delete ", "/* \n \n this is a multi - line comment; \n \n asf \n \n *******/ \n \n \n c",

												/*
												"", " <//>", "\" fake escape sequence: \\y \"", "&!=", ".a1", "\'\' \'" };

																									std::vector<int> expected_categorizer_results = { 1, 0, 5, 4, 3, 2, 6, -1, -1, -1, -1, -1, -1, -1 };

																									categorizer.categorize_test(categorizer_test_strings, expected_categorizer_results, "C TOKENIZER");

																									*/



																									/*
																									DFA identifier_DFA = CSyntax::create_identifier_DFA();
																									DFA header_names_DFA = CSyntax::create_header_names_DFA();
																									DFA pp_number_DFA = CSyntax::create_pp_number_DFA();
																									DFA character_constants_DFA = CSyntax::create_character_constants_DFA();
																									DFA punctuator_DFA = CSyntax::create_punctuator_DFA();
																									DFA string_literal_DFA = CSyntax::create_string_literal_DFA();


																									std::vector<string> identifier_test_strings = { "apple", "apples_24", "", "0apple", "_0apple", "45", "_45", "\\u0abc", "\\U0ABCDEF0", " ", "apple pie", "_appl\\U0ABCDEF0\\u1243__e", "_appl\\U0ABC\\u1243__e", "_appl\\U0ABCDEF1\\U1243__e", "\\U0ABCDEF1\\u1243__e" };
																									std::vector<bool> expected_identifier_results = { true, true, false, false, true, false, true, true, true, false, false, true , false, false, true };

																									std::vector<string> header_names_test_strings = { "<cmath.h>", "<howdy>", "<10>", "\"non-header\"", "</*>", "<//>", "<>", "<header.h", "", " ", "<\'>", "<a/e>" };
																									std::vector<bool> expected_header_names_results = { true, true, true, false, false, false, false, false, false, false, false, true };

																									std::vector<string> pp_number_test_strings = { ".01", "0.1", "10", "10a", ".10.E+a", ".46_p-.3", ".", "", "f135", "0x0xe-", "_124", "54_p--", "0.00............" };
																									std::vector<bool> expected_pp_number_results = { true, true, true, true, true, true, false, false, false, true, false, false, true };

																									std::vector<string> character_constants_test_strings = { "\'char sequence \'", "L\'char sequence \'", "u\'char ][;;,.,./] sequence\'", "U\'char sequence \'", "\'\'", "L\'\'", "char_sequence", "\'char sequence" };
																									std::vector<bool> expected_character_constants_results = { true, true, true, true, false, false, false, false };

																									std::vector<string> punctuator_test_strings = { ";", "[", ">>=", "<%", "...", "..", "", " ", "\"", "\'", "#", "##", "###", "&=", "&!=", "a", "4", "_", "-", "--", "---" };
																									std::vector<bool> expected_punctuator_results = { true, true, true, true, true, false, false, false, false, false, true, true, false, true, false, false, false, false, true, true, false };

																									std::vector<string> string_literal_test_strings = { "u8\"asd\"",  "u\"string\"", "U\"escape\"", "L\"\"", "\"\n\"", "\"asb", "\"escape sequences: \\a, \\b, \\f, \\r, \\t, \\v, \\n, \\?, \\\', \\\", \\\\ \"", "\" fake escape sequence: \\y \"", "", " " };
																									std::vector<bool> expected_string_literal_results = { true, true, true, true, false, false, true, false, false, false };

																									cout << endl << "IDENTIFIER TEST: " << endl;
																									identifier_DFA.accept_test(identifier_test_strings, expected_identifier_results, "IDENTIFIER TEST");

																									cout << endl << "HEADER NAMES TEST: " << endl;
																									header_names_DFA.accept_test(header_names_test_strings, expected_header_names_results, "HEADER NAMES TEST");

																									cout << endl << "PREPROCESSING NUMBER TEST: " << endl;
																									pp_number_DFA.accept_test(pp_number_test_strings, expected_pp_number_results, "PREPROCESSING NUMBER TEST");

																									cout << endl << "CHARACTER CONSTANTS TEST: " << endl;
																									character_constants_DFA.accept_test(character_constants_test_strings, expected_character_constants_results, "CHARACTER CONSTANTS TEST");

																									cout << endl << "PUNCTUATOR TEST: " << endl;
																									punctuator_DFA.accept_test(punctuator_test_strings, expected_punctuator_results, "PUNCTUATOR TEST");

																									cout << endl << "STRING LITERAL TEST: " << endl;
																									string_literal_DFA.accept_test(string_literal_test_strings, expected_string_literal_results, "STRING LITERAL TEST");



																									*/
																									/*
																									cout << endl << "CATEGORIZER TEST: " << endl;*/