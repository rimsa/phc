/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Move isset and unset into individual calls.
 */

#ifndef PHC_SPLIT_UNSET_ISSET_H
#define PHC_SPLIT_UNSET_ISSET_H

#include "Lower_expr.h"
#include "fresh.h"

class Split_unset_isset : public AST::Transform
{
public:
	// Split unset 
	void pre_eval_expr(AST::Eval_expr* in, List<AST::Statement*>* out);

	// Split isset
	AST::Expr* pre_method_invocation(AST::Method_invocation* in); 
};

#endif // PHC_SPLIT_UNSET_ISSET_H
