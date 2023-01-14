#pragma once

//terminals
#define __IDENTIFIER__ (static_cast<int>(token::_identifier))
#define __TYPE_IDENTIFIER__ (static_cast<int>(token::_type_identifier))
#define __INT_CONST__ (static_cast<int>(token::_integer_constant))
#define __FP_CONST__ (static_cast<int>(token::_fp_constant))
#define __CHAR_CONST__ (static_cast<int>(token::_character_constant))
#define __STRING_LITERAL__ (static_cast<int>(token::_string_literal))
#define __LEFT_BRACKET__ (static_cast<int>(token::_left_bracket))
#define __RIGHT_BRACKET__ (static_cast<int>(token::_right_bracket))
#define __LEFT_PAREN__ (static_cast<int>(token::_left_paren))
#define __RIGHT_PAREN__ (static_cast<int>(token::_right_paren))
#define __LEFT_BRACE__ (static_cast<int>(token::_left_brace))
#define __RIGHT_BRACE__ (static_cast<int>(token::_right_brace))
#define __PERIOD__ (static_cast<int>(token::_period))
#define __ARROW__ (static_cast<int>(token::_arrow))
#define __PLUS_PLUS__ (static_cast<int>(token::_plus_plus))
#define __MINUS_MINUS__ (static_cast<int>(token::_minus_minus))
#define __QUESTION_MARK__ (static_cast<int> (token::_question))
#define __COLON__ (static_cast<int>(token::_colon))
#define __OR__ (static_cast<int>(token::_bar_bar))
#define __AND_AND__ (static_cast<int>(token::_and_and))
#define __INCLUSIVE_OR__ (static_cast<int>(token::_bar))
#define __EXCLUSIVE_OR__ (static_cast<int>(token::_caret))
#define __AND__ (static_cast<int>(token::_and))
#define __EQUAL_EQUAL__ (static_cast<int> (token::_equal_equal))
#define __NOT_EQUAL__ (static_cast<int>(token::_not_equal))
#define __LESS_THAN__ (static_cast<int> (token::_less_than))
#define __GREATER_THAN__ (static_cast<int> (token::_greater_than))
#define __LEQ__ (static_cast<int> (token::_leq))
#define __GEQ__ (static_cast<int> (token::_geq))
#define __LEFT_SHIFT__ (static_cast<int> (token::_shift_left))
#define __RIGHT_SHIFT__ (static_cast<int> (token::_shift_right))
#define __PLUS__ (static_cast<int> (token::_plus))
#define __MINUS__ (static_cast<int> (token::_minus))
#define __ASTERISK__ (static_cast<int> (token::_asterisk))
#define __SLASH__ (static_cast<int>(token::_forward_slash))
#define __PERCENT__ (static_cast<int> (token::_percent))
#define __TILDE__ (static_cast<int> (token::_tilde))
#define __NOT__ (static_cast<int> (token::_exclamation))
#define __SEMICOLON__ (static_cast<int> (token::_semicolon))
#define __ELLIPSIS__ (static_cast<int> (token::_ellipsis))
#define __EQUAL__ (static_cast<int> (token::_equal))
#define __ASTERISK_EQUAL__ (static_cast<int> (token::_asterisk_equal))
#define __SLASH_EQUAL__ (static_cast<int> (token::_slash_equal))
#define __PERCENT_EQUAL__ (static_cast<int> (token::_percent_equal))
#define __PLUS_EQUAL__ (static_cast<int> (token::_plus_equal))
#define __MINUS_EQUAL__ (static_cast<int> (token::_minus_equal))
#define __SHIFT_LEFT_EQUAL__ (static_cast<int> (token::_shift_left_equal))
#define __SHIFT_RIGHT_EQUAL__ (static_cast<int> (token::_shift_right_equal))
#define __AND_EQUAL__ (static_cast<int> (token::_and_equal))
#define __CARET_EQUAL__ (static_cast<int> (token::_caret_equal))
#define __BAR_EQUAL__ (static_cast<int> (token::_bar_equal))
#define __COMMA__ (static_cast<int> (token::_comma))
#define __HASHTAG__ (static_cast<int> (token::_hashtag))
#define __HASHTAG_HASHTAG__ (static_cast<int> (token::_hashtag_hashtag))
#define __AUTO__ (static_cast<int> (token::_auto))
#define __BREAK__ (static_cast<int> (token::_break))
#define __CASE__ (static_cast<int> (token::_case))
#define __CHAR__ (static_cast<int> (token::_char))
#define __CONST__ (static_cast<int> (token::_const))
#define __CONTINUE__ (static_cast<int> (token::_continue))
#define __DEFAULT__ (static_cast<int> (token::_default))
#define __DO__ (static_cast<int> (token::_do))
#define __DOUBLE__ (static_cast<int> (token::_double))
#define __ELSE__ (static_cast<int> (token::_else))
#define __ENUM__ (static_cast<int> (token::_enum))
#define __EXTERN__ (static_cast<int> (token::_extern))
#define __FLOAT__ (static_cast<int> (token::_float))
#define __FOR__ (static_cast<int> (token::_for))
#define __GOTO__ (static_cast<int> (token::_goto))
#define __IF__ (static_cast<int> (token::_if))
#define __INLINE__ (static_cast<int> (token::_in_line))
#define __INT__ (static_cast<int> (token::_int))
#define __LONG__ (static_cast<int> (token::_long))
#define __REGISTER__ (static_cast<int> (token::_register))
#define __RESTRICT__ (static_cast<int> (token::_restrict_))
#define __RETURN__ (static_cast<int> (token::_return))
#define __SHORT__ (static_cast<int> (token::_short))
#define __SIGNED__ (static_cast<int> (token::_signed))
#define __SIZEOF__ (static_cast<int> (token::_sizeof))
#define __STATIC__ (static_cast<int> (token::_static))
#define __STRUCT__ (static_cast<int> (token::_struct))
#define __SWITCH__ (static_cast<int> (token::_switch))
#define __TYPEDEF__ (static_cast<int> (token::_typedef))
#define __UNION__ (static_cast<int> (token::_union))
#define __UNSIGNED__ (static_cast<int> (token::_unsigned))
#define __VOID__ (static_cast<int> (token::_void))
#define __VOLATILE__ (static_cast<int> (token::_volatile))
#define __WHILE__ (static_cast<int> (token::_while))
#define __ALIGN_AS__ (static_cast<int> (token::_align_as))
#define __ALIGN_OF__ (static_cast<int> (token::_align_of))
#define __ATOMIC__ (static_cast<int> (token::_atomic))
#define __BOOL__ (static_cast<int> (token::_bool))
#define __COMPLEX__ (static_cast<int> (token::_complex))
#define __GENERIC__ (static_cast<int> (token::_generic))
#define __IMAG__ (static_cast<int> (token::_imaginary))
#define __NO_RETURN__ (static_cast<int> (token::_no_return))
#define __STATIC_ASSERT__ (static_cast<int> (token::_static_assert))
#define __THREAD_LOCAL__ (static_cast<int> (token::_thread_local))
//#define __ENUM_IDENTIFIER__ (static_cast<int> (token::_enum_identifier))

