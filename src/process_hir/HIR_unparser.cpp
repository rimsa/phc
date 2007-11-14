/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Unparse the HIR back to PHP syntax. Makes an effort to adhere to the Zend
 * coding style guidelines at
 *
 *   http://framework.zend.com/manual/en/coding-standard.coding-style.html
 */

#include <iostream>
#include <iomanip> 
#include "HIR_unparser.h" 
#include "cmdline.h"

extern struct gengetopt_args_info args_info;

using namespace std;
using namespace HIR;

void debug (HIR_node *in)
{
	static HIR_unparser *pup = new HIR_unparser (cerr);
	in->visit (pup);
}

HIR_unparser::HIR_unparser (ostream& os) : PHP_unparser (os)
{
}

void HIR_unparser::children_php_script(HIR_php_script* in)
{
	echo_nl("<?php");
	if(!args_info.no_leading_tab_flag) inc_indent();

	// We don't want to output the { and }, so we manually traverse the list
	List<HIR_statement*>::const_iterator i;
	for(i = in->statements->begin(); i != in->statements->end(); i++)
		visit_statement(*i);
	
	if(!args_info.no_leading_tab_flag) dec_indent();
	echo_nl("?>");
}

void HIR_unparser::children_interface_def(HIR_interface_def* in)
{
	echo("interface ");
	visit_interface_name(in->interface_name);

	if(!in->extends->empty())
	{
		echo(" extends ");
		visit_interface_name_list(in->extends);
	}
	
	visit_member_list(in->members);
}

void HIR_unparser::children_class_def(HIR_class_def* in)
{
	visit_class_mod(in->class_mod);
	echo("class ");
	visit_class_name(in->class_name);

	if(in->extends != NULL)
	{
		echo(" extends ");
		visit_class_name(in->extends);
	}
	
	if(!in->implements->empty())
	{
		echo(" implements ");
		visit_interface_name_list(in->implements);
	}
	
	visit_member_list(in->members);
}

void HIR_unparser::children_class_mod(HIR_class_mod* in)
{
	if(in->is_abstract) echo("abstract ");
	if(in->is_final) echo("final ");
}

void HIR_unparser::children_method(HIR_method* in)
{
	visit_signature(in->signature);
	if(in->statements != NULL)
	{
		newline();
		visit_statement_list(in->statements);
		newline();
	}
	else
		// Abstract method
		echo_nl(";");
}

void HIR_unparser::children_signature(HIR_signature* in)
{
	visit_method_mod(in->method_mod);
	echo("function ");
	if(in->is_ref) echo("&");
	visit_method_name(in->method_name);
	visit_formal_parameter_list(in->formal_parameters);
}

void HIR_unparser::children_method_mod(HIR_method_mod* in)
{
	if(in->is_public)		echo("public ");
	if(in->is_protected)	echo("protected ");
	if(in->is_private)	echo("private ");
	if(in->is_static)		echo("static ");
	if(in->is_abstract)	echo("abstract ");
	if(in->is_final)		echo("final ");
}

void HIR_unparser::children_formal_parameter(HIR_formal_parameter* in)
{
	visit_type(in->type);
	if(in->is_ref) echo("&");
	visit_name_with_default(in->var);
}

void HIR_unparser::children_type(HIR_type* in)
{
	if(in->class_name != NULL)
	{
		visit_class_name(in->class_name);
		echo(" ");
	}
}

void HIR_unparser::children_attribute(HIR_attribute* in)
{
	visit_attr_mod(in->attr_mod);

	// Class attributes get a dollar sign, with the exception of const attributes
	if(!in->attr_mod->is_const) echo("$"); 
	visit_variable_name(in->var->variable_name);
	if(in->var->expr != NULL)
	{
		echo(" = ");
		visit_expr(in->var->expr);
	}

	echo(";");
	// newline is output by post_commented_node
}

void HIR_unparser::children_attr_mod(HIR_attr_mod* in)
{
	if(in->is_public)		echo("public ");
	if(in->is_protected)	echo("protected ");
	if(in->is_private)	echo("private ");
	if(in->is_static)		echo("static ");
	if(in->is_const)		echo("const ");

	if(!in->is_public && 
		!in->is_protected && 
		!in->is_private && 
		!in->is_static && 
		!in->is_const)
			echo("var ");
}


/* This is simpler than the other if, since there's no user-written code to
 * maintain, and the statements can only be gotos */
