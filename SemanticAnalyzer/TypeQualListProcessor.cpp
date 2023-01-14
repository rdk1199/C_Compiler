#include "TypeQualListProcessor.h"

using std::set;
using std::string;

set<TypeQualifier> TypeQualListProcessor::process_type_qualifier_list(ParseTreeNode* type_qual_list_node)
{
	set<TypeQualifier> quals = {};

	return get_type_qualifiers(type_qual_list_node, quals);
}

set<TypeQualifier> TypeQualListProcessor::get_type_qualifiers(ParseTreeNode* type_qual_list_node, std::set<TypeQualifier>& qual_set)
{
	string type_qual_lexeme = type_qual_list_node->children.back()->children[0]->lexeme;

	qual_set.insert(type_qual_map.at(type_qual_lexeme));

	if (type_qual_list_node->children.size() == 2)
	{
		get_type_qualifiers(type_qual_list_node->children[0], qual_set);

		return qual_set;
	}

	else
	{
		return qual_set;
	}
}