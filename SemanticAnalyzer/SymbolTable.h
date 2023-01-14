#pragma once

#include "C_Compiler/ContextFreeGrammar.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include "TypeGraph.h"

enum class ScopeType { _file, _function, _block, _function_prot};


struct TypeTableEntry
{
	bool is_null;
	TypeTree type_tree;

	TypeTableEntry():
		is_null(true)
	{}

	TypeTableEntry(TypeTree type) :
		is_null(false),
		type_tree(type)
	{}
};

struct Symbol
{	
	std::string id_name;

	TypeTableEntry* type;  

	std::set<TypeQualifier> qualifiers;
	int alignment;


	Symbol() :
		id_name(""),
		type(NULL)
	{}

	Symbol(std::string id, TypeTableEntry type_table_entry) :
		id_name(id),
		type(&type_table_entry)
	{}
};

struct Scope
{
	ScopeType scope_type;

	Scope* enclosure = NULL;

	std::map<std::string, Symbol> ordinary_identifiers; //enum constants, regular variables
	std::map<std::string, TypeTableEntry> type_table; //maps struct, union, and enum tags to their definitions, as well as derived types to their definitions. Types not named in the program are given compiler generated tags
	std::map<std::string, TypeTableEntry> typedef_entries; //shares namespace with ordinary_identifiers

	Scope() :
		enclosure(NULL),
		ordinary_identifiers({}),
		type_table({}),
		typedef_entries({})
	{}
};

class SymbolTable //outermost scope is file scope
{
private:

	std::set<Scope*> scopes;
	Scope* current_scope;

	int current_anon_tag = 0; //anonymous types will be assigned a tag by the compiler; this tag will just be a number (in string form) which is tracked by this int

public:
	
	Symbol find_identifier(std::string id_name);  //find information about a variable based on its name
	TypeTableEntry find_type(std::string type_tag); //find a struct, enum, or union type or anonymous type via its tag.
	TypeTableEntry find_typedef(std::string typedef_name); 

	bool check_identifier(std::string id_name);
	bool check_type(std::string type_tag);
	bool check_typedef(std::string typedef_name);

	void enter_new_scope();
	void exit_current_scope();

	void insert_tag_symbol(std::string new_tag, TypeTableEntry new_symbol); //declare new types, update existing ones
	void insert_id_symbol(Symbol new_symbol); //declare new variable or function
	void insert_typedef(std::string typedef_name, TypeTableEntry new_type); //declare new typedef'd type

	int find_enum_value(std::string enum_tag);


	SymbolTable();
	~SymbolTable();
};

class SymbolNotFound : std::exception
{
	virtual const char* what() const throw()
	{
		return "symbol not found";
	}
};




