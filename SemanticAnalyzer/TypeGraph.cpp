#include "TypeGraph.h"

using std::map;
using std::string;
using std::set;
using std::vector;
using std::multiset;


const map<BasicType, int> size_map =
{
	{BasicType::_Bool, 1},
	{BasicType::_char, 1},
	{BasicType::_signed_char, 1},
	{BasicType::_unsigned_char, 1},
	{BasicType::_short_int, 2},
	{BasicType::_unsigned_short, 4},
	{BasicType::_int, 4},
	{BasicType::_unsigned_int, 4},
	{BasicType::_long_int, 4},
	{BasicType::_unsigned_long, 4},
	{BasicType::_long_long_int, 8},
	{BasicType::_unsigned_long_long, 8},
	{BasicType::_float, 4},
	{BasicType::_double, 8},
	{BasicType::_long_double, 8},
	{BasicType::_float_Complex, 8},
	{BasicType::_double_Complex, 16},
	{BasicType::_long_double_Complex, 16},
};

const map<BasicType, string> basic_type_name_map =
{
	{BasicType::_Bool, "bool"},
	{BasicType::_char, "char"},
	{BasicType::_signed_char, "signed char"},
	{BasicType::_unsigned_char, "unsigned char"},
	{BasicType::_short_int, "short"},
	{BasicType::_unsigned_short, "unsigned short"},
	{BasicType::_int, "int"},
	{BasicType::_unsigned_int, "unsigned int"},
	{BasicType::_long_int, "long"},
	{BasicType::_unsigned_long, "unsigned long"},
	{BasicType::_long_long_int, "long long"},
	{BasicType::_unsigned_long_long, "unsigned long long"},
	{BasicType::_float, "float"},
	{BasicType::_double, "double"},
	{BasicType::_long_double, "long double"},
	{BasicType::_float_Complex, "complex"},
	{BasicType::_double_Complex, "double complex"},
	{BasicType::_long_double_Complex, "long double complex"},
};

const map<multiset<TypeSpecifier>, BasicType> type_map =
{
	{{TypeSpecifier::_void}, BasicType::_void},
	{{TypeSpecifier::_char}, BasicType::_char},
	{{TypeSpecifier::_signed, TypeSpecifier::_char}, BasicType::_signed_char},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_char}, BasicType::_unsigned_char},
	{{TypeSpecifier::_short}, BasicType::_short_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_short}, BasicType::_short_int},
	{{TypeSpecifier::_int, TypeSpecifier::_short}, BasicType::_short_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_short, TypeSpecifier::_int}, BasicType::_short_int},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_short}, BasicType::_unsigned_short},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_short, TypeSpecifier::_int}, BasicType::_unsigned_short},
	{{TypeSpecifier::_int}, BasicType::_int},
	{{TypeSpecifier::_signed}, BasicType::_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_int}, BasicType::_int},
	{{TypeSpecifier::_unsigned}, BasicType::_unsigned_int},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_int}, BasicType::_unsigned_int},
	{{TypeSpecifier::_long, TypeSpecifier::_int}, BasicType::_long_int},
	{{TypeSpecifier::_long}, BasicType::_long_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_long}, BasicType::_long_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_long, TypeSpecifier::_int}, BasicType::_long_int},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_long}, BasicType::_unsigned_long},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_long, TypeSpecifier::_int}, BasicType::_unsigned_long},
	{{TypeSpecifier::_long, TypeSpecifier::_long}, BasicType::_long_long_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_long, TypeSpecifier::_long}, BasicType::_long_long_int},
	{{TypeSpecifier::_long, TypeSpecifier::_long, TypeSpecifier::_int}, BasicType::_long_long_int},
	{{TypeSpecifier::_signed, TypeSpecifier::_long, TypeSpecifier::_long, TypeSpecifier::_int }, BasicType::_long_long_int},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_long, TypeSpecifier::_long}, BasicType::_unsigned_long_long},
	{{TypeSpecifier::_unsigned, TypeSpecifier::_long, TypeSpecifier::_long, TypeSpecifier::_int}, BasicType::_unsigned_long_long},
	{{TypeSpecifier::_float}, BasicType::_float},
	{{TypeSpecifier::_double}, BasicType::_double},
	{{TypeSpecifier::_long, TypeSpecifier::_double}, BasicType::_long_double},
	{{TypeSpecifier::_Bool}, BasicType::_Bool},
	{{TypeSpecifier::_float, TypeSpecifier::_Complex}, BasicType::_float_Complex},
	{{TypeSpecifier::_double, TypeSpecifier::_Complex}, BasicType::_double_Complex},
	{{TypeSpecifier::_long, TypeSpecifier::_double, TypeSpecifier::_Complex}, BasicType::_long_double_Complex},
};

const map<string, TypeQualifier> type_qual_map =
{
	{"const", TypeQualifier::_const},
	{"restrict", TypeQualifier::_restrict_},
	{"volatile", TypeQualifier::_volatile},
	{"_Atomic", TypeQualifier::_atomic}
};


TypeNode::TypeNode(TypeCategory _category) :
	category(_category)
{}

BasicTypeNode::BasicTypeNode(BasicType basic_type):
	TypeNode(TypeCategory::_basic),
	type(basic_type)
{}

EnumTypeNode::EnumTypeNode(map<string, int> enum_values) :
	TypeNode(TypeCategory::_enum),
	values(enum_values)
{}

PointerTypeNode::PointerTypeNode(string ref_type_tag):
	TypeNode(TypeCategory::_pointer),
	named_ref_type(true),
	ref_type(NULL),
	ref_tag(ref_type_tag)
{}

PointerTypeNode::PointerTypeNode(TypeNode* ref_type_ptr) :
	TypeNode(TypeCategory::_pointer),
	named_ref_type(false),
	ref_type(ref_type_ptr),
	ref_tag("")
{}

