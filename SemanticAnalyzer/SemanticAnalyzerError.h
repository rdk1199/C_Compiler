#pragma once

#include <exception>

class InternalCallError : std::exception
{
	virtual const char* what() const throw()
	{
		return "invalid internal call";
	}
};

class InvalidParseTree : std::exception
{
	virtual const char* what() const throw()
	{
		return "invalid parse tree";
	}
};

class InvalidDeclaration : std::exception
{
	virtual const char* what() const throw()
	{
		return "Declaration Error!";
	}
};

class IncompleteTypeError : std::exception
{
	virtual const char* what() const throw()
	{
		return "tried to use an incomplete type where one is not allowed!";
	}
};

class IncompleteEnumDeclaration : std::exception
{
	virtual const char* what() const throw()
	{
		return "declared incomplete enum type";
	}
};

class RedeclaredIdentifier : std::exception
{
	virtual const char* what() const throw()
	{
		return "redeclared identifier";
	}
};