#define __MAX__ __THREAD_LOCAL__ + 1 
//= 98


//nonterminals 

#define __PRIMARY_EXPRESSION__ __MAX__ + 1
#define __EXPRESSION__ __MAX__ + 2
#define __GENERIC_SELECTION__ __MAX__ + 3
#define __CONSTANT__ __MAX__ + 4
#define __GENERIC_SELECTION__ __MAX__ + 5
#define __ASSIGNMENT_EXPRESSION__ __MAX__ + 6
#define __GENERIC_ASSOC_LIST__ __MAX__ + 7
#define __GENERIC_ASSOCIATION__ __MAX__ + 8
#define __TYPE_NAME__ __MAX__ + 9
#define __POSTFIX_EXPRESSION__ __MAX__ + 10
#define __ARGUMENT_EXPRESSION_LIST__ __MAX__ + 11
#define __INITIALIZER_LIST__ __MAX__ + 12
#define __UNARY_EXPRESSION__ __MAX__ + 13
#define __UNARY_OPERATOR__ __MAX__ + 14
#define __CAST_EXPRESSION__ __MAX__ + 15
#define __MULTIPLICATIVE_EXPRESSION__ __MAX__ + 16
#define __ADDITIVE_EXPRESSION__ __MAX__ + 17
#define __SHIFT_EXPRESSION__ __MAX__ + 18
#define __CONDITIONAL_EXPRESSION__ __MAX__ + 19
#define __LOGICAL_OR_EXPRESSION__ __MAX__ + 20
#define __LOGICAL_AND_EXPRESSION__ __MAX__ + 21
#define __INCLUSIVE_OR_EXPRESSION__ __MAX__ + 22
#define __EXCLUSIVE_OR_EXPRESSION__ __MAX__ + 23
#define __AND_EXPRESSION__ __MAX__ + 24
#define __EQUALITY_EXPRESSION__ __MAX__ + 25
#define __RELATIONAL_EXPRESSION__ __MAX__ + 26
#define __ASSIGNMENT_OPERATOR__ __MAX__ + 27
#define __CONSTANT_EXPRESSION__ __MAX__ + 28
#define __DECLARATION__ __MAX__ + 29
#define __DECLARATION_SPECIFIERS__ __MAX__ + 30
#define __STATIC_ASSERT_DECLARATION__ __MAX__ + 31
#define __INIT_DECLARATOR_LIST__ __MAX__ + 32
#define __TYPE_SPECIFIER__ __MAX__ + 33
#define __TYPE_QUALIFIER__ __MAX__ + 34
#define __STORAGE_CLASS_SPECIFIER__ __MAX__ + 35
#define __FUNCTION_SPECIFIER__ __MAX__ + 36
#define __ALIGNMENT_SPECIFIER__ __MAX__ + 37
#define __INIT_DECLARATOR__ __MAX__ + 38
#define __DECLARATOR__ __MAX__ + 39
#define __INITIALIZER__ __MAX__ + 40
#define __ATOMIC_TYPE_SPECIFIER__ __MAX__ + 41
#define __STRUCT_OR_UNION_SPECIFIER__ __MAX__ + 42
#define __ENUM_SPECIFIER__ __MAX__ + 43
#define __TYPEDEF_NAME__ __MAX__ + 44
#define __STRUCT_DECLARATION_LIST__ __MAX__ + 45
#define __STRUCT_OR_UNION__ __MAX__ + 46
#define __STRUCT_DECLARATION__ __MAX__ + 47
#define __SPECIFIER_QUALIFIER_LIST__ __MAX__ + 48
#define __ENUMERATOR_LIST__ __MAX__ + 49
#define __STRUCT_DECLARATOR_LIST__ __MAX__ + 50
#define __STRUCT_DECLARATOR__ __MAX__ + 51
#define __ENUMERATOR__ __MAX__ + 52
//#define __ENUMERATION_CONSTANT__ __MAX__ + 53
#define __POINTER__ __MAX__ + 54
#define __DIRECT_DECLARATOR__ __MAX__ + 55 
#define __TYPE_QUALIFIER_LIST__ __MAX__ + 56
#define __PARAMETER_TYPE_LIST__ __MAX__ + 57
#define __IDENTIFIER_LIST__ __MAX__ + 58
#define __PARAMETER_LIST__ __MAX__ + 59
#define __PARAMETER_DECLARATION__ __MAX__ + 60
#define __ABSTRACT_DECLARATOR__ __MAX__ + 61
#define __DIRECT_ABSTRACT_DECLARATOR__ __MAX__ + 62
#define __DESIGNATION__ __MAX__ + 63
#define __DESIGNATOR_LIST__ __MAX__ + 64
#define __DESIGNATOR__ __MAX__ + 65
#define __STATEMENT__ __MAX__ + 66
#define __LABELED_STATEMENT__ __MAX__ + 67
#define __COMPOUND_STATEMENT__ __MAX__ + 68
#define __EXPRESSION_STATEMENT__ __MAX__ + 69
#define __SELECTION_STATEMENT__ __MAX__ + 70
#define __ITERATION_STATEMENT__ __MAX__ + 71
#define __JUMP_STATEMENT__ __MAX__ + 72
#define __BLOCK_ITEM_LIST__ __MAX__ + 73
#define __BLOCK_ITEM__ __MAX__ + 74
#define __TRANSLATION_UNIT__ __MAX__ + 75
#define __EXTERNAL_DECLARATION__ __MAX__ + 76
#define __FUNCTION_DEFINITION__ __MAX__ + 77
#define __DECLARATION_LIST__ __MAX__ + 78
#define __DECLARATION_PREFIX__ __MAX__ + 79
