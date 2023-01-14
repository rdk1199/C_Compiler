#include "CSyntax.h"
#include "Preprocessor.h"
#include "TableDrivenScanner.h"
#include "ExpressionGrammar.h"

#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <list>
#include <chrono>
#include <ctime>
#include <deque>

using std::string;
using std::map;
using std::min;
using std::cout;
using std::endl;
using std::list;
using std::stoi;
using std::to_string;
using std::deque;
using std::vector;
using std::set;
using std::multimap;
using std::initializer_list;

Preprocessor::Preprocessor(vector<PPWord> file, set<string> macro_names, map<string, vector<PPWord>> obj_macros, map<string, function_macro> func_macros, string h_files, string q_files):
	macro_names(macro_names),
	object_macros(obj_macros),
	function_macros(func_macros),
	h_search_file_name(h_files),
	q_search_file_name(q_files)
{
	file_tokens.insert(file_tokens.end(), file.begin(), file.end());

	//https://stackoverflow.com/questions/27427905/how-to-get-current-date-in-c-program
	time_t t = time(0);
	struct tm time_struct;
	localtime_s(&time_struct, &t);

	date_string = months[time_struct.tm_mon] + "_" + to_string(time_struct.tm_mday) + "_" + to_string(time_struct.tm_year);
	time_string = to_string(time_struct.tm_hour) + ":" + to_string(time_struct.tm_min) + ":" + to_string(time_struct.tm_sec);
}

Preprocessor::Preprocessor(vector<PPWord> file, string h_files, string q_files) :
	h_search_file_name(h_files),
	q_search_file_name(q_files)
{
	file_tokens.insert(file_tokens.end(), file.begin(), file.end());

	object_macros["__STDC__"] = { PPWord("1", pp_token::_pp_number, 1)};
	object_macros["__STDC_HOSTED__"] = { PPWord("1", pp_token::_pp_number, 1)};
	object_macros["__STDC_VERSION__"] = { PPWord("201710L", pp_token::_pp_number, 1) };

	time_t t = time(0);
	struct tm time_struct;
	localtime_s(&time_struct, &t);

	date_string = months[time_struct.tm_mon] + "_" + to_string(time_struct.tm_mday) + "_" + to_string(time_struct.tm_year);
	time_string = to_string(time_struct.tm_hour) + ":" + to_string(time_struct.tm_min) + ":" + to_string(time_struct.tm_sec);

	update_object_macros(0);
}



vector<PPWord> Preprocessor::preprocess()
{
	int i = 0;	
	bool can_have_directive = true; //this is set to true at each new line, and remains true until a non-white-space token is found 
	int current_line_num = 1; 

	while (file_tokens.size() > 0) //RULE: tokens are inserted into out_file precisely when they are deleted from file_tokens
	{
		string lexeme = file_tokens[0].lexeme;
		pp_token token = file_tokens[0].token;

		if (token != pp_token::_white_space && token != pp_token::_hashtag && token != pp_token::_place_marker)
		{
			can_have_directive = false; 
		}

		switch (token)
		{
			case(pp_token::_identifier):

				if (lexeme == "_Pragma")
				{
					i = execute_pragma_op(file_tokens, 0);
					for (int j = 0; j <= i; j++)
					{
						file_tokens.pop_front();
					}
				}

				else if(macro_names.count(lexeme) > 0 && file_tokens[0].macro_exclude.count(lexeme) == 0 && file_tokens[0].macro_replacable) 
				{
					int start = 0;
					bool replacement_success = true;

					vector<PPWord> macro_replacement = replace_macro(file_tokens, start, replacement_success);

					if (replacement_success)
					{
						for (int j = 0; j <= start; j++)
						{
							file_tokens.pop_front();
						}

						file_tokens.insert(file_tokens.begin(), macro_replacement.begin(), macro_replacement.end());
						can_have_directive = false;
					}

					else
					{
						out_file.push_back(file_tokens[0]);
						file_tokens.pop_front();
					}
					
				}

				else
				{
					out_file.push_back(file_tokens[0]);
					file_tokens.pop_front();
				}
				break;

			/* retokenization has been moved to scanner
			case(pp_token::_header_name): //valid header names are found only after include directives, so if one is found here, it must be retokenized
			{
				vector<PPWord> retokenization = retokenize_header_name(file_tokens[0]);

				file_tokens.pop_front();
				file_tokens.insert(file_tokens.begin(), retokenization.begin(), retokenization.end());

				break;
			}
			*/

			case(pp_token::_hashtag):
			{
				if (can_have_directive)
				{
					int directive_index = skip_to_next(file_tokens, 0);

					if (directive_index != -1 && directive_names.count(file_tokens[directive_index].lexeme) > 0) //directive found
					{
						string directive = file_tokens[directive_index].lexeme;

						if (directive == "if" || directive == "ifdef" || directive == "ifndef")
						{	
							vector<PPWord> included_section = process_if_section(directive_index);

							for (int j = 0; j <= directive_index; j++)
							{
								file_tokens.pop_front();
							}

							file_tokens.insert(file_tokens.begin(), included_section.begin(), included_section.end());

							can_have_directive = true; 
						}

						else if (directive == "elif" || directive == "else" || directive == "endif")
						{
							throw PreprocessorError();  //will be handled by process_if_section; if they are found here, then they do not have an opening if-statement (error)
						}

						else if (directive == "define")
						{
							i = process_define(directive_index);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}

						else if (directive == "undef")
						{
							i = process_undef(directive_index);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}

						else if (directive == "line")
						{
							i = process_line(directive_index);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}

						else if (directive == "error")
						{
							i = process_error(directive_index);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}

						else if (directive == "pragma")
						{
							i = process_pragma(directive_index);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}

						else if (directive == "include")
						{
							vector<PPWord> included_file = process_include(directive_index);
							out_file.insert(out_file.end(), included_file.begin(), included_file.end());
							for (int j = 0; j <= directive_index; j++)
							{
								file_tokens.pop_front();
							}
						}

						else //non-directive
						{
							i = go_to_end_of_line(file_tokens, i);
							for (int j = 0; j <= i; j++)
							{
								file_tokens.pop_front();
							}
						}
					}

					else //non-directive
					{
						i = go_to_end_of_line(file_tokens, i);
						for (int j = 0; j <= i; j++)
						{
							file_tokens.pop_front();
						}
					}
				}

				else
				{
					out_file.push_back(file_tokens[i]);
					file_tokens.pop_front();
				}
				break;
			}

			case(pp_token::_white_space):
			{
				if (file_tokens[i].lexeme[0] == '\n')
				{
					can_have_directive = true;
				}

				out_file.push_back(file_tokens[i]);
				file_tokens.pop_front();

				break;
			}
			default:
				out_file.push_back(file_tokens[i]);
				file_tokens.pop_front();
				break;
		}

		i = 0; //loop always begins at the front of the rest of the unprocessed file
	}	
	return out_file;
}

