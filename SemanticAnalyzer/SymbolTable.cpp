#include "SymbolTable.h"
#include "SemanticAnalyzerError.h"

using std::map;
using std::multiset;
using std::set;
using std::string;
using std::vector;

using std::stringstream;

SymbolTable::SymbolTable():
	scopes({}),
	current_scope(NULL)
{
	enter_new_scope();
}

Symbol SymbolTable::find_identifier(string id_name) //variables
{
	Scope* current = current_scope;

	while (current != NULL)
	{
		auto check = current->ordinary_identifiers.find(id_name);

		if (check != current->ordinary_identifiers.end())
		{
			return check->second;
		}

		current = current->enclosure;
	}

	throw SymbolNotFound();
	return Symbol();
}

TypeTableEntry SymbolTable::find_type(string type_tag)
{
	Scope* current = current_scope;
	
	while (current != NULL)
	{
		auto check = current->type_table.find(type_tag);

		if (check != current->type_table.end())
		{
			return check->second;
		}

		current = current->enclosure;

	}
	return TypeTableEntry();
}

TypeTableEntry SymbolTable::find_typedef(string typedef_name)
{
	Scope* current = current_scope;

	while (current != NULL)
	{
		auto check = current->typedef_entries.find(typedef_name);

		if (check != current->typedef_entries.end())
		{
			return check->second;
		}

		current = current->enclosure;
	}

	return TypeTableEntry();
}

bool SymbolTable::check_identifier(string id_name) //variables
{
	Scope* current = current_scope;

	while (current != NULL)
	{
		auto check = current->ordinary_identifiers.find(id_name);

		if (check != current->ordinary_identifiers.end())
		{
			return true;
		}

		current = current->enclosure;
	}

	return false;
}

bool SymbolTable::check_type(string type_tag)
{
	Scope* current = current_scope;

	while (current != NULL)
	{
		auto check = current->type_table.find(type_tag);

		if (check != current->type_table.end())
		{
			return true;
		}

		current = current->enclosure;

	}
	return false;
}

bool SymbolTable::check_typedef(string typedef_name)
{
	Scope* current = current_scope;

	while (current != NULL)
	{
		auto check = current->typedef_entries.find(typedef_name);

		if (check != current->typedef_entries.end())
		{
			return true;
		}

		current = current->enclosure;
	}

	return false; 
}

void SymbolTable::enter_new_scope()
{
	Scope* new_scope = new Scope();

	new_scope->enclosure = current_scope;

	current_scope = new_scope;
	scopes.insert(new_scope);
}

void SymbolTable::exit_current_scope()
{
	current_scope = current_scope->enclosure;
}

//======================================== MUST ADD DUPLICATE CHECKING =====================================//

void SymbolTable::insert_id_symbol(Symbol new_symbol) //TODO
{
	current_scope->ordinary_identifiers[new_symbol.id_name] = new_symbol;
}

void SymbolTable::insert_tag_symbol(string new_tag, TypeTableEntry new_symbol)// TODO
{
	if (new_tag.empty())
	{
		current_anon_tag++;
		new_tag = std::to_string(current_anon_tag);
	}

	current_scope->type_table[new_tag] = new_symbol;
}

void SymbolTable::insert_typedef(string typedef_name, TypeTableEntry new_type)
{
	current_scope->typedef_entries[typedef_name] = new_type;
}

//===========================================================================================================//

SymbolTable::~SymbolTable()
{
	for (Scope* s : scopes)
	{
		delete s;
	}
}