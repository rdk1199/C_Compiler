#pragma once

#include "C_Compiler/ParseTree.h"
#include "TypeSpecProcessor.h"
#include "ConstantExpressionEvaluator.h"

struct DeclarationSpecifierInfo;

class DeclarationProcessor
{

private: 


	static void process_declaration_specifiers(ParseTreeNode* declaration_specifiers_node, DeclarationSpecifierInfo& out_info);
	static StorageClassSpecifier process_storage_class_specifier(ParseTreeNode* storage_class_specifier_node);
	static TypeQualifier process_type_qualifier(ParseTreeNode* type_qualifier_node);
	static FunctionSpecifier process_function_specifier(ParseTreeNode* function_specifier_node);
	static int process_alignment_specifier(ParseTreeNode* alignment_specifier_node);

	static bool check_dec_specs(const DeclarationSpecifierInfo& dec_spec_info);

	static TypeSpecifier process_type_specifier(ParseTreeNode* type_specifier_node);

	static void process_static_assert_declaration(ParseTreeNode* static_assert_declaration_node);


public: 
	static std::set<Symbol> process_declaration(ParseTreeNode* declaration_node, SymbolTable& symbol_table);

};

struct DeclarationSpecifierInfo
{
	std::set<StorageClassSpecifier> storage_class_specifiers;
	std::set<TypeQualifier> type_qualifiers;
	std::set<FunctionSpecifier> function_specifiers;
	std::multiset<TypeSpecifier> type_specifiers;
	ParseTreeNode* type_specifier_node = NULL; //for further examination of struct, union, enum specs

	int alignment = 0;
};