/*
vector<PPWord> Preprocessor::retokenize_header_name(const PPWord& header_name_token)
{
	vector<PPWord> out = {};

	PPWord start = PPWord("<", pp_token::_less_than);

	out.push_back(start);

	string remainder = header_name_token.lexeme.substr(1, header_name_token.lexeme.size() - 1);

	TableDrivenScanner scanner(remainder, C_tokenizer);

	vector<PPWord> new_tokens = scanner.scan();

	for (int i = 0; i < new_tokens.size(); i++)
	{
		out.push_back(new_tokens[i]);
	}

	return out; 
}
*/

vector<PPWord> Preprocessor::process_defined_ops(const vector<PPWord>& word_string) //white space should be REMOVED before call!
{
	vector<PPWord> out; 
	
	for (int i = 0; i < word_string.size(); i++)
	{
		if (word_string[i].token == pp_token::_identifier && word_string[i].lexeme == "defined")
		{
			if(i+1 < word_string.size() && word_string[i+1].token == pp_token::_identifier)
			{
				string name = word_string[i + 1].lexeme;

				if (macro_names.count(name) > 0)
				{
					out.push_back(PPWord("1", pp_token::_pp_number, word_string[i+1].line_number + current_line_number_offset));
				}

				else
				{
					out.push_back(PPWord("0", pp_token::_pp_number, word_string[i + 1].line_number + current_line_number_offset));
				}

				i = i + 1;
			}

			else if (i + 3 < word_string.size() && word_string[i + 1].token == pp_token::_left_paren && word_string[i + 2].token == pp_token::_identifier && word_string[i + 3].token == pp_token::_right_paren)
			{
				string name = word_string[i + 2].lexeme;

				if (macro_names.count(name) > 0)
				{
					out.push_back(PPWord("1", pp_token::_pp_number, word_string[i+2].line_number + current_line_number_offset));
				}
				else
				{
					out.push_back(PPWord("0", pp_token::_pp_number, word_string[i + 2].line_number + current_line_number_offset));
				}
				
				i = i + 3;
			}

			else
			{
				throw PreprocessorError(); //improper use of defined
			}
		}

		else
		{
			out.push_back(word_string[i]);
		}
	}

	return out; 
}

int Preprocessor::execute_pragma_op(const deque<PPWord>& word_string, int index)
{
	int lparen = skip_lines_to_next(word_string, index);
	if (lparen == -1 || word_string[lparen].token != pp_token::_left_paren)
	{
		throw PreprocessorError(); 
	}

	int string_lit = skip_lines_to_next(word_string, lparen);
	if (string_lit == -1 || word_string[string_lit].token != pp_token::_string_literal)
	{
		throw PreprocessorError();
	}

	int rparen = skip_lines_to_next(word_string, string_lit);

	if (rparen == -1 || word_string[rparen].token != pp_token::_right_paren)
	{
		throw PreprocessorError();
	}

	vector<PPWord> pragma_tokens = destringize_pragma(word_string[string_lit].lexeme);

	process_pragma_tokens(pragma_tokens);

	return rparen;
}

void Preprocessor::process_pragma_tokens(const vector<PPWord>& pragma)
{
	vector<PPWord> pragma_tokens = remove_white_space(pragma);
	if (pragma_tokens.size() != 3 || pragma_tokens[0].lexeme != "STDC" || standard_pragmas.count(pragma_tokens[1].lexeme) == 0 || on_off.count(pragma_tokens[2].lexeme) == 0)
	{
		throw PreprocessorError(); //invalid standard pragma
	}

	if (pragma_tokens[2].lexeme != "DEFAULT")
	{
		string tag = pragma_tokens[1].lexeme;
		string setting = pragma_tokens[2].lexeme;

		if (tag == "FP_CONTRACT")
		{
			fp_contract = setting == "ON" ? true : false;
		}

		else if (tag == "FENV_ACCESS")
		{
			fenv_access = setting == "ON" ? true : false;
		}

		else if (tag == "CX_LIMITED_RANGE")
		{
			cx_limited_range = setting == "ON" ? true : false;
		}
	}
}

vector<PPWord> Preprocessor::destringize_pragma(string string_literal)
{
	if (string_literal.size() <= 2)
	{
		return {}; //empty string
	}
	string_literal = string_literal.substr(1, string_literal.size() - 2);

	vector<char> new_string_chars;

	for (int i = 0; i < string_literal.size(); i++)
	{
		if (string_literal[i] == '\\' && i + 1 < string_literal.size() && (string_literal[i + 1] == '\"' || string_literal[i + 1] == '\\'))
		{
			new_string_chars.push_back(string_literal[i + 1]);
			i++;
		}

		else
		{
			new_string_chars.push_back(string_literal[i]);
		}
	}

	string new_string_literal(new_string_chars.begin(), new_string_chars.end());
	return TableDrivenScanner(new_string_literal, C_tokenizer).scan();
}


vector<PPWord> Preprocessor::zero_identifiers(const vector<PPWord>& word_string)
{
	vector<PPWord> out; 

	for (int i = 0; i < word_string.size(); i++)
	{
		if (word_string[i].token == pp_token::_identifier)
		{
			out.push_back(PPWord("0", pp_token::_pp_number, word_string[i].line_number + current_line_number_offset));
		}
		
		else
		{
			out.push_back(word_string[i]);
		}
	}

	return out; 
}