void HIR_unparser::children_branch(HIR_branch* in)
{
	echo("if (");
	bool in_if_expression = true;
	visit_expr(in->expr);
	in_if_expression = false;
	echo(") goto ");
	visit_label_name (in->iftrue);
	echo (" else goto ");
	visit_label_name (in->iffalse);
	echo (";");

	newline();
}

void HIR_unparser::children_return(HIR_return* in)
{
	echo("return");
	if(in->expr != NULL)
	{
		echo(" ");
		visit_expr(in->expr);
	}
	echo(";");
	// newline output by post_commented_node
}

void HIR_unparser::children_static_declaration(HIR_static_declaration* in)
{
	echo("static ");
	visit_name_with_default(in->var);
	echo(";");
	// newline output by post_commented_node
}

void HIR_unparser::children_global(HIR_global* in)
{
	echo("global $");
	visit_variable_name(in->variable_name);
	echo(";");
	// newline output by post_commented_node
}

void HIR_unparser::children_try(HIR_try* in)
{
	echo("try");
	space_or_newline();

	visit_statement_list(in->statements);
	space_or_newline();

	visit_catch_list(in->catches);
}

void HIR_unparser::children_catch(HIR_catch* in)
{
	echo("catch (");
	visit_class_name(in->class_name);
	echo(" $");
	visit_variable_name(in->variable_name);
	echo(")");
	space_or_newline();

	visit_statement_list(in->statements);
}

// We override post_catch_chain to avoid post_commented_node adding a newline
// after every catch (which messes up the layout when using same line curlies)
// We cannot deal with after-comments here, so we just assert that they don't
// exist and wait until somebody complains :)
void HIR_unparser::post_catch_chain(HIR_catch* in)
{
	List<String*>::const_iterator i;
	List<String*>* comments = in->get_comments();
	
	for(i = comments->begin(); i != comments->end(); i++)
	{
		assert(!((*i)->attrs->is_true("phc.unparser.comment.after")));
	}
}

void HIR_unparser::children_throw(HIR_throw* in)
{
	echo("throw ");
	visit_expr(in->expr);
	echo(";");
	// newline output by post_commented_node
}

void HIR_unparser::children_eval_expr(HIR_eval_expr* in)
{
	visit_expr(in->expr);
	echo(";");
	// The newline gets added by post_commented_node
}

void HIR_unparser::children_assignment(HIR_assignment* in)
{
	visit_variable(in->variable);

	if(in->is_ref)
		echo(" =& ");
	else
		echo(" = ");

	visit_expr(in->expr);
}

void HIR_unparser::children_cast(HIR_cast* in)
{
	echo("(");
	visit_cast(in->cast);
	echo(") ");
	visit_variable_name(in->variable_name);
}

void HIR_unparser::children_unary_op(HIR_unary_op* in)
{
	visit_op(in->op);
	visit_variable_name(in->variable_name);
}

void HIR_unparser::children_bin_op(HIR_bin_op* in)
{
	visit_variable_name(in->left);
	if(*in->op->value != ",") echo(" "); // We output "3 + 5", but "3, 5"
	visit_op(in->op);
	echo(" ");
	visit_variable_name(in->right);
}

void HIR_unparser::children_constant(HIR_constant* in)
{
	if(in->class_name != NULL)
	{
		visit_class_name(in->class_name);
		echo("::");
	}

	visit_constant_name(in->constant_name);
}

void HIR_unparser::children_instanceof(HIR_instanceof* in)
{
	visit_variable_name(in->variable_name);
	echo(" instanceof ");
	visit_class_name(in->class_name);
}

void HIR_unparser::children_variable(HIR_variable* in)
{
	HIR_reflection* reflection;

	if(in->target != NULL)
	{
		Token_class_name* class_name = dynamic_cast<Token_class_name*>(in->target);

		if(class_name)
		{
			visit_class_name(class_name);
			echo("::$");
		}
		else
		{
			visit_target(in->target);
			echo("->");
		}
	}
	else
	{
		echo("$");
	}

	reflection = dynamic_cast<HIR_reflection*>(in->variable_name);
	
	if(reflection)
	{
//		TODO this doesnt do anything. Bug?
//		name = dynamic_cast<HIR_variable*>(reflection->expr);
		visit_variable_name(in->variable_name);
	}
	else
	{
		visit_variable_name(in->variable_name);
	}

	List<HIR_expr*>::const_iterator i;
	for(i = in->array_indices->begin(); i != in->array_indices->end(); i++)
	{
		if(*i && (*i)->attrs->is_true("phc.unparser.index_curlies"))
		{
			echo("{");
			if(*i) visit_expr(*i);
			echo("}");
		}
		else
		{
			echo("[");
			if(*i) visit_expr(*i);
			echo("]");
		}
	}
}

