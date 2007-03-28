/* 
 * phc -- the open source PHP compiler 
 * See doc/license/README.license for licensing information
 *
 * Replaces an include statement with the parsed contents of the included file.
 * 
 */

#include <libgen.h> // for dirname
#include <iostream>
#include <fstream>
#include "Process_includes.h"
#include "AST_visitor.h"
#include "AST.h"
#include "lib/error.h"
#include "parsing/parse.h"
#include "process_ast/PHP_unparser.h"
#include "cmdline.h"

extern struct gengetopt_args_info args_info;

/*
 * The include functionality is described in http://php.net/include/. 
 *
 * We support:
 * - include statements whose parameter must be a literal string
 * - the included file being in the current directory or the directory
 *	  containing the file being parsed
 * 
 * 
 * We don't support:
 * - return statements in the included file
 * - the '$val = include('my_file.php');' construct
 * - get_included_files()
 * - proper error messages. If it fails, we leave in the include statement
 *
 *
 * We intend to support in the future (but probably in the IR):
 * - include statements whose parameter can be derived using constant
 *   propegation or other dataflow analysis
 * - Run-time inclusion
 * - Remote compile-time or run-time inclusion
 * - return statements and the complications they entail
 */

/* Any variables available at that line in the calling file will be
 * available within the called file, from that point forward. */
// We put them in the current function

/* However, all functions and classes defined in the included file have
 * the global scope. */
// we put them in %MAIN% 

/* If the include occurs inside a function within the calling file, then all of
 * the code contained in the called file will behave as though it had been
 * defined inside that function. So, it will follow the variable scope of that
 * function. */
// Done: we put them in the current function

/* Because include() and require()  are special language constructs, you
 * must enclose them within a statement block if it's inside a conditional
 * block. */
// Done: This is handled automatically by the parser

/* $bar is the value 1 because the include was successful. Notice the
 * difference between the above examples. The first uses return() within the
 * included file while the other does not. If the file can't be included, FALSE
 * is returned and E_WARNING is issued. */
// Not done: we dont support this

/* Handling Returns: It is possible to execute a return()  statement
 * inside an included file in order to terminate processing in that file and
 * return to the script which called it. Also, it's possible to return values
 * from included files. You can take the value of the include call as you would
 * a normal function. This is not, however, possible when including remote
 * files unless the output of the remote file has valid PHP start and end tags
 * (as with any local file). You can declare the needed variables within those
 * tags and they will be introduced at whichever point the file was included.
 * */
// Not done: We'll handle returns in the IR, since the IR has goto, which these
// really need. Also, contructs like 
// '$a = do_something(do_something_else(include('myfile.php')))' are easy to
// support in the IR and nasty to support in the AST

/* If there are functions defined in the included file, they can be used in the
 * main file independent if they are before return() or after. If the file is
 * included twice, PHP 5 issues fatal error because functions were already
 * declared, while PHP 4 doesn't complain about functions defined after
 * return(). It is recommended to use include_once() instead of checking if the
 * file was already included and conditionally return inside the included file.
 * */
// Done: redefined functions signal an error

// With the _once, if there is an include_ or require_once, and there is an
// include anywhere which we cant include, fail all includes and issue a
// warning.
// Not done: just leaving this out. Include_once at your peril

// We could update get_included_files(), (say, by appending it with the file nmae of the included file), but that's a bit difficult to get right.
// Not done, a bit tricky

class Return_check : public AST_visitor
{
public:
	bool found;
	int line_number;

	Return_check()
	{
		found = false;
	}

	void pre_return(AST_return* in)
	{	
		if (not found)
		{
			found = true;
			line_number = in->get_line_number();
		}
	}
};

class Set_variable_targets_to_null : public AST_visitor
{
public:

	void pre_variable(AST_variable* in)
	{
		// When we include a file, the globals become locals, and no longer
		// should have %MAIN% as their target
		Token_class_name* main = new Token_class_name(new String("%MAIN%"));
		if (in->target and in->target->match(main))
		{
			in->target = NULL;
		}
	}
};