vector<PPWord> Preprocessor::process_if_section(int& index) //index = index of if, ifndef, or ifdef in token string.
{
	int unclosed_ifs = 1;

	vector<PPWord> if_expression = get_constant_expression(file_tokens, index);

	vector<vector<PPWord>> if_expressions = { if_expression };
	vector<int> if_group_start_indices = { go_to_end_of_line(file_tokens, index) + 1 }; //holds indices for start of group if, elif, related to initial if
	vector<vector<PPWord>> if_groups; //holds token groups for each if, elif, or else

	bool endif_found = false;
	bool else_found = false; 

	int else_group_start_index = -1;
	vector<PPWord> else_group = {};

	int current_index = index;

	while (!endif_found)
	{
		int next_if_directive_index = find_next_if_directive(file_tokens, current_index);
		int end_of_previous_group = go_to_previous_line(file_tokens, next_if_directive_index);

		if (next_if_directive_index == -1)
		{
			throw PreprocessorError(); //unterminated if!
		}

		PPWord next_directive = file_tokens[next_if_directive_index];
		string next_directive_name = next_directive.lexeme;

		// cout << "next directive name: " << next_directive_name << endl;

		if (next_directive_name == "if" || next_directive_name == "ifdef" || next_directive_name == "ifndef")
		{
			unclosed_ifs++;
		}

		if (unclosed_ifs == 1)
		{
			if (next_directive_name == "elif")
			{
				if (else_found)
				{
					throw PreprocessorError(); //new elif after else!
				}

				if_groups.push_back({}); //fill in group for previous if or elif

				for (int i = if_group_start_indices.back(); i <= end_of_previous_group; i++)
				{
					if_groups[if_groups.size() - 1].push_back(file_tokens[i]);
				}

				if_group_start_indices.push_back(go_to_end_of_line(file_tokens, next_if_directive_index) + 1);
				if_expressions.push_back(get_constant_expression(file_tokens, next_if_directive_index));
			}

			else if (next_directive_name == "else")
			{
				if (else_found || skip_to_next(file_tokens, next_if_directive_index) != -1)
				{
					throw PreprocessorError(); //multiple elses or else with expression after
				}

				if_groups.push_back({}); //fill in group for previous if or elif

				for (int i = if_group_start_indices.back(); i <= end_of_previous_group; i++)
				{
					if_groups[if_groups.size() - 1].push_back(file_tokens[i]);
				}

				else_found = true;
				else_group_start_index = go_to_end_of_line(file_tokens, next_if_directive_index) + 1;
			}
		}

		if (next_directive_name == "endif")
		{
			unclosed_ifs--;

			if (unclosed_ifs == 0)
			{
				endif_found = true;

				index = go_to_end_of_line(file_tokens, next_if_directive_index); 
				
				//push back final group

				if (else_found)
				{
					for (int i = else_group_start_index; i <= end_of_previous_group; i++)
					{
						else_group.push_back(file_tokens[i]);
					}
				}

				else
				{
					if_groups.push_back({}); //fill in group for previous if or elif

					for (int i = if_group_start_indices.back(); i <= end_of_previous_group; i++)
					{
						if_groups[if_groups.size() - 1].push_back(file_tokens[i]);
					}
				}

			}
		}
		
		current_index = next_if_directive_index;
	}

	bool acceptance_found = false;

	vector<PPWord> included_group;

	for (int i = 0; i < if_groups.size(); i++)
	{
		if (evaluate_constant_expression(if_expressions[i]))
		{
			acceptance_found = true;
			included_group = if_groups[i];
			break;
		}
	}

	if (!acceptance_found)
	{
		included_group = else_group;
	}

	return included_group;
}

int Preprocessor::find_next_if_directive(const deque<PPWord>& word_list, int index) //return = index of next if directive found, if directives being if, ifdef, ifndef, elif, else, endif, or -1 if none exists // should call from current if directive
{
	bool can_have_directive = false; 

	for (int i = index + 1; i < word_list.size(); i++)
	{
		PPWord current_word = word_list[i];

		switch (current_word.token)
		{
		case (pp_token::_white_space):
			{
				if (current_word.lexeme[0] == '\n')
				{
					can_have_directive = true;
				}
				break;
			}

		case(pp_token::_hashtag):
			{

				int next = skip_to_next(word_list, i);
				
				if (next != -1 && can_have_directive && if_directive_names.count(word_list[next].lexeme) > 0)
				{ 
					return next;
				}

			break;
			}

		default:
			can_have_directive = false; 
			break;
		}
	}

	return -1;
}

vector<PPWord> Preprocessor::get_constant_expression(const deque<PPWord>& word_list, int index) //index = if, elif, ifdef, or ifndef token
{
	vector<PPWord> const_expression;

	string if_type = word_list[index].lexeme;

	if (if_type == "if" || if_type == "elif")
	{
		const_expression = replace_macros(process_defined_ops(remove_white_space(get_rest_of_line(word_list, index)))); //could do defined ops before macro replacement, then check for any defined operators after replacement and reject if found (for stricter conformance)
	}

	else if (if_type == "ifdef" || if_type == "ifndef")
	{
		int subject_index = skip_to_next(word_list, index);

		if (subject_index == -1 || !(word_list[subject_index].token == pp_token::_identifier) || skip_to_next(word_list, subject_index) != -1)
		{
			throw PreprocessorInternalError(); //invalid ifdef or ifndef format!
		}

		string name = word_list[subject_index].lexeme;

		int defined = macro_names.count(name) > 0 ? 1 : 0;
		int ifdef = if_type == "ifdef" ? 1 : 0;

		string num = defined * ifdef ? "1" : "0";

		const_expression = { PPWord(num, pp_token::_pp_number, word_list[index].line_number + current_line_number_offset) };
	}

	else
	{
		throw PreprocessorInternalError();//should not have been called!
	}

	const_expression = zero_identifiers(const_expression);
	return const_expression;
}

