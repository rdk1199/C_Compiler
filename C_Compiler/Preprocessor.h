#pragma once

#include <vector>
#include "CSyntax.h"
#include "Scanner.h"
#include <string>
#include "DFA.h"
#include <iostream>
#include <deque>

struct function_macro;

class Preprocessor
{ 

private:

	std::string date_string;
	std::string time_string;

	std::deque<PPWord> file_tokens;

	const std::set<std::string> directive_names = { "define", "if", "ifdef", "ifndef", "elif", "else", "endif", "include", "undef", "line", "error", "pragma" };
	const std::set<std::string> predefined_macro_names = { "__DATE__", "__FILE__", "__LINE__", "__STDC__", "__STDC_HOSTED__", "__STDC_VERSION__", "__TIME__" };
	const std::set<std::string> if_directive_names = { "if", "ifdef", "ifndef", "elif", "else", "endif" };
	const std::set<std::string> standard_pragmas = { "FP_CONTRACT", "FENV_ACCESS", "CX_LIMITED_RANGE" };
	const std::set<std::string> on_off = { "ON", "OFF", "DEFAULT" };
	const std::vector<std::string> months = { "Jan", "Feb", "Mar", "Apr", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	std::set<std::string> macro_names= { "__DATE__", "__FILE__", "__LINE__", "__STDC__", "__STDC_HOSTED__", "__STDC_VERSION__", "__TIME__" };

	const std::set<token> expression_tokens = { token::_integer_constant,  token::_character_constant,  token::_question, token::_colon, token::_bar_bar,  token::_and_and,  token::_bar, token::_caret, token::_and,  token::_equal_equal,  
		token::_not_equal,  token::_less_than,  token::_greater_than, token::_leq,  token::_geq,  token::_shift_left,  token::_shift_right,  token::_plus, token::_minus, token::_asterisk,  token::_forward_slash,  token::_percent,  
		token::_tilde,  token::_exclamation,  token::_left_paren,  token::_right_paren };

	const std::set<token> expression_ops = {token::_question, token::_colon, token::_bar_bar,  token::_and_and,  token::_bar, token::_caret, token::_and,  token::_equal_equal,
		token::_not_equal,  token::_less_than,  token::_greater_than, token::_leq,  token::_geq,  token::_shift_left,  token::_shift_right,  token::_plus, token::_minus, token::_asterisk,  token::_forward_slash,  token::_percent,
		token::_tilde,  token::_exclamation};

	std::map<std::string, std::vector<PPWord>> object_macros;
	std::map<std::string, function_macro> function_macros;

	std::string file_name = "test_file";
	int current_line_number_offset = 0;

	bool fp_contract = false;
	bool fenv_access = true;
	bool cx_limited_range = false; 

	std::string h_search_file_name; 
	std::string q_search_file_name;

	std::vector<PPWord> out_file;
	
	//helper functions

	std::vector<PPWord> get_rest_of_line(const std::vector<PPWord>& word_list, int index);
	std::vector<PPWord> get_rest_of_line(const std::deque<PPWord>& word_queue, int index);
	int skip_to_next(const std::vector<PPWord>& word_list, int index);
	int skip_to_next(const std::deque<PPWord>& word_queue, int index);
	int skip_lines_to_next(const std::vector<PPWord>& word_list, int index);
	int skip_lines_to_next(const std::deque<PPWord>& word_queue, int index);
	int go_to_end_of_line(const std::vector<PPWord>& word_list, int index);
	int go_to_end_of_line(const std::deque<PPWord>& word_queue, int index);
	int go_to_previous_line(const std::vector<PPWord>& word_list, int index);
	int go_to_previous_line(const std::deque<PPWord>& word_queue, int index);
	std::vector<PPWord> zero_identifiers(const std::vector<PPWord>&);
	std::vector<PPWord> get_constant_expression(const std::deque<PPWord>& word_list, int index);
	int find_next_if_directive(const std::deque<PPWord>& word_list, int index);
	bool evaluate_constant_expression(const std::vector<PPWord>& expression);
	std::vector<PPWord> destringize_pragma(std::string string_literal);
	int execute_pragma_op(const std::deque<PPWord>& word_string, int index);
	void process_pragma_tokens(const std::vector<PPWord>& pragma);
	//std::vector<PPWord> retokenize_header_name(const PPWord& header_name_token); //moved to scanner
	
	/*
	The following functions process individual directives; they return the index at which scanning shall continue from (not including that token), or, in the case where a vector<PPWord>
	is returned, the index passed through is set to the correct continuation index;

	index = index of directive name
	*/

	std::vector<PPWord> process_if_section(int& index);
	std::vector<PPWord> process_defined_ops(const std::vector<PPWord>& word_string);
	int process_define(int index);
	int process_undef(int index);
	int process_error(int index);
	int process_line(int index);
	std::vector<PPWord> process_include(int& index);
	int process_pragma(int index);
	void update_object_macros(int index); // updates __FILE__ , __LINE__ , __TIME__, and __DATE__ macros
	
	//These functions execute directives

	//index = index of identifier being macro replaced, return = string of tokens that replace macro

	std::vector<PPWord> replace_macros(const std::vector<PPWord>& word_list); //replace all macros in the list, excluding those in exclude std::set
	std::vector<PPWord> replace_macro(const std::deque<PPWord>& word_queue, int& index, bool& success); //if a macro name is found, checks are done to ensure it is actually a macro; success records this, and replacement is performed if necessary
	std::vector<PPWord> replace_object_macro(const std::deque<PPWord>& word_queue, int& index); 
	std::vector<PPWord> replace_function_macro(const std::deque<PPWord>& word_queue, int& index);
	std::vector<PPWord> perform_argument_ops(const std::vector<PPWord>& word_list); // executes ## operations in function like macro args
	std::vector<PPWord> remove_white_space(const std::vector<PPWord>& word_list); //removes all whitespace tokens
	std::vector<PPWord> trim_white_space(const std::vector<PPWord>& word_list); //remove leading and trailing white space only
	std::vector<PPWord> remove_placemarkers(const std::vector<PPWord>& word_list);

	Preprocessor(std::vector<PPWord> file, std::set<std::string> macro_names, std::map<std::string, std::vector<PPWord>> obj_macros, std::map<std::string, function_macro> func_macros, std::string h_files, std::string q_files);

public: 

	Preprocessor(std::vector<PPWord> file, std::string h_files, std::string q_files);

	std::vector<PPWord> preprocess();

	std::set<std::string> get_macro_names() { return macro_names; }
	std::map<std::string, std::vector<PPWord>> get_object_macros() { return object_macros; }
	std::map<std::string, function_macro> get_function_macros() { return function_macros; }
	int get_current_line_number_offset() { return current_line_number_offset; }
	void print_flags() { std::cout << "fp_contract: " << fp_contract << ", fenv_access: " << fenv_access << ", cx_limited_range: " << cx_limited_range; }
	static std::vector<Word> convert_to_tokens(const std::vector<PPWord>& pp_words);
};

class PreprocessorError : std::exception
{
private: 
	std::string error_message;

public:

	PreprocessorError() :
		error_message("Preprocessing Directive Error")
	{}

	PreprocessorError(std::string message):
		error_message("Preprocessing Directive Error: " + message)
	{}

	virtual const char* what() const throw()
	{
		return error_message.c_str();
	}
};

class PreprocessorInternalError :std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Preprocessor attempted illegal operation";
	}
};

class TokenConversionError : std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Token conversion error";
	}
};

struct function_macro {

	std::string macro_name;
	int num_args = 0; //>= 0
	bool var_args = false; //variable arguments?

	std::vector<std::string> arg_list;

	std::set<std::string> arg_set;

	std::vector<PPWord> replacement_tokens;

	function_macro():
		macro_name(""),
		num_args(-1),
		var_args(false),
		arg_list({}),
		replacement_tokens({}),
		arg_set({})
	{}

	function_macro(std::string name, int args, bool var, std::vector<std::string> _arg_list, std::vector<PPWord> replacement) :
		macro_name(name),
		num_args(args),
		var_args(var),
		arg_list(_arg_list),
		replacement_tokens({}),
		arg_set({})
	{
		for (int i = 0; i < replacement.size(); i++)
		{
			if (replacement[i].token != pp_token::_white_space)
			{
				replacement_tokens.push_back(replacement[i]);
			}
		}

		arg_set.insert(arg_list.begin(), arg_list.end());

		if (replacement_tokens.front().token == pp_token::_hashtag_hashtag || replacement_tokens.back().token == pp_token::_hashtag_hashtag || replacement_tokens.back().token == pp_token::_hashtag)
		{
			throw PreprocessorError(); //illegal use of ## operator or # operator at end of replacement tokens (6.10.3.3-1), (6.10.3.2-1)
		}

		for (int i = 0; i < replacement_tokens.size() - 1; i++)
		{
			if (replacement_tokens[i].token == pp_token::_hashtag && (replacement_tokens[i + 1].token != pp_token::_identifier || arg_set.count(replacement_tokens[i + 1].lexeme) == 0))
			{
				throw PreprocessorError(); //illegal use of # operator (6.10.3.2-1)
			}
		}

	}

	void print()
	{
		std::cout << "function macro: " << macro_name << std::endl;
		
		std::cout << "arguments: ";

		for (int i = 0; i < arg_list.size(); i++)
		{
			std::cout << arg_list[i] << " ";
		}
		std::cout << std::endl << "replacement string: ";

		for (int i = 0; i < replacement_tokens.size(); i++)
		{
			std::cout << replacement_tokens[i].lexeme;
		}

		std::cout << std::endl;

	}

};