void HIR_unparser::children_reflection(HIR_reflection* in)
{
	visit_variable_name (in->variable_name);
}

void HIR_unparser::children_pre_op(HIR_pre_op* in)
{
	visit_op(in->op);
	visit_variable(in->variable);
}

void HIR_unparser::children_array(HIR_array* in)
{
	echo("array");
	visit_array_elem_list(in->array_elems);
}

void HIR_unparser::children_array_elem(HIR_array_elem* in)
{
	if(in->key != NULL)
	{
		visit_expr(in->key);
		echo(" => ");
	}
	if(in->is_ref) echo("&");
	visit_expr(in->val);
}

void HIR_unparser::children_method_invocation(HIR_method_invocation* in)
{
	bool after_arrow = false;
	bool use_brackets = true;
	Token_class_name* static_method;
	Token_method_name* method_name;

	static_method = dynamic_cast<Token_class_name*>(in->target);
	if(static_method)
	{
		visit_class_name(static_method);
		echo("::");
	}
	else if(in->target != NULL)
	{
		visit_target(in->target);
		echo("->");
		after_arrow = true;
	}

	// Leave out brackets in calls in builtins
	method_name = dynamic_cast<Token_method_name*>(in->method_name);
	if(method_name)
	{
		use_brackets &= *method_name->value != "echo";
		use_brackets &= *method_name->value != "print";
		use_brackets &= *method_name->value != "include";
		use_brackets &= *method_name->value != "include_once";
		use_brackets &= *method_name->value != "require";
		use_brackets &= *method_name->value != "require_once";
		use_brackets &= *method_name->value != "use";
		use_brackets &= *method_name->value != "clone";
	}

	if(!use_brackets)
	{
		visit_method_name(in->method_name);
		echo(" ");	
		visit_actual_parameter_list(in->actual_parameters);
	}
	else
	{
		visit_method_name(in->method_name);

		echo("(");
		visit_actual_parameter_list(in->actual_parameters);
		echo(")");
	}
}

void HIR_unparser::children_actual_parameter(HIR_actual_parameter* in)
{
	if(in->is_ref) echo("&");
	visit_expr(in->expr);
}

void HIR_unparser::children_new(HIR_new* in)
{
	echo("new ");
	visit_class_name(in->class_name);
	echo("(");
	visit_actual_parameter_list(in->actual_parameters);
	echo(")");
}

void HIR_unparser::visit_interface_name_list(List<Token_interface_name*>* in)
{
	List<Token_interface_name*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) echo(", ");
		visit_interface_name(*i);
	}
}

void HIR_unparser::visit_member_list(List<HIR_member*>* in)
{
	newline();
	echo_nl("{");
	inc_indent();

	HIR_visitor::visit_member_list(in);	

	dec_indent();
	echo_nl("}");
}

void HIR_unparser::visit_statement_list(List<HIR_statement*>* in)
{
	echo("{");
	inc_indent();

	HIR_visitor::visit_statement_list(in);

	dec_indent();
	echo("}");
}

void HIR_unparser::visit_formal_parameter_list(List<HIR_formal_parameter*>* in)
{
	echo("(");
	List<HIR_formal_parameter*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) echo(", ");
		visit_formal_parameter(*i);
	}
	echo(")");
}

void HIR_unparser::visit_catch_list(List<HIR_catch*>* in)
{
	List<HIR_catch*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) space_or_newline();
		visit_catch(*i);
	}
}

void HIR_unparser::visit_expr_list(List<HIR_expr*>* in)
{
	List<HIR_expr*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
		if(*i) visit_expr(*i);
}

void HIR_unparser::visit_array_elem_list(List<HIR_array_elem*>* in)
{
	echo("(");

	List<HIR_array_elem*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) echo(", ");
		visit_array_elem(*i);
	}

	echo(")");
}

void HIR_unparser::visit_actual_parameter_list(List<HIR_actual_parameter*>* in)
{
	List<HIR_actual_parameter*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) echo(", ");
		visit_actual_parameter(*i);
	}
}

void HIR_unparser::visit_name_with_default_list(List<HIR_name_with_default*>* in)
{
	List<HIR_name_with_default*>::const_iterator i;
	for(i = in->begin(); i != in->end(); i++)
	{
		if(i != in->begin()) echo(", ");
		visit_name_with_default(*i);
	}
}