vector<PPWord> Preprocessor::process_include(int& index)
{
	vector<PPWord> include_tokens = get_rest_of_line(file_tokens, index);

	index = go_to_end_of_line(file_tokens, index);

	include_tokens = remove_white_space(replace_macros(include_tokens));

	if (!(include_tokens.size() == 1) || (include_tokens[0].token != pp_token::_string_literal && include_tokens[0].token != pp_token::_header_name))
	{
		throw PreprocessorError(); //invalid include directive
	}

	string file_name = include_tokens[0].lexeme;
	
	vector<PPWord> include_file;

	if (file_name[0] == '<') //h-char-sequence
	{
		file_name = file_name.substr(1, file_name.size() - 2);
		string file_path = h_search_file_name + "\\" + file_name;

		TableDrivenScanner scanner(file_path, C_tokenizer, 1000);
		include_file = scanner.scan();
	}

	else //q-char-sequence
	{
		file_name = file_name.substr(1, file_name.size() - 2);
		string file_path = q_search_file_name + "\\" + file_name;

		try 
		{
			TableDrivenScanner scanner(file_path, C_tokenizer, 1000);
			include_file = scanner.scan();
		}
		
		catch (FileNotFoundError)
		{
			string alt_file_path = h_search_file_name + "\\" + file_name;
			TableDrivenScanner scanner(alt_file_path, C_tokenizer, 1000);
			include_file = scanner.scan();
		}
	}
	
	Preprocessor preprocessor(include_file, h_search_file_name, q_search_file_name);

	vector<PPWord> include_out = preprocessor.preprocess();

	set<string> new_macro_names = preprocessor.get_macro_names();
	map<string, vector<PPWord>> new_object_macros = preprocessor.get_object_macros();
	map<string, function_macro> new_func_macros = preprocessor.get_function_macros();

	macro_names.insert(new_macro_names.begin(), new_macro_names.end());
	object_macros.insert(new_object_macros.begin(), new_object_macros.end());
	function_macros.insert(new_func_macros.begin(), new_func_macros.end());

	return include_out; 
}

int Preprocessor::process_line(int index)
{;
	vector<PPWord> line_tokens = remove_white_space(replace_macros(get_rest_of_line(file_tokens, index)));

	if (!((line_tokens.size() == 1 && line_tokens[0].token == pp_token::_pp_number) ||(line_tokens.size() == 2 && line_tokens[0].token == pp_token::_pp_number && line_tokens[1].token == pp_token::_string_literal)))
	{
		throw PreprocessorError(); //invalid line directive
	}

	string digits_lexeme = line_tokens[0].lexeme;

	if (digits_lexeme.size() == 0)
	{
		throw PreprocessorError(); //empty digit sequence
	}

	for (int i = 0; i < digits_lexeme.size(); i++)
	{
		if (!('0' <= digits_lexeme[i] && digits_lexeme[i] <= '9'))
		{
			throw PreprocessorError(); //not a digit sequence!
		}
	}

	int line_num = stoi(digits_lexeme);

	if (line_num <= 0 || line_num >= INT32_MAX)
	{
		throw PreprocessorError(); //invalid line reassignment
	}

	current_line_number_offset = line_num - file_tokens[index].line_number;

	if (line_tokens.size() == 2)
	{
		string name_lexeme = line_tokens[1].lexeme;
		string new_file_name = name_lexeme.substr(1, name_lexeme.size() - 2);

		file_name = new_file_name;
	}

	return go_to_end_of_line(file_tokens, index);
}

int Preprocessor::process_define(int index)
{
	int defined_identifier_index = skip_to_next(file_tokens, index);

	if (defined_identifier_index < 0 || file_tokens[defined_identifier_index].token != pp_token::_identifier)
	{
		//must define something
		throw PreprocessorError(); //no identifier found, or next non-whitespace is not identifier

	}

	else if (predefined_macro_names.count(file_tokens[defined_identifier_index].lexeme) > 0 || file_tokens[defined_identifier_index].lexeme == "defined")
	{
		throw PreprocessorError(); //identifier is already predefined (6.10.8-2)
	}

	if (file_tokens[defined_identifier_index + 1].token == pp_token::_white_space) //object-like macro; must have white-space between name and replacement list (6.10.3-3)
	{
		vector<PPWord> replacement_string = get_rest_of_line(file_tokens, defined_identifier_index);

		string macro_name = file_tokens[defined_identifier_index].lexeme;

		if (function_macros.count(macro_name) > 0)
		{
			throw PreprocessorError(); //cannot redefine function macro as object without undef (6.10.3-2)
		}

		auto already_defined = object_macros.find(macro_name);



		if (already_defined != object_macros.end() && remove_white_space(replacement_string) != remove_white_space(already_defined->second))//equivalent definition allowed (6.10.3-2)
		{
			throw PreprocessorError(); //tried to redefine existing macro without undefining first (6.10.3-2)
		}

		macro_names.insert(macro_name);
		object_macros[macro_name] = replacement_string;

		//cout << "defined object macro: " << macro_name << ":";
		for (int i = 0; i < replacement_string.size(); i++)
		{
			//cout << " " << replacement_string[i].lexeme;
		}

		//cout << endl;

		return go_to_end_of_line(file_tokens, index);
	}

	else if (file_tokens[defined_identifier_index + 1].token == pp_token::_left_paren) //function-like macro, open paren found
	{
		int paren_index = defined_identifier_index;
		paren_index++;


		bool end = false;
		int num_args = 0;
		bool var_args = false;

		set<string> arg_name_set;
		vector<string> arg_name_list;

		int close_paren_index = -1;

		int loop_start_index = paren_index;

		while (!end)
		{
			int arg_index = skip_to_next(file_tokens, loop_start_index);

			if (arg_index < 0)
			{
				throw PreprocessorError(); //open paren with no corresponding close on end of line
			}

			switch (file_tokens[arg_index].token)
			{
			case(pp_token::_identifier):
			{
				string arg_name = file_tokens[arg_index].lexeme;

				if (arg_name_set.count(arg_name) == 0)
				{
					arg_name_set.insert(arg_name);
					arg_name_list.push_back(arg_name);
				}

				else
				{
					throw PreprocessorError(); //repeat argument name
				}

				num_args++;

				break;
			}

			case(pp_token::_right_paren):

				close_paren_index = arg_index;
				end = true;

				break;
			case(pp_token::_ellipsis):

				var_args = true;

				close_paren_index = skip_to_next(file_tokens, arg_index);

				if (close_paren_index < 0 || file_tokens[close_paren_index].token != pp_token::_right_paren)
				{
					throw PreprocessorError(); //invalid tokens following ellipsis (should always be closing paren)
				}

				arg_name_set.insert("__VA_ARGS__");
				arg_name_list.push_back("__VA_ARGS__");

				end = true;
				break;

			default:
				throw PreprocessorError(); //invalid token where argument or close paren should be
				break;
			}

			if (end)
			{
				break;
			}

			int comma_index = skip_to_next(file_tokens, arg_index);

			if (comma_index < 0)
			{
				throw PreprocessorError(); //identifier at end of line
			}

			switch (file_tokens[comma_index].token)
			{
			case(pp_token::_comma):
			{
				int next_arg_index = skip_to_next(file_tokens, comma_index);

				if (next_arg_index < 0 || (file_tokens[next_arg_index].token != pp_token::_identifier && file_tokens[next_arg_index].token != pp_token::_ellipsis))
				{
					throw PreprocessorError(); //comma is there, but there is no next argument
				}

				break;
			}
			case(pp_token::_right_paren):

				close_paren_index = comma_index;
				end = true;

				break;
			default:
				throw PreprocessorError(); //invalid token following argument
				break;
			}

			loop_start_index = comma_index;
		}

		vector<PPWord> replacement_string = get_rest_of_line(file_tokens, close_paren_index);

		if (!var_args)
		{
			for (int i = 0; i < replacement_string.size(); i++)
			{
				PPWord word = replacement_string[i];

				if (word.token == pp_token::_identifier && word.lexeme == "__VA_ARGS__")
				{
					throw PreprocessorError(); //tried to use variable args when not allowed (6.10.3-5)
				}

			}
		}

		string macro_name = file_tokens[defined_identifier_index].lexeme;

		if (object_macros.count(macro_name) > 0)
		{
			throw PreprocessorError(); //cannot redefine object macro as function without undef (6.10.3-2)
		}


		auto already_defined = function_macros.find(macro_name);

		if (already_defined != function_macros.end() &&
			!(remove_white_space(replacement_string) == remove_white_space(already_defined->second.replacement_tokens) && var_args == already_defined->second.var_args && arg_name_list == already_defined->second.arg_list))
		{
			throw PreprocessorError(); //tried to redefine existing macro without undefining first (6.10.3-2) //equivalent definitions allowed (6.10.3-2)
		}

		macro_names.insert(macro_name);
		function_macro func(macro_name, num_args, var_args, arg_name_list, replacement_string);
		function_macros[macro_name] = func;
		//func.print();
		return go_to_end_of_line(file_tokens, index);
	}

	else //error
	{
		throw PreprocessorError(); //invalid define format
		return go_to_end_of_line(file_tokens, index);
	}
}

