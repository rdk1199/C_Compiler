

#include "DeclarationProcessor.h"
#include "C_Compiler/C_Grammar_Macros.h"
#include <iostream>
#include "SemanticAnalyzerError.h"

using std::multiset;
using std::cout;
using std::endl;
using std::set;
using std::vector;
using std::map;



set<Symbol> DeclarationProcessor::process_declaration(ParseTreeNode* declaration_node, SymbolTable& symbol_table)
{
	if (declaration_node->symbol != __DECLARATION__)
	{
		throw InternalCallError();
	}

	else if (declaration_node->children.size() == 0)
	{
		throw InvalidParseTree();
	}

	if (declaration_node->children[0]->symbol == __STATIC_ASSERT_DECLARATION__)
	{
		process_static_assert_declaration(declaration_node->children[0]);
		return {};
	}

	DeclarationSpecifierInfo dec_spec_info;

	if (declaration_node->children[0]->symbol == __DECLARATION_SPECIFIERS__)
	{
		process_declaration_specifiers(declaration_node->children[0], dec_spec_info);
	}

	else if (declaration_node->children[0]->symbol == __DECLARATION_PREFIX__)
	{
		process_declaration_specifiers(declaration_node->children[0]->children[0], dec_spec_info);
	}

	else
	{
		throw InvalidParseTree();
	}
}

void DeclarationProcessor::process_declaration_specifiers(ParseTreeNode* declaration_specifiers_node, DeclarationSpecifierInfo& out_info)
{
	if (declaration_specifiers_node->symbol != __DECLARATION_SPECIFIERS__)
	{
		throw InternalCallError();
	}

	else if (declaration_specifiers_node->children.size() == 0)
	{
		throw InvalidParseTree();
	}

	ParseTreeNode* current_child = declaration_specifiers_node->children[0];

	switch (current_child->symbol)
	{
	case(__STORAGE_CLASS_SPECIFIER__):

		out_info.storage_class_specifiers.insert(process_storage_class_specifier(current_child));

		break;

	case(__TYPE_SPECIFIER__):

		out_info.type_specifiers.insert(process_type_specifier(current_child));
		out_info.type_specifier_node = current_child;

		break;

	case(__TYPE_QUALIFIER__):

		out_info.type_qualifiers.insert(process_type_qualifier(current_child));

		break;

	case(__FUNCTION_SPECIFIER__):

		out_info.function_specifiers.insert(process_function_specifier(current_child));

		break;

	case(__ALIGNMENT_SPECIFIER__):

		out_info.alignment = process_alignment_specifier(current_child);

		break;

	default:
		throw InvalidParseTree();
		break;
	}

	if (declaration_specifiers_node->children.size() == 2)
	{
		process_declaration_specifiers(declaration_specifiers_node->children[1], out_info);
	}

}

StorageClassSpecifier DeclarationProcessor::process_storage_class_specifier(ParseTreeNode* storage_class_specifier_node)
{
	if (storage_class_specifier_node->symbol != __STORAGE_CLASS_SPECIFIER__)
	{
		throw InternalCallError();
	}

	else if (storage_class_specifier_node->children.size() != 1)
	{
		throw InvalidParseTree();
	}

	switch (storage_class_specifier_node->children[0]->symbol)
	{
	case(__EXTERN__):
		return StorageClassSpecifier::_extern;
		break;
	case(__STATIC__):
		return StorageClassSpecifier::_static;
		break;
	case(__THREAD_LOCAL__):
		return StorageClassSpecifier::_thread_local;
		break;
	case(__AUTO__):
		return StorageClassSpecifier::_auto;
		break;
	case(__REGISTER__):
		return StorageClassSpecifier::_register;
		break;
	case(__TYPEDEF__):
		return StorageClassSpecifier::_typedef;
		break;
	default:
		throw InvalidParseTree();
		return StorageClassSpecifier::_typedef; //error -> never returns
		break;
	}
}

