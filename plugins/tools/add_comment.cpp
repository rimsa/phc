/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * After each statement, add a debug_zval_dump call to each variable in the statement.
 */

#include "pass_manager/Plugin_pass.h"

using namespace AST;

extern "C" void load (Pass_manager* pm, Plugin_pass* pass)
{
	pm->add_before_named_pass (pass, "ast");
}

extern "C" void run_ast (Node* in, Pass_manager* pm, String* option)
{
	String * comment = new String ("// ");
	comment->append (*option);
	// Add a comment to the first statement
	PHP_script *script = dynamic_cast <PHP_script*> (in);
	if (script->statements->size () == 0)
	{
		// create a statement for the comment
		Nop* nop = new Nop ();
		nop->get_comments()->push_front (comment);
	}
	else
	{
		// attach the comment to the first statement
		script->statements->front ()->get_comments ()->push_front (comment);
	}
}