int Preprocessor::process_undef(int  index)
{
	int id_index = skip_to_next(file_tokens, index);

	if (id_index < 0)
	{
		throw PreprocessorError(); //empty undef 
	}

	int is_end = skip_to_next(file_tokens, id_index);

	if (is_end != -1)
	{
		throw PreprocessorError(); //should not be anything else after identifier on line
	}

	string id = file_tokens[id_index].lexeme;

	if (predefined_macro_names.count(id) > 0 || id == "defined")
	{
		throw PreprocessorError(); //attempted to undef a predefined macro
	}

	macro_names.erase(id);
	object_macros.erase(id);
	function_macros.erase(id);

	return go_to_end_of_line(file_tokens, index);

}

int Preprocessor::process_error(int index)
{
	vector<PPWord> error_tokens = get_rest_of_line(file_tokens, index);

	cout << "Error directive executed: ";
	for (int i = 0; i < error_tokens.size(); i++)
	{
		cout << error_tokens[i].lexeme << " ";
	}

	exit(EXIT_FAILURE);

}

int Preprocessor::process_pragma(int index)
{
	vector<PPWord> pragma_token_string = get_rest_of_line(file_tokens, index);
	process_pragma_tokens(pragma_token_string);
	return go_to_end_of_line(file_tokens, index);
}

int Preprocessor::skip_to_next(const vector<PPWord>& word_string, int  index) //get index of next non-whitespace token on line, not including start, or -1 if rest of line is white space
{
	index++;
	while ((word_string[index].token == pp_token::_white_space || word_string[index].token == pp_token::_place_marker) && word_string[index].lexeme[0] != '\n')
	{
		index++;
	}

	if (word_string[index].token != pp_token::_white_space && word_string[index].token != pp_token::_place_marker)
	{
		return index;
	}

	else
	{
		return -1; 
	}
}


int Preprocessor::skip_to_next(const deque<PPWord>& word_string, int  index) //get index of next non-whitespace token on line, not including start, or -1 if rest of line is white space
{
	index++;
	while ((word_string[index].token == pp_token::_white_space || word_string[index].token == pp_token::_place_marker) && word_string[index].lexeme[0] != '\n')
	{
		index++;
	}

	if (word_string[index].token != pp_token::_white_space && word_string[index].token != pp_token::_place_marker)
	{
		return index;
	}

	else
	{
		return -1;
	}
}

int Preprocessor::skip_lines_to_next(const vector<PPWord>& word_string, int index) //get index of next non-whitespace token in word_string, not including start, or -1 if rest of file is white space
{
	index++;
	while (index != word_string.size() && (word_string[index].token == pp_token::_white_space || word_string[index].token == pp_token::_place_marker))
	{
		index++;
	}

	if (index == word_string.size())
	{
		return -1;
	}

	return index;
}


int Preprocessor::skip_lines_to_next(const deque<PPWord>& word_string, int index) //get index of next non-whitespace token in word_string, not including start, or -1 if rest of file is white space
{
	index++;
	while (index != word_string.size() && (word_string[index].token == pp_token::_white_space || word_string[index].token == pp_token::_place_marker))
	{
		index++;
	}

	if (index == word_string.size())
	{
		return -1;
	}

	return index;
}


vector<PPWord> Preprocessor::get_rest_of_line(const vector<PPWord>& word_string, int  index) //returns all preprocessing tokens up to new next line,  not including start, and not including leading or following whitespace
{
	vector<PPWord> out;

	index++;
	while (word_string[index].token == pp_token::_white_space && word_string[index].lexeme[0]!= '\n')
	{
		index++;
	}

	if (word_string[index].lexeme[0] == '\n') //empty replacement string
	{
		return out;
	}

	while (word_string[index].lexeme[0] != '\n')
	{
		out.push_back(word_string[index]);
		index++;
	}

	int last = out.size() - 1;

	while (out[last].token == pp_token::_white_space)
	{
		out.pop_back();
		last--;
	}

	return out; 
}

vector<PPWord> Preprocessor::get_rest_of_line(const deque<PPWord>& word_string, int index) //returns all preprocessing tokens up to new next line,  not including start, and not including leading or following whitespace
{
	vector<PPWord> out;

	index++;
	while (word_string[index].token == pp_token::_white_space && word_string[index].lexeme[0] != '\n')
	{
		index++;
	}

	if (word_string[index].lexeme[0] == '\n') //empty replacement string
	{
		return out;
	}

	while (word_string[index].lexeme[0] != '\n')
	{
		out.push_back(word_string[index]);
		index++;
	}

	int last = out.size() - 1;

	while (out[last].token == pp_token::_white_space)
	{
		out.pop_back();
		last--;
	}

	return out;
}

