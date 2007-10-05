/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Generate C code
 *
 * Currently, the C code is generated directly from the AST; once we have an
 * IR, the C code will be generated from the IR instead.
 */

#ifndef GENERATE_C
#define GENERATE_C

#include <sstream>
#include "cmdline.h"
#include "HIR_visitor.h"
#include "process_ast/Pass_manager.h"

using namespace HIR;

class Generate_C : public HIR_visitor, public Pass
{
public:

	ostream& os;

	void run (AST::AST_php_script*, Pass_manager*);
	Generate_C(ostream&);

public:
	void children_statement(HIR_statement* in);
	void pre_php_script(HIR_php_script* in);
	void post_php_script(HIR_php_script* in);

public:
	String* extension_name;
	bool is_extension;
	List<HIR_signature*>* methods;	// List of all methods compiled	
	bool return_by_reference; 		// current methods returns by reference	
};

#endif // GENERATE_C
