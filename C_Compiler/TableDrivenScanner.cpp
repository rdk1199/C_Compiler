
#include "TableDrivenScanner.h"
#include "CSyntax.h"
#include <stack>
#include <string>

using std::stack;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

TableDrivenScanner::TableDrivenScanner(const string& file_name, const DFA& dfa, int buffer_sz) :
	source_name(file_name),
	tokenizer(dfa),
	buffer_size(buffer_sz),
	buffer({}),
	buffer_fence(0),
	buffer_ptr(0),
	from_file(true)
{
	source.open(file_name);

	if (!source.is_open())
	{
		throw FileNotFoundError(file_name);
	}

	buffer.resize(2*buffer_size);

	
	int end_index = 0;

	for (int i = 0; i < buffer_size; i++)
	{
		char next = get_char();
		buffer[i] = next;
		end_index++;
		if (source.eof() || next <= EOF)
		{
			buffer[i] = EOF;
			break;
		}
	}
	
	//print_buffer();

	for (int i = end_index; i < 2 * buffer_size; i++)
	{
		buffer[i] = '@';
	}

	//print_buffer();
}

TableDrivenScanner::TableDrivenScanner(string s, const DFA& dfa) :
	source_name(""),
	tokenizer(dfa),
	buffer_size(s.size() + 1),
	buffer({}),
	buffer_fence(0),
	buffer_ptr(0),
	from_file(false)
{
	buffer.resize(2 * buffer_size);
	for (int i = 0; i <= s.size(); i++)
	{
		buffer[i] = s[i];
		if (buffer[i] <= EOF)
		{
			break;
		}
	}

	buffer[s.size()] = EOF;
}

void TableDrivenScanner::print_buffer()
{
	cout << "[";

	for (int i = 0; i < buffer.size(); i++)
	{
		if (i == buffer_fence)
		{
			cout << "|";
		}

		if (i == buffer_ptr)
		{
			cout << " {" << buffer[i] << "}";
		}

		else
		{
			cout << " " << buffer[i];
		}

		
	}

	cout << "]" << endl;
}

char TableDrivenScanner::peek_next()
{
	return buffer[buffer_ptr];
}

char TableDrivenScanner::next_char()
{
	char c = buffer[buffer_ptr];

	buffer_ptr = (buffer_ptr + 1) % (2*buffer_size);

	if (!from_file)
	{
		return c;
	}

	if (buffer_ptr % buffer_size == 0)
	{
		if (!past_fill)
		{
			//cout << "refilling buffer" << endl;

			//print_buffer();


			for (int i = buffer_ptr; i < buffer_ptr + buffer_size; i++)
			{
				char next = get_char();
				//buffer[i] = next;
				if (next <= EOF || source.eof())
				{
					buffer[i] = EOF;
					//cout << "EOF found";
					break;
				}

				else
				{
					buffer[i] = next;
				}
			}
			buffer_fence = (buffer_ptr + buffer_size) % (2 * buffer_size);

		//	cout << "after refill: " << endl;

			//print_buffer();
		}

		else
		{
			past_fill = false; 
		}
	}
	return c;
}

char TableDrivenScanner::get_char() //handles trigraph sequences and line merges (stages 1 and 2)
{
	char c;

	source.get(c);

	//cout << "got char " << c << endl;

	if (c != '?' && c != '\\')
		return c;

	switch (c) 
	{
		case '?':
			if (source.peek() != '?')
			{
				return c;
			}
			else
			{
				source.get();
				if (trigraph_map.count(source.peek()) > 0)
				{
					source.get(c);
					return trigraph_map.at(c);
				}

				else
				{
					source.clear();
					source.unget();
					return c;
				}
			}
			break;
		case '\\':
			if (source.peek() == '\n')
			{
				source.get(c);
				if (source.peek() != EOF)
				{
					source.get(c);
					current_line_num++;
					return c;
				}
				else
				{
					return c;
					throw InvalidLineMerge(source_name);
				}
			}
			else
			{
				return c;
			}
			break;
		default:
			return c;
			break;
	}

}

void TableDrivenScanner::move_back()
{
	//cout << buffer_ptr << " " << buffer_fence << endl;

	if (buffer_ptr == buffer_fence)
	{
		throw(BufferUnderflow());
	}

	if (buffer_ptr % buffer_size == 0)
	{
		past_fill = true;
	}

	buffer_ptr = (2 * buffer_size + buffer_ptr - 1) % (2 * buffer_size);
}