vector<PPWord> Preprocessor::trim_white_space(const vector<PPWord>& word_string) //returns all preprocessing tokens up to new next line,  not including start, and not including leading or following whitespace
{
	vector<PPWord> out = {};

	int index = 0;
	while (index < word_string.size() && word_string[index].token == pp_token::_white_space)
	{
		index++;
	}

	if (index == word_string.size()) //empty replacement string
	{
		return out;
	}

	while (index < word_string.size())
	{
		out.push_back(word_string[index]);
		index++;
	}

	int last = out.size() - 1;

	while (last >= 0 && out[last].token == pp_token::_white_space)
	{
		out.pop_back();
		last--;
	}

	return out;
}

vector<PPWord> Preprocessor::remove_placemarkers(const vector<PPWord>& word_string)
{
	vector<PPWord> out = {};

	for (int i = 0; i < word_string.size(); i++)
	{
		if (word_string[i].token != pp_token::_place_marker)
		{
			out.push_back(word_string[i]);
		}
	}

	return out; 
}

int  Preprocessor::go_to_end_of_line(const vector<PPWord>& word_string, int index) // return index of end of line, or word_string.size() if not found 
{
	while (index < word_string.size() && word_string[index].lexeme[0] != '\n')
	{
		index++;
	}

	return index;
}

int  Preprocessor::go_to_end_of_line(const deque<PPWord>& word_string, int index) // return index of end of line, or word_string.size() if not found 
{
	while (index < word_string.size() && word_string[index].lexeme[0] != '\n')
	{
		index++;
	}

	return index;
}

int Preprocessor::go_to_previous_line(const vector<PPWord>& word_string, int index)//return to end of previous line, starting from index, or -1, if called on first line
{
	index--; 
	while (word_string[index].lexeme[0] != '\n' && index > 0)
	{
		index--;
	}

	return index;
}


int Preprocessor::go_to_previous_line(const deque<PPWord>& word_string, int index)//return to end of previous line, starting from index, or -1, if called on first line
{
	index--;
	while (index > 0 && word_string[index].lexeme[0] != '\n')
	{
		index--;
	}

	return index;
}

vector<PPWord> Preprocessor::replace_macros(const vector<PPWord>& word_string)
{
	deque<PPWord> word_queue(word_string.begin(), word_string.end());
	vector<PPWord> out;

	while (word_queue.size() > 0)
	{
		if (word_queue[0].token == pp_token::_identifier && macro_names.count(word_queue[0].lexeme) > 0 && word_queue[0].macro_exclude.count(word_queue[0].lexeme) == 0 && word_queue[0].macro_replacable)
		{
			int index = 0;
			bool replacement_success;
			
			vector<PPWord> replacement = replace_macro(word_queue, index, replacement_success);

			if (replacement_success)
			{
				for (int i = 0; i <= index; i++)
				{
					word_queue.pop_front();
				}

				word_queue.insert(word_queue.begin(), replacement.begin(), replacement.end());
			}

			else //name is not a start of macro in this string
			{
				out.push_back(word_queue[0]);
				word_queue.pop_front();
			}
		}

		else if (word_queue[0].token == pp_token::_identifier && macro_names.count(word_queue[0].lexeme) > 0 && word_queue[0].macro_exclude.count(word_queue[0].lexeme) > 0)
		{
			word_queue[0].macro_replacable = false;
			out.push_back(word_queue[0]);
			word_queue.pop_front();
		}

		else
		{
			out.push_back(word_queue[0]);
			word_queue.pop_front();
		}
	}
	return out;
}

vector<PPWord> Preprocessor::replace_macro(const deque<PPWord>& word_queue, int& index, bool& success)
{
	string lexeme = word_queue[index].lexeme;

	vector<PPWord> out;

	int next = skip_lines_to_next(word_queue, index);

	if (object_macros.count(lexeme) > 0 && word_queue[index].macro_exclude.count(lexeme) == 0 && word_queue[index].macro_replacable)
	{
		//cout << "replacing " << lexeme << " macro" << endl;
		out = replace_object_macro(word_queue, index);
		for (int i = 0; i < out.size(); i++)
		{
			out[i].macro_exclude.insert(lexeme);
		}

		success = true;
	}

	else if (function_macros.count(lexeme) > 0 && next > 0 && next < word_queue.size() && word_queue[next].token == pp_token::_left_paren && word_queue[index].macro_exclude.count(lexeme) == 0 && word_queue[index].macro_replacable)
	{
		//cout << "replacing " << lexeme << " macro" << endl;
		out = replace_function_macro(word_queue, index);
		for (int i = 0; i < out.size(); i++)
		{
			out[i].macro_exclude.insert(lexeme);
		}
		success = true;
	}

	else //no replacement occurs
	{
		out = {word_queue[index]};
		success = false; 
	}
	return out;
}

vector<PPWord> Preprocessor::replace_object_macro(const deque<PPWord>& word_queue, int& index)
{
	pp_token token = word_queue[index].token;
	string name = word_queue[index].lexeme;

	vector<PPWord> out; 

	if (token != pp_token::_identifier || object_macros.count(name) == 0)
	{
		throw PreprocessorInternalError(); //tried to replace object that isn't an identifier
	}

	update_object_macros(index);

	vector<PPWord> replacement_string = object_macros[name];

	for (int i = 0; i < replacement_string.size(); i++)
	{
		replacement_string[i].macro_exclude.insert(name);
		out.push_back(replacement_string[i]);
	}

	//continue scanning from very next index

	return out;
}

