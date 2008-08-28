/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Convenience debugging functions.
 */

#include "process_ir/debug.h"
#include "process_ir/XML_unparser.h"
#include "process_ast/AST_unparser.h"
#include "process_hir/HIR_unparser.h"
#include "process_mir/MIR_unparser.h"

/* Dump the XML for anynode to stderr. A global function. */
// these are split so they're easy to call from the debugger

void debug (AST::Node *in)
{
	CHECK_DEBUG ();
	static AST_unparser *pup = new AST_unparser (cdebug, true);
	pup->unparse (in);
	cdebug << endl;
}

void xdebug (AST::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, false);
}

void xadebug (AST::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, true);
}

void debug (HIR::Node *in)
{
	CHECK_DEBUG ();
	static HIR_unparser *pup = new HIR_unparser (cdebug, true);
	pup->unparse (in);
	cdebug << endl;
}

void xdebug (HIR::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, false);
}

void xadebug (HIR::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, true);
}


void debug (MIR::Node *in)
{
	CHECK_DEBUG ();
	static MIR_unparser *pup = new MIR_unparser (cdebug, true);
	pup->unparse (in);
	cdebug << endl;
}

void xdebug (MIR::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, false);
}

void xadebug (MIR::Node* in)
{
	CHECK_DEBUG ();
	xml_unparse (in, cdebug, true);
}

ostream& cdebug = cerr;

bool debugging_enabled = false;

void enable_cdebug ()
{
	debugging_enabled = true;
}

void disable_cdebug ()
{
	debugging_enabled = false;
}