PPWord TableDrivenScanner::next_word()
{
	stack<int> prev_states;
	vector<char> lexeme_chain;

	int current_state = 0;
	prev_states.push(current_state);

	char c = '\0';

	while (current_state != -1)
	{
		c = next_char();

		if(c <= EOF)
		{
			//source.clear();
			lexeme_chain.push_back(EOF);
			prev_states.push(-1);
			break;
		}

		current_state = tokenizer[current_state].at(c);

		lexeme_chain.push_back(c);
		prev_states.push(current_state);

		//cout << c << " : " << current_state << endl;
	}

	//cout << "rolling back" << endl;

	current_state = prev_states.top();
	prev_states.pop();

	//cout << "starting move back" << endl;

	while (lexeme_chain.size() > 0 && !tokenizer.accepts(current_state))
	{
		current_state = prev_states.top();
		prev_states.pop();
		move_back();
		lexeme_chain.pop_back();
	}

	//cout << "finished moving back" << endl;
	string lexeme(lexeme_chain.begin(), lexeme_chain.end());

	if (tokenizer.accepts(current_state))
	{
		int category = tokenizer.categorize(current_state);

		if (category == static_cast<int>(pp_token::_white_space)) //replace all white-space, including comments, with one space; preserve new-lines
		{

			int num_new_lines = std::count(lexeme_chain.begin(), lexeme_chain.end(), '\n');

			if (num_new_lines == 0)
			{
				lexeme = " ";
			}

			else
			{
				lexeme = string(num_new_lines, '\n');
				current_line_num += num_new_lines;
			}
		}

		else if (category == static_cast<int>(pp_token::_punctuator)) //replace digraphs
		{
			if (digraph_map.find(lexeme) != digraph_map.end())
			{
				lexeme = digraph_map.at(lexeme);
			}

			return PPWord(lexeme, punctuator_map.at(lexeme), current_line_num);

		}
		return PPWord(lexeme, pp_token(category), current_line_num);
	}
	else
	{
		throw InvalidToken(PPWord(lexeme, pp_token::_invalid, current_line_num));
	}
	return PPWord(lexeme, pp_token::_invalid, current_line_num);
}

vector<PPWord> TableDrivenScanner::retokenize_header(const PPWord& header_name_token)
{
	vector<PPWord> out = {};

	PPWord start = PPWord("<", pp_token::_less_than, header_name_token.line_number);

	out.push_back(start);

	string remainder = header_name_token.lexeme.substr(1, header_name_token.lexeme.size() - 1);

	TableDrivenScanner scanner(remainder, C_tokenizer);

	vector<PPWord> new_tokens = scanner.scan();

	for (int i = 0; i < new_tokens.size(); i++)
	{
		new_tokens[i].line_number = header_name_token.line_number;

		if (new_tokens[i].token == pp_token::_header_name) //another header found 
		{
			vector<PPWord> new_tokens = retokenize_header(new_tokens[i]);
			out.insert(out.end(), new_tokens.begin(), new_tokens.end());
		}

		out.push_back(new_tokens[i]);
	}

	return out;
}

bool check_for_header(const vector<PPWord>& tokens) //scan backwards in token string looking for #include and start of line/file
{
	auto current = tokens.rbegin();

	while (current != tokens.rend() && current->token == pp_token::_white_space)
	{
		if (current->lexeme[0] == '\n')
		{
			return false;
		}

		current++;
	}

	if (current == tokens.rend() || current->lexeme != "include")
	{
		return false;
	}

	current++;

	while (current != tokens.rend() && current->token == pp_token::_white_space)
	{
		if (current->lexeme[0] == '\n')
		{
			return false;
		}

		current++;
	}
	
	if (current == tokens.rend() || current->lexeme != "#")
	{
		return false;
	}

	current++;

	while (current != tokens.rend() && current->token == pp_token::_white_space)
	{
		if (current->lexeme[0] == '\n')
		{
			return true;
		}

		current++;
	}

	if (current == tokens.rend())
	{
		return true;
	}

	else
	{
		return false; 
	}
}

vector<PPWord> TableDrivenScanner::scan()
{
	vector<PPWord> out;

	while (peek_next() > EOF)
	{
		PPWord word("", pp_token::_invalid);

		try
		{
			word = next_word();

			if (word.token == pp_token::_header_name && !check_for_header(out))
			{
				vector<PPWord> retokenized_header = retokenize_header(word);
				out.insert(out.end(), retokenized_header.begin(), retokenized_header.end());
			}

			else
			{
				out.push_back(word);
			}
		}

		catch (InvalidToken i_token)
		{
			cout << i_token.out << endl;
			cout << "invalid token detected; current buffer: " << endl;
			print_buffer();
			source.close();
			break;
		}
	}

	out.push_back(PPWord("\n", pp_token::_white_space, current_line_num)); //5.1.1.2-2
	out.push_back(PPWord("", pp_token::_EOF, current_line_num + 1));
	source.close();

	return out; 
}