vector<PPWord> Preprocessor::replace_function_macro(const deque<PPWord>& word_queue, int& index)
{
	pp_token token = word_queue[index].token;
	string name = word_queue[index].lexeme;

	if (token != pp_token::_identifier || function_macros.count(name) == 0)
	{
		throw PreprocessorInternalError(); //tried to replace object that isn't a function macro
	}

	function_macro func = function_macros[name];
	vector<PPWord> replacement_string = func.replacement_tokens;
	
	int l_paren = skip_lines_to_next(word_queue, index);
	int next_to = skip_lines_to_next(word_queue, l_paren);

	vector<PPWord> out;

	if (word_queue[next_to].token == pp_token::_right_paren) //0 args or 1 empty arg
	{
		if (func.num_args > 1)
		{
			throw PreprocessorError(); //not enough arguments
		}

		else if (func.num_args == 0) //0 args
		{
			for (int i = 0; i < replacement_string.size(); i++)
			{
				PPWord replace_word = replacement_string[i];

				if (!(replace_word.lexeme == "__VA_ARGS__" && replace_word.token == pp_token::_identifier))
				{
					out.push_back(replace_word);
				}
			}
		}

		else //1 empty arg
		{
			for (int i = 0; i < replacement_string.size(); i++)
			{
				PPWord replace_word = replacement_string[i];

				if (!(replace_word.lexeme == "__VA_ARGS__" || func.arg_set.count(replace_word.lexeme) > 0 || replace_word.token == pp_token::_hashtag))
				{
					out.push_back(replace_word);
				}
				
				else if (replace_word.token == pp_token::_hashtag) //stringization -> empty string
				{
					out.push_back(PPWord("\"\"", pp_token::_string_literal, -1));
				}

				else if (replace_word.lexeme == "__VA_ARGS__" || func.arg_set.count(replace_word.lexeme) > 0)
				{
					out.push_back(PPWord("", pp_token::_place_marker, word_queue[l_paren].line_number));
				}
			}
		}

		index = next_to;

		vector<PPWord> arg_ops_out = remove_placemarkers(perform_argument_ops(out));
		
		for (int i = 0; i < arg_ops_out.size(); i++)
		{
			arg_ops_out[i].macro_exclude.insert(name);
		}

		out = arg_ops_out;

		return out; 
	}
	
	int num_args = 1; //0 arg function handled above
	int n_internal_parens = 0; //= number of open left parens (not including first) without corresponding close parens
	bool end_of_arg_list = false;
	int j = l_paren + 1;

	vector<vector<PPWord>> args; //will hold pp-token strings corresponding to arguments
	args.push_back({});
	int current_arg_index = 0;

	int return_end_index = -1;

	while (j != -1 && j < word_queue.size() && !end_of_arg_list)
	{
		PPWord word = word_queue[j];

		switch (word.token)
		{
		case(pp_token::_left_paren):
			
			n_internal_parens++;
			args[current_arg_index].push_back(word);
			j++;
			break;

		case(pp_token::_right_paren):

			if (n_internal_parens == 0) //end found
			{	
				return_end_index = j;
				end_of_arg_list = true;
			}
			else
			{
				n_internal_parens--;
				args[current_arg_index].push_back(word);
				j++;
			}
			
			break;

		case(pp_token::_comma):

			if (n_internal_parens == 0) //arg end found
			{
				if (args[current_arg_index].empty()) //for use with ## operations (6.10.3.3-2)
				{
					args[current_arg_index].push_back(PPWord("", pp_token::_place_marker, word.line_number + current_line_number_offset));
				}
				args.push_back({});
				current_arg_index++; 
				num_args++;
				j++;
			}

			else
			{
				args[current_arg_index].push_back(word);
				j++;
			}

			break;

		default:
			args[current_arg_index].push_back(word);
			j++;
			break;
		}
	}

	if (!end_of_arg_list|| num_args < func.num_args || (!func.var_args && num_args > func.num_args))
	{
		throw PreprocessorError(); // arg list error (unfinished, too many, or too few)
	}

	//map argument names to replacements
	map<string, vector<PPWord>> non_macro_replace_map; //arguments are not macro replaced (for # ,## ops)
	map<string, vector<PPWord>> macro_replace_map;  //arguments are macro replaced

	vector<vector<PPWord>> macro_replaced_args(num_args);

	for (int i = 0; i < num_args; i++)
	{
		vector<PPWord> trimmed_arg = trim_white_space(args[i]);

		args[i] = trimmed_arg;

		if (args[i].empty())
		{
			args[i].push_back(PPWord("", pp_token::_place_marker));
		}

		vector<PPWord> macro_replaced_arg = replace_macros(args[i]);
		macro_replaced_args[i] = macro_replaced_arg;

		string replace_string = func.arg_list[min(i, static_cast<int>(func.arg_list.size()) - 1)];

		if (replace_string == "__VA_ARGS__" && !macro_replace_map[replace_string].empty())
		{
			macro_replace_map[replace_string].push_back(PPWord(",", pp_token::_comma));
			non_macro_replace_map[replace_string].push_back(PPWord(",", pp_token::_comma));
		}

		macro_replace_map[replace_string].insert(macro_replace_map[replace_string].end(), macro_replaced_args[i].begin(), macro_replaced_args[i].end());
		non_macro_replace_map[replace_string].insert(non_macro_replace_map[replace_string].end(), args[i].begin(), args[i].end());

	}
	
	for (int i = 0; i < func.replacement_tokens.size(); i++)
	{
		PPWord replacement_token = func.replacement_tokens[i];

		if (replacement_token.token == pp_token::_identifier && macro_replace_map.count(replacement_token.lexeme) > 0)
		{
			vector<PPWord> substitute_tokens;

			if ((i - 1 >= 0 && func.replacement_tokens[i - 1].token == pp_token::_hashtag_hashtag) || (i + 1 < func.replacement_tokens.size() && func.replacement_tokens[i+1].token == pp_token::_hashtag_hashtag))
			{
				substitute_tokens = non_macro_replace_map[replacement_token.lexeme];
			}

			else
			{
				substitute_tokens = macro_replace_map[replacement_token.lexeme];
			}

			for (int j = 0; j < substitute_tokens.size(); j++)
			{
				out.push_back(substitute_tokens[j]);

			}
		}
		
		else if (replacement_token.token == pp_token::_hashtag) //i+1 < func.replacement_tokens.size(), and next token is argument since we check for this in macro constructor //handle # operator right here
		{
			vector<char> char_string;
			
			char_string.push_back('\"');

			vector<PPWord> substitute_tokens = non_macro_replace_map[func.replacement_tokens[i + 1].lexeme];//this must be arg by above

			for (int j = 0; j < substitute_tokens.size(); j++)
			{
				string lex = substitute_tokens[j].lexeme;

				for (int k = 0; k < lex.size(); k++)
				{
					if (lex[k] == '\"') //(6.10.3.2-2)
					{
						char_string.push_back('\\');
						char_string.push_back('\"');
					}

					else if (lex[k] == '\\')
					{
						char_string.push_back('\\');
						char_string.push_back('\\');
					}

					else if (substitute_tokens[j].token == pp_token::_white_space)
					{
						char_string.push_back(' ');
					}

					else
					{
						char_string.push_back(lex[k]);
					}
				}
			}

			char_string.push_back('\"');

			string out_string(char_string.begin(), char_string.end());
			out.push_back(PPWord(out_string, pp_token::_string_literal, word_queue[return_end_index].line_number + current_line_number_offset));

			i++; //already handled next argument
		}

		else
		{
			out.push_back(replacement_token);
		}
	}

	index = return_end_index;
	
	vector<PPWord> arg_ops_out = remove_placemarkers(perform_argument_ops(out));

	for (int i = 0; i < arg_ops_out.size(); i++)
	{
		arg_ops_out[i].macro_exclude.insert(name);
	}

	out = arg_ops_out;
	return out;	
}

