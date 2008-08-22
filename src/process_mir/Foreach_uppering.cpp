/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Convert Foreach statements to use ArrayIterators.
 */

#include "Foreach_uppering.h"
#include "process_ir/General.h"

using namespace MIR;

void Foreach_uppering::pre_foreach_reset (MIR::Foreach_reset* in, Statement_list* out)
{
	(*out
		<< "$" << in->iter << " = new ArrayObject ($" << in->array << ");\n"
		<< "$" << in->iter << " = $" << in->iter << "->getIterator();"
	).finish (in);
}

void Foreach_uppering::pre_foreach_next (MIR::Foreach_next* in, Statement_list* out)
{
	(*out
		<< "$" << in->iter << "->next ();\n"
	).finish (in);
}

void Foreach_uppering::pre_foreach_end (MIR::Foreach_end* in, Statement_list* out)
{
	// Do nothing (the garbage collector will clean this up)
}

Expr* Foreach_uppering::pre_foreach_has_key (MIR::Foreach_has_key* in)
{
	MIR::Statement_list out;
	(out
		<< "$" << in->iter << "->valid ();\n"
	).finish (in);

	// this gets shredded into $T = $iter->valid (), so find the assignment and extract the expr.
	return (dynamic_cast<Eval_expr*> (out.front()))->expr;
}

Expr* Foreach_uppering::pre_foreach_get_key (MIR::Foreach_get_key* in)
{
	if (in->attrs->is_true ("phc.unparser.use_get_key"))
	{
		NIL* nil = new NIL;
		nil->clone_mixin_from (in);
		return nil;
	}

	MIR::Statement_list out;
	(out
		<< "$" << in->iter << "->key ();\n"
	).finish (in);

	// this gets shredded into $iter->key (), so extract the expr.
	return (dynamic_cast<Eval_expr*> (out.front()))->expr;
}

Expr* Foreach_uppering::pre_foreach_get_val (MIR::Foreach_get_val* in)
{
	MIR::Statement_list out;
	(out
		<< "$" << in->iter << "->current ();\n"
	).finish (in);

	// this gets shredded into $iter->val() so extract the expr.
	return (dynamic_cast<Eval_expr*> (out.front()))->expr;
}