// Token classes
void HIR_unparser::children_interface_name(Token_interface_name* in)
{
	echo(in->value);
}

void HIR_unparser::children_class_name(Token_class_name* in)
{
	echo(in->value);
}

void HIR_unparser::children_method_name(Token_method_name* in)
{
	echo(in->value);
}

void HIR_unparser::children_variable_name(Token_variable_name* in)
{
	echo(in->value);
}

void HIR_unparser::children_cast(Token_cast* in)
{
	echo(in->value);
}

void HIR_unparser::children_op(Token_op* in)
{
	echo(in->value);
}

void HIR_unparser::children_constant_name(Token_constant_name* in)
{
	echo(in->value);
}

void HIR_unparser::children_int(Token_int* in)
{
	echo(in->source_rep);
}

void HIR_unparser::children_real(Token_real* in)
{
	echo(in->source_rep);
}

void HIR_unparser::children_string(Token_string* in)
{
	if(
			*in->source_rep == "__FILE__" ||
			*in->source_rep == "__CLASS__" ||
			*in->source_rep == "__METHOD__" ||
			*in->source_rep == "__FUNCTION__"
	  )
	{
		echo(in->source_rep);
	}
	else
	{
    // any escaping must already be present in source_rep; any strings that
    // originate from the user will be escaped as the user escaped them,
    // and any strings that originate from passes within the compiler should
    // have been escaped by those passes
		
    if(in->attrs->is_true("phc.unparser.is_singly_quoted"))
		{
			echo("'");
			echo(in->source_rep);
			echo("'");
		}
		else 
		{
      echo("\"");
			echo(in->source_rep);
			echo("\"");
		}
	}
}

void HIR_unparser::children_bool(Token_bool* in)
{
	echo(in->source_rep);
}

void HIR_unparser::children_null(Token_null* in)
{
	echo(in->source_rep);
}

// Generic classes
void HIR_unparser::pre_node(HIR_node* in)
{
	if(in->attrs->is_true("phc.unparser.starts_line") && !at_start_of_line)
	{
		newline();
		echo(args_info.tab_arg);
	}
}

/* There are a number of places where curlies are required. All of these
 * involves reflection, and some special cases in the node containing the
 * reflected variable. In general $$$$x (ie any amunt of nested variabels) is
 * ok, and anything else requires curlies. */
bool needs_curlies (HIR_reflection* in)
{
	return false;
}

void HIR_unparser::pre_variable (HIR_variable* in)
{
	HIR_reflection* reflect = dynamic_cast<HIR_reflection*>(in->variable_name);
	if (reflect && 
			(needs_curlies (reflect) || in->array_indices->size () > 0))
		assert (0); // this shouldnt be possible anymore
}

void HIR_unparser::pre_method_invocation (HIR_method_invocation* in)
{
	HIR_reflection* reflect = dynamic_cast<HIR_reflection*>(in->method_name);
	if (in->target
			&& reflect && needs_curlies (reflect))
		assert (0); // this shouldnt be possible anymore
}

void HIR_unparser::pre_global (HIR_global* in)
{
	HIR_reflection* reflect = dynamic_cast<HIR_reflection*>(in->variable_name);
	if (reflect && needs_curlies (reflect))
		assert (0); // this shouldnt be possible anymore
}

void HIR_unparser::pre_expr(HIR_expr* in)
{
	if(in->attrs->is_true("phc.unparser.needs_user_brackets"))
		echo("(");
	if(in->attrs->is_true("phc.unparser.needs_curlies") or in->attrs->is_true("phc.unparser.needs_user_curlies"))
		echo("{");
}

void HIR_unparser::post_expr(HIR_expr* in)
{
	if(in->attrs->is_true("phc.unparser.needs_curlies") or in->attrs->is_true("phc.unparser.needs_user_curlies"))
		echo("}");
	if(in->attrs->is_true("phc.unparser.needs_user_brackets"))
		echo(")");
}

void HIR_unparser::children_label_name (Token_label_name* in)
{
	echo(in->value);
}

void 
HIR_unparser::children_goto (HIR_goto* in)
{
	echo ("goto ");
	visit_label_name (in->label_name);
	echo_nl (";");
}

void 
HIR_unparser::children_label (HIR_label* in)
{
	visit_label_name (in->label_name);
	echo_nl(":");
}

void HIR_unparser::children_name_with_default (HIR_name_with_default* in)
{
	echo("$");
	visit_variable_name(in->variable_name);
	if(in->expr != NULL)
	{
		echo(" = ");
		visit_expr(in->expr);
	}
}