TypeQualifier DeclarationProcessor::process_type_qualifier(ParseTreeNode* type_qualifier_node)
{
	if (type_qualifier_node->symbol != __TYPE_QUALIFIER__)
	{
		throw InternalCallError();
	}

	else if (type_qualifier_node->children.size() != 1)
	{
		throw InvalidParseTree();
	}

	switch (type_qualifier_node->children[0]->symbol)
	{
	case(__CONST__):
		return TypeQualifier::_const;
		break;
	case(__RESTRICT__):
		return TypeQualifier::_restrict_;
		break;
	case(__VOLATILE__):
		return TypeQualifier::_volatile;
		break;
	case(__ATOMIC__):
		return TypeQualifier::_atomic;
		break;
	default:
		throw InvalidParseTree();
		return TypeQualifier::_const; //this never executes
		break;
	}

}

FunctionSpecifier DeclarationProcessor::process_function_specifier(ParseTreeNode* function_specifier_node)
{
	if (function_specifier_node->symbol != __FUNCTION_SPECIFIER__)
	{
		throw InternalCallError();
	}

	else if (function_specifier_node->children.size() != 1)
	{
		throw InvalidParseTree();
	}

	switch (function_specifier_node->children[0]->symbol)
	{
	case(__INLINE__):
		return FunctionSpecifier::_in_line;
		break;
	case(__NO_RETURN__):
		return FunctionSpecifier::_no_return;
		break;
	default:
		throw InvalidParseTree();
		return FunctionSpecifier::_in_line; //error -> never returns
		break;
	}
}

TypeSpecifier DeclarationProcessor::process_type_specifier(ParseTreeNode* type_specifier_node)
{
	if (type_specifier_node->symbol != __TYPE_SPECIFIER__)
	{
		throw InternalCallError();
	}

	else if (type_specifier_node->children.size() != 1)
	{
		throw InvalidParseTree();
	}

	switch (type_specifier_node->symbol)
	{
	case(__VOID__):
		return TypeSpecifier::_void;
		break;
	case(__CHAR__):
		return TypeSpecifier::_char;
		break;
	case(__SHORT__):
		return TypeSpecifier::_short;
		break;
	case(__INT__):
		return TypeSpecifier::_int;
		break;
	case(__LONG__):
		return TypeSpecifier::_long;
		break;
	case(__FLOAT__):
		return TypeSpecifier::_float;
		break;
	case(__DOUBLE__):
		return TypeSpecifier::_double;
		break;
	case(__SIGNED__):
		return TypeSpecifier::_signed;
		break;
	case(__UNSIGNED__):
		return TypeSpecifier::_unsigned;
		break;
	case(__BOOL__):
		return TypeSpecifier::_Bool;
		break;
	case(__COMPLEX__):
		return TypeSpecifier::_Complex;
		break;
	case(__ATOMIC_TYPE_SPECIFIER__):
		return TypeSpecifier::_atomic;
		break;

	case(__STRUCT_OR_UNION_SPECIFIER__):

		if (type_specifier_node->children.size() == 1 && type_specifier_node->children[0]->symbol == __STRUCT__)
		{
			return TypeSpecifier::_struct;
		}

		else if (type_specifier_node->children.size() == 1 && type_specifier_node->children[0]->symbol == __UNION__)
		{
			return TypeSpecifier::_union;
		}

		else
		{
			throw InvalidParseTree();
		}
		break;

	case(__ENUM_SPECIFIER__):
		return TypeSpecifier::_enum;
		break;

	case(__TYPEDEF_NAME__):
		return TypeSpecifier::_typedef_name;
		break;

	default:
		throw InvalidParseTree();
		break;
	}
}

void DeclarationProcessor::process_static_assert_declaration(ParseTreeNode* static_assert_declaration_node)
{
	if (static_assert_declaration_node->symbol != __STATIC_ASSERT_DECLARATION__)
	{
		throw InternalCallError();
	}

	ParseTreeNode* constant_expression_node = static_assert_declaration_node->children[2];
	ParseTreeNode* string_literal_node = static_assert_declaration_node->children[4];

	int expression_value = ConstantExpressionEvaluator::evaluate_integer_constant_expression(constant_expression_node);

	if (expression_value == 0)
	{
		cout << string_literal_node->lexeme;
		exit(-1);
	}
}



