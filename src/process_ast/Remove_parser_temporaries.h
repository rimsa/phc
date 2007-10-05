/* 
 * phc -- the open source PHP compiler 
 * See doc/license/README.license for licensing information
 *
 * The parser a number of attributes to AST nodes that it needs to be
 * able to build the tree. These attributes are no longer necessary 
 * when parsing has completed, so this transform removes them.
 */

#ifndef PHC_REMOVE_PARSER_TEMPORARIES
#define PHC_REMOVE_PARSER_TEMPORARIES

#include "AST_visitor.h"

using namespace AST;

class Remove_parser_temporaries : public AST_visitor
{
public:
	void pre_node(AST_node* in);
};

#endif // PHC_REMOVE_PARSER_TEMPORARIES
