#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

struct Symbol;

enum class StorageClassSpecifier { _typedef, _extern, _static, _thread_local, _auto, _register };
enum class TypeQualifier { _const, _restrict_, _volatile, _atomic };
enum class FunctionSpecifier { _in_line, _no_return };
enum class TypeSpecifier { _void, _char, _short, _int, _long, _float, _double, _signed, _unsigned, _Bool, _Complex, _atomic, _struct, _union, _enum, _typedef_name };

enum class TypeCategory { _null = -1, _basic, _function, _array, _pointer, _struct, _union, _enum, _atomic};

enum class BasicType {
	_Bool, _char, _signed_char, _short_int, _int, _long_int, _long_long_int, _unsigned_char, _unsigned_short, _unsigned_int, _unsigned_long, _unsigned_long_long,
	_float, _double, _long_double, _float_Complex, _double_Complex, _long_double_Complex, _void
};

extern const std::map<BasicType, std::string> basic_type_name_map;

extern const std::map<BasicType, int> size_map;
extern const std::map<std::multiset<TypeSpecifier>, BasicType> type_map;

extern const std::map<std::string, TypeQualifier> type_qual_map;

struct Field //used for struct/union fields as well as function arguments
{
	std::string name;

	bool named_type;

	TypeNode* field_type = NULL;
	std::string field_tag = "";

	int size = -1; //size in bits, for bit fields

};

struct TypeNode
{
	TypeCategory category; 
	
	bool complete;
	bool defined;
	bool variably_modified = false;
	bool atomic = false;

	TypeNode(TypeCategory _category);
};

struct BasicTypeNode:public TypeNode
{
	BasicType type;

	BasicTypeNode(BasicType basic_type);
};

struct ArrayTypeNode : public TypeNode
{
	bool named_element_type;

	TypeNode* element_type = NULL;
	std::string element_tag = "";

	int size;
	bool variable_length = false;
};

struct EnumTypeNode : public TypeNode
{
	std::map<std::string, int> values;

	EnumTypeNode(std::map<std::string, int> enum_values);
};

struct StructTypeNode : public TypeNode
{
	std::vector<Field> fields = {};
};

struct UnionTypeNode : public TypeNode
{
	std::vector<Field> fields = {};
};

struct FunctionTypeNode : public TypeNode
{
	bool named_return_type;

	TypeNode* return_type = NULL;
	std::string return_tag = "";

	std::vector<Field> parameters = {};
	bool variadic = false; 
};

struct PointerTypeNode : public TypeNode
{
	bool named_ref_type;

	TypeNode* ref_type = NULL;
	std::string ref_tag = "";

	set<TypeQualifier> qualifiers;

	PointerTypeNode(std::string ref_type_tag);
	PointerTypeNode(TypeNode* ref_type_ptr);
};

class TypeTree
{
private: 

	TypeCategory base_type;
	TypeNode* root; 
	

public:

	TypeTree(TypeNode* _root):
		root(_root),
		base_type(_root->category)
	{}

	TypeTree() :
		root(NULL),
		base_type(TypeCategory::_null)
	{}

	TypeNode* get_root() { return root; }
	TypeCategory get_base_type() { return base_type; }

	void print_type();
};

