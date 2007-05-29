/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Check for invalid PHP, which otherwise fits within the constraints of
 * our language. This should be run after the parser so we dont operate
 * on invalid code, and any time we worry we may generate invalid code in
 * the HIR.
 */

#ifndef PHC_INVALID_CHECK
#define PHC_INVALID_CHECK

#include "AST_visitor.h"

class Invalid_check : public AST_visitor 
{
public:
	void pre_statement (AST_statement* in);
	void pre_assignment (AST_assignment* in);
	void pre_foreach (AST_foreach* in);
	void pre_interface_def (AST_interface_def* in);
	void pre_directive (AST_directive *in);
};

#endif // PHC_INVALID_CHECK