// store the current php script
AST_php_script* Process_includes::pre_php_script(AST_php_script* in)
{
	current_script = in;
	return in;
}

// store the method currently being defined
void Process_includes::pre_method(AST_method* in, List<AST_member*>* out)
{
	in_main_run = (in == current_script->get_class_def("%MAIN%")->get_method("%run%"));
	out->push_back(in);
}

// look for include statements
void Process_includes::pre_eval_expr(AST_eval_expr* in, List<AST_statement*>* out)
{
	AST_method_invocation* pattern;
	Token_string* token_filename = new Token_string(NULL, NULL);
	Token_method_name* method_name = new Token_method_name(NULL);

	// the filename is the only parameter of the include statement
	pattern = new AST_method_invocation(
		new Token_class_name(new String("%STDLIB%")),
		method_name,
		new List<AST_actual_parameter*>(
			new AST_actual_parameter(false, token_filename)
		)
	);

	// check we have a matching function
	if	((in->expr->match(pattern)) and
			(*method_name->value == "include" or *method_name->value == "require"))
	{
		String* filename = token_filename->value;

		// Set up search directory
		List<String*>* dirs = new List<String*>();

		// If the included file starts with "./" or "../", use empty search path
		// Otherwise, pass in dirname(filename) as the search path
		// TODO: this should include the actual include path at some stage
		if(filename->substr(0, 2) != "./" && filename->substr(0, 3) != "../")
		{
			char* directory_name = strdup(in->get_filename()->c_str());
			directory_name = dirname(directory_name);
			dirs->push_back(new String(directory_name));
		}

		// Try to parse the file
		AST_php_script* php_script = parse(filename, dirs, false);
		if(php_script == NULL)
		{
			// Script could not be found or not be parsed; leave the include in
			phc_warning(WARNING_INCLUDE_FAILED, in->get_filename(), in->get_line_number(), filename->c_str());
			out->push_back(in);
			return;
		}
			
		// Right now we deal with the statements to be returned.
		AST_class_def* main = php_script->get_class_def("%MAIN%");
		AST_method* run = main->get_method("%run%");

		// We don't support returning values from included scripts; 
		// issue a warning and leave the include as-is
		Return_check rc;
		rc.visit_statement_list(run->statements);
		if(rc.found)
		{
			phc_warning(WARNING_INCLUDE_RETURN, in->get_filename(), in->get_line_number(), filename->c_str());
			out->push_back(in);
			return;
		}

		// clear the target if statements arent being output to main::run
		if (not in_main_run) 
		{
			Set_variable_targets_to_null svttn;
			svttn.visit_statement_list(run->statements);
		}

		// copy the statements from %MAIN%::%run%
		out->push_back_all(run->statements);

		// copy classes
		List<AST_class_def*>::const_iterator ci;
		for(ci = php_script->class_defs->begin(); ci != php_script->class_defs->end(); ci++)
		{
			if(*ci != main)
			{
				current_script->class_defs->push_back(*ci);
			}
		}

		// copy attributes and methods from %MAIN%
		List<AST_member*>::const_iterator mi;
		for(mi = main->members->begin(); mi != main->members->end(); mi++)
		{
			if(*mi != run)
			{
				bool is_method = (dynamic_cast<AST_method*>(*mi) != NULL);

				// attributes arent pushed out if the current method isnt %main%::%run%
				if (is_method or not in_main_run)
				{
					current_script->get_class_def("%MAIN%")->members->push_back(*mi);
				}
			}
		}
	}
	else
	{
		// It's not an include statement (or an unsupported version of the include,
		// e.g. one that assigns to a variable)


		// check if its an unsupported version of include
		AST_method_invocation *method = new AST_method_invocation(new Token_class_name(new String("%STDLIB%")), method_name, NULL);
		if	((in->expr->match(method)) and
				(*method_name->value == "include" or *method_name->value == "require"))
		{
			ostringstream os;
			PHP_unparser pup(os);
			pup.visit_actual_parameter_list(method->actual_parameters);
			phc_warning(WARNING_INCLUDE_FAILED, in->get_filename(), in->get_line_number(), os.str().c_str());
		}


		out->push_back(in);
	}
}