vector<PPWord> Preprocessor::perform_argument_ops(const vector<PPWord>& word_list)
{
	list<PPWord> token_list;
	token_list.insert(token_list.end(), word_list.begin(), word_list.end());

	auto i = token_list.begin();

	while (i != token_list.end()) //## first
	{
		if ((*i).token == pp_token::_hashtag_hashtag)
		{
			
			auto prev = std::next(i, -1); //these are both in list, since we checked for ## at begin and end in function macro constructor
			auto next = std::next(i, 1);

			//cout << "## found between " << (*prev).lexeme << " and " << (*next).lexeme;

			string new_lexeme = (*prev).lexeme + (*next).lexeme;
			pp_token new_token = pp_token::_place_marker;

			if (new_lexeme.size() == 0)
			{
				next++;

				i = token_list.insert(prev, PPWord(new_lexeme, new_token, (*i).line_number + current_line_number_offset));
				token_list.erase(prev, next);
				continue;
			}

			int category = C_tokenizer.accepts(new_lexeme);

			if (category >= 0)
			{
				new_token = static_cast<pp_token>(category);

				if (category == static_cast<int>(pp_token::_punctuator)) //replace digraphs and categorize punctuators
				{
					if (digraph_map.find(new_lexeme) != digraph_map.end())
					{
						new_lexeme = digraph_map.at(new_lexeme);
					}
					new_token = punctuator_map.at(new_lexeme);

				}

				next++;

				i = token_list.insert(prev, PPWord(new_lexeme, new_token, (*i).line_number + current_line_number_offset));
				token_list.erase(prev, next);

				continue;
			}

			else
			{
				throw PreprocessorError(); //invalid token created through concatenation
			}
		}
		i++;
	}

	vector<PPWord> out;
	
	for (auto i = token_list.begin(); i != token_list.end(); i++)
	{
		if ((*i).token != pp_token::_place_marker)
		{
			out.push_back((*i));
		}
	}
	return out;
}

vector<PPWord> Preprocessor::remove_white_space(const vector<PPWord>& word_list)
{
	vector<PPWord> out;

	for (int i = 0; i < word_list.size(); i++)
	{
		if (word_list[i].token != pp_token::_white_space)
		{
			out.push_back(word_list[i]);
		}
	}
	return out;
}

void Preprocessor::update_object_macros(int index)
{
	object_macros["__LINE__"] = {PPWord(to_string(file_tokens[index].line_number + current_line_number_offset), pp_token::_pp_number, file_tokens[index].line_number + current_line_number_offset)};
	object_macros["__FILE__"] = {PPWord("\"" + file_name + "\"", pp_token::_string_literal, file_tokens[index].line_number + current_line_number_offset)};
	object_macros["__DATE__"] = { PPWord(date_string, pp_token::_string_literal, file_tokens[index].line_number + current_line_number_offset)};
	object_macros["__TIME__"] = { PPWord(time_string, pp_token::_string_literal, file_tokens[index].line_number + current_line_number_offset)};
}

vector<Word> Preprocessor::convert_to_tokens(const vector<PPWord>& pp_words)
{
	vector<Word> out;

	for (int i = 0; i < pp_words.size(); i++)
	{
		string lexeme = pp_words[i].lexeme;
		pp_token current_token = pp_words[i].token;

		switch (current_token)
		{
		case(pp_token::_identifier):
			if (keywords.count(lexeme) > 0)
			{
				out.push_back(Word(lexeme, keyword_token_map.at(lexeme)));
			}

			else
			{
				out.push_back(Word(lexeme, token::_identifier));
			}

			break;

		case(pp_token::_header_name):

			throw TokenConversionError();
			break;

		case(pp_token::_pp_number):
		{
			int num_category = C_num_categorizer.accepts(lexeme);

			if (num_category < 0)
			{
				throw TokenConversionError();
			}

			else
			{
				out.push_back(Word(lexeme, static_cast<token>(num_category)));
			}
			break;
		}
		case(pp_token::_white_space):
			break;

		case(pp_token:: _string_literal): //5.1.1.2-6 (string literal concatenation)
		{
			string final_string = ""; //5.1.1.2-5

			while (i < pp_words.size() && (current_token == pp_token::_place_marker || current_token == pp_token::_white_space || current_token == pp_token::_string_literal))
			{
				if (current_token == pp_token::_string_literal)
				{
					string lexeme = pp_words[i].lexeme;
					string string_chars = lexeme.substr(1, lexeme.size() - 2);

					final_string += string_chars;
				}
				i++;
				current_token = pp_words[i].token;
			}

			i--;

			out.push_back(Word(final_string, token::_string_literal));

			break;
		}
		case(pp_token::_place_marker):
			break;
		case(pp_token::_invalid):
			throw TokenConversionError();
			break;

		default:
			out.push_back(Word(lexeme, token_conversion_map.at(current_token)));
			break;
		}
	}

	return out; 

}

bool Preprocessor::evaluate_constant_expression(const vector<PPWord>& expression)
{
	vector<Word> expression_words = convert_to_tokens(expression);
	
	for (int i = 0; i < expression_words.size(); i++)
	{
		if (expression_tokens.count(expression_words[i].terminal) == 0)
		{
			throw PreprocessorError(); //invalid expression
		}
	}

	ParseTree expression_tree = expression_grammar.LR1_parse(expression_words);

	int expression_value = evaluate_expression_tree(&expression_tree.get_root());

	return expression_value != 0;
}
