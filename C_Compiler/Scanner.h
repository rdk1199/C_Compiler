#pragma once

#include "CSyntax.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <iostream>
#include <map>

class Scanner
{
public: 

	virtual std::vector<PPWord> scan() = 0;
	virtual PPWord next_word() = 0;

};

class InvalidToken : std::exception
{
public:

	std::string out;

	InvalidToken(PPWord token)
	{
		out = "Invalid token found: word: <"  + token.lexeme +  "> line: " + std::to_string(static_cast<int>(token.line_number));
	}

private:
	
	
	virtual const char* what() const throw()
	{
		return out.c_str();
	}
};