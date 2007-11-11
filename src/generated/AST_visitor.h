#ifndef _AST_VISITOR_H_
#define _AST_VISITOR_H_

#include <iostream>
#include <sstream>
#include <iomanip>
#include "lib/error.h"
#include "lib/Object.h"
#include "lib/List.h"
#include "lib/String.h"
#include "lib/Boolean.h"
#include "lib/Integer.h"
#include "lib/AttrMap.h"
#include <list>
#include <string>
#include <assert.h>
using namespace std;


#include "AST.h"

namespace AST{
class AST_visitor
{
public:
    virtual ~AST_visitor();
// Invoked before the children are visited
public:
    virtual void pre_node(AST_node* in);
    virtual void pre_php_script(AST_php_script* in);
    virtual void pre_statement(AST_statement* in);
    virtual void pre_class_def(AST_class_def* in);
    virtual void pre_class_mod(AST_class_mod* in);
    virtual void pre_interface_def(AST_interface_def* in);
    virtual void pre_member(AST_member* in);
    virtual void pre_method(AST_method* in);
    virtual void pre_signature(AST_signature* in);
    virtual void pre_method_mod(AST_method_mod* in);
    virtual void pre_formal_parameter(AST_formal_parameter* in);
    virtual void pre_type(AST_type* in);
    virtual void pre_attribute(AST_attribute* in);
    virtual void pre_attr_mod(AST_attr_mod* in);
    virtual void pre_name_with_default(AST_name_with_default* in);
    virtual void pre_if(AST_if* in);
    virtual void pre_while(AST_while* in);
    virtual void pre_do(AST_do* in);
    virtual void pre_for(AST_for* in);
    virtual void pre_foreach(AST_foreach* in);
    virtual void pre_switch(AST_switch* in);
    virtual void pre_switch_case(AST_switch_case* in);
    virtual void pre_break(AST_break* in);
    virtual void pre_continue(AST_continue* in);
    virtual void pre_return(AST_return* in);
    virtual void pre_static_declaration(AST_static_declaration* in);
    virtual void pre_global(AST_global* in);
    virtual void pre_declare(AST_declare* in);
    virtual void pre_directive(AST_directive* in);
    virtual void pre_try(AST_try* in);
    virtual void pre_catch(AST_catch* in);
    virtual void pre_throw(AST_throw* in);
    virtual void pre_eval_expr(AST_eval_expr* in);
    virtual void pre_nop(AST_nop* in);
    virtual void pre_branch(AST_branch* in);
    virtual void pre_goto(AST_goto* in);
    virtual void pre_label(AST_label* in);
    virtual void pre_expr(AST_expr* in);
    virtual void pre_literal(AST_literal* in);
    virtual void pre_assignment(AST_assignment* in);
    virtual void pre_op_assignment(AST_op_assignment* in);
    virtual void pre_list_assignment(AST_list_assignment* in);
    virtual void pre_list_element(AST_list_element* in);
    virtual void pre_nested_list_elements(AST_nested_list_elements* in);
    virtual void pre_cast(AST_cast* in);
    virtual void pre_unary_op(AST_unary_op* in);
    virtual void pre_bin_op(AST_bin_op* in);
    virtual void pre_conditional_expr(AST_conditional_expr* in);
    virtual void pre_ignore_errors(AST_ignore_errors* in);
    virtual void pre_constant(AST_constant* in);
    virtual void pre_instanceof(AST_instanceof* in);
    virtual void pre_variable(AST_variable* in);
    virtual void pre_variable_name(AST_variable_name* in);
    virtual void pre_reflection(AST_reflection* in);
    virtual void pre_target(AST_target* in);
    virtual void pre_pre_op(AST_pre_op* in);
    virtual void pre_post_op(AST_post_op* in);
    virtual void pre_array(AST_array* in);
    virtual void pre_array_elem(AST_array_elem* in);
    virtual void pre_method_invocation(AST_method_invocation* in);
    virtual void pre_method_name(AST_method_name* in);
    virtual void pre_actual_parameter(AST_actual_parameter* in);
    virtual void pre_new(AST_new* in);
    virtual void pre_class_name(AST_class_name* in);
    virtual void pre_commented_node(AST_commented_node* in);
    virtual void pre_identifier(AST_identifier* in);
    virtual void pre_class_name(Token_class_name* in);
    virtual void pre_interface_name(Token_interface_name* in);
    virtual void pre_method_name(Token_method_name* in);
    virtual void pre_variable_name(Token_variable_name* in);
    virtual void pre_directive_name(Token_directive_name* in);
    virtual void pre_label_name(Token_label_name* in);
    virtual void pre_int(Token_int* in);
    virtual void pre_real(Token_real* in);
    virtual void pre_string(Token_string* in);
    virtual void pre_bool(Token_bool* in);
    virtual void pre_null(Token_null* in);
    virtual void pre_op(Token_op* in);
    virtual void pre_cast(Token_cast* in);
    virtual void pre_constant_name(Token_constant_name* in);
// Invoked after the children have been visited
public:
    virtual void post_node(AST_node* in);
    virtual void post_php_script(AST_php_script* in);
    virtual void post_statement(AST_statement* in);
    virtual void post_class_def(AST_class_def* in);
    virtual void post_class_mod(AST_class_mod* in);
    virtual void post_interface_def(AST_interface_def* in);
    virtual void post_member(AST_member* in);
    virtual void post_method(AST_method* in);
    virtual void post_signature(AST_signature* in);
    virtual void post_method_mod(AST_method_mod* in);
    virtual void post_formal_parameter(AST_formal_parameter* in);
    virtual void post_type(AST_type* in);
    virtual void post_attribute(AST_attribute* in);
    virtual void post_attr_mod(AST_attr_mod* in);
    virtual void post_name_with_default(AST_name_with_default* in);
    virtual void post_if(AST_if* in);
    virtual void post_while(AST_while* in);
    virtual void post_do(AST_do* in);
    virtual void post_for(AST_for* in);
    virtual void post_foreach(AST_foreach* in);
    virtual void post_switch(AST_switch* in);
    virtual void post_switch_case(AST_switch_case* in);
    virtual void post_break(AST_break* in);
    virtual void post_continue(AST_continue* in);
    virtual void post_return(AST_return* in);
    virtual void post_static_declaration(AST_static_declaration* in);
    virtual void post_global(AST_global* in);
    virtual void post_declare(AST_declare* in);
    virtual void post_directive(AST_directive* in);
    virtual void post_try(AST_try* in);
    virtual void post_catch(AST_catch* in);
    virtual void post_throw(AST_throw* in);
    virtual void post_eval_expr(AST_eval_expr* in);
    virtual void post_nop(AST_nop* in);
    virtual void post_branch(AST_branch* in);
    virtual void post_goto(AST_goto* in);
    virtual void post_label(AST_label* in);
    virtual void post_expr(AST_expr* in);
    virtual void post_literal(AST_literal* in);
    virtual void post_assignment(AST_assignment* in);
    virtual void post_op_assignment(AST_op_assignment* in);
    virtual void post_list_assignment(AST_list_assignment* in);
    virtual void post_list_element(AST_list_element* in);
    virtual void post_nested_list_elements(AST_nested_list_elements* in);
    virtual void post_cast(AST_cast* in);
    virtual void post_unary_op(AST_unary_op* in);
    virtual void post_bin_op(AST_bin_op* in);
    virtual void post_conditional_expr(AST_conditional_expr* in);
    virtual void post_ignore_errors(AST_ignore_errors* in);
    virtual void post_constant(AST_constant* in);
    virtual void post_instanceof(AST_instanceof* in);
    virtual void post_variable(AST_variable* in);
    virtual void post_variable_name(AST_variable_name* in);
    virtual void post_reflection(AST_reflection* in);
    virtual void post_target(AST_target* in);
    virtual void post_pre_op(AST_pre_op* in);
    virtual void post_post_op(AST_post_op* in);
    virtual void post_array(AST_array* in);
    virtual void post_array_elem(AST_array_elem* in);
    virtual void post_method_invocation(AST_method_invocation* in);
    virtual void post_method_name(AST_method_name* in);
    virtual void post_actual_parameter(AST_actual_parameter* in);
    virtual void post_new(AST_new* in);
    virtual void post_class_name(AST_class_name* in);
    virtual void post_commented_node(AST_commented_node* in);
    virtual void post_identifier(AST_identifier* in);
    virtual void post_class_name(Token_class_name* in);
    virtual void post_interface_name(Token_interface_name* in);
    virtual void post_method_name(Token_method_name* in);
    virtual void post_variable_name(Token_variable_name* in);
    virtual void post_directive_name(Token_directive_name* in);
    virtual void post_label_name(Token_label_name* in);
    virtual void post_int(Token_int* in);
    virtual void post_real(Token_real* in);
    virtual void post_string(Token_string* in);
    virtual void post_bool(Token_bool* in);
    virtual void post_null(Token_null* in);
    virtual void post_op(Token_op* in);
    virtual void post_cast(Token_cast* in);
    virtual void post_constant_name(Token_constant_name* in);
// Visit the children of a node
public:
    virtual void children_php_script(AST_php_script* in);
    virtual void children_class_def(AST_class_def* in);
    virtual void children_class_mod(AST_class_mod* in);
    virtual void children_interface_def(AST_interface_def* in);
    virtual void children_method(AST_method* in);
    virtual void children_signature(AST_signature* in);
    virtual void children_method_mod(AST_method_mod* in);
    virtual void children_formal_parameter(AST_formal_parameter* in);
    virtual void children_type(AST_type* in);
    virtual void children_attribute(AST_attribute* in);
    virtual void children_attr_mod(AST_attr_mod* in);
    virtual void children_name_with_default(AST_name_with_default* in);
    virtual void children_if(AST_if* in);
    virtual void children_while(AST_while* in);
    virtual void children_do(AST_do* in);
    virtual void children_for(AST_for* in);
    virtual void children_foreach(AST_foreach* in);
    virtual void children_switch(AST_switch* in);
    virtual void children_switch_case(AST_switch_case* in);
    virtual void children_break(AST_break* in);
    virtual void children_continue(AST_continue* in);
    virtual void children_return(AST_return* in);
    virtual void children_static_declaration(AST_static_declaration* in);
    virtual void children_global(AST_global* in);
    virtual void children_declare(AST_declare* in);
    virtual void children_directive(AST_directive* in);
    virtual void children_try(AST_try* in);
    virtual void children_catch(AST_catch* in);
    virtual void children_throw(AST_throw* in);
    virtual void children_eval_expr(AST_eval_expr* in);
    virtual void children_nop(AST_nop* in);
    virtual void children_branch(AST_branch* in);
    virtual void children_goto(AST_goto* in);
    virtual void children_label(AST_label* in);
    virtual void children_assignment(AST_assignment* in);
    virtual void children_op_assignment(AST_op_assignment* in);
    virtual void children_list_assignment(AST_list_assignment* in);
    virtual void children_nested_list_elements(AST_nested_list_elements* in);
    virtual void children_cast(AST_cast* in);
    virtual void children_unary_op(AST_unary_op* in);
    virtual void children_bin_op(AST_bin_op* in);
    virtual void children_conditional_expr(AST_conditional_expr* in);
    virtual void children_ignore_errors(AST_ignore_errors* in);
    virtual void children_constant(AST_constant* in);
    virtual void children_instanceof(AST_instanceof* in);
    virtual void children_variable(AST_variable* in);
    virtual void children_reflection(AST_reflection* in);
    virtual void children_pre_op(AST_pre_op* in);
    virtual void children_post_op(AST_post_op* in);
    virtual void children_array(AST_array* in);
    virtual void children_array_elem(AST_array_elem* in);
    virtual void children_method_invocation(AST_method_invocation* in);
    virtual void children_actual_parameter(AST_actual_parameter* in);
    virtual void children_new(AST_new* in);
// Tokens don't have children, so these methods do nothing by default
public:
    virtual void children_class_name(Token_class_name* in);
    virtual void children_interface_name(Token_interface_name* in);
    virtual void children_method_name(Token_method_name* in);
    virtual void children_variable_name(Token_variable_name* in);
    virtual void children_directive_name(Token_directive_name* in);
    virtual void children_label_name(Token_label_name* in);
    virtual void children_int(Token_int* in);
    virtual void children_real(Token_real* in);
    virtual void children_string(Token_string* in);
    virtual void children_bool(Token_bool* in);
    virtual void children_null(Token_null* in);
    virtual void children_op(Token_op* in);
    virtual void children_cast(Token_cast* in);
    virtual void children_constant_name(Token_constant_name* in);
// Unparser support
public:
    virtual void visit_marker(char const* name, bool value);
    virtual void visit_null(char const* type_id);
    virtual void visit_null_list(char const* type_id);
    virtual void pre_list(char const* type_id, int size);
    virtual void post_list(char const* type_id, int size);
// Invoke the chain of pre-visit methods along the inheritance hierachy
// Do not override unless you know what you are doing
public:
    virtual void pre_php_script_chain(AST_php_script* in);
    virtual void pre_class_def_chain(AST_class_def* in);
    virtual void pre_class_mod_chain(AST_class_mod* in);
    virtual void pre_interface_def_chain(AST_interface_def* in);
    virtual void pre_method_chain(AST_method* in);
    virtual void pre_signature_chain(AST_signature* in);
    virtual void pre_method_mod_chain(AST_method_mod* in);
    virtual void pre_formal_parameter_chain(AST_formal_parameter* in);
    virtual void pre_type_chain(AST_type* in);
    virtual void pre_attribute_chain(AST_attribute* in);
    virtual void pre_attr_mod_chain(AST_attr_mod* in);
    virtual void pre_name_with_default_chain(AST_name_with_default* in);
    virtual void pre_if_chain(AST_if* in);
    virtual void pre_while_chain(AST_while* in);
    virtual void pre_do_chain(AST_do* in);
    virtual void pre_for_chain(AST_for* in);
    virtual void pre_foreach_chain(AST_foreach* in);
    virtual void pre_switch_chain(AST_switch* in);
    virtual void pre_switch_case_chain(AST_switch_case* in);
    virtual void pre_break_chain(AST_break* in);
    virtual void pre_continue_chain(AST_continue* in);
    virtual void pre_return_chain(AST_return* in);
    virtual void pre_static_declaration_chain(AST_static_declaration* in);
    virtual void pre_global_chain(AST_global* in);
    virtual void pre_declare_chain(AST_declare* in);
    virtual void pre_directive_chain(AST_directive* in);
    virtual void pre_try_chain(AST_try* in);
    virtual void pre_catch_chain(AST_catch* in);
    virtual void pre_throw_chain(AST_throw* in);
    virtual void pre_eval_expr_chain(AST_eval_expr* in);
    virtual void pre_nop_chain(AST_nop* in);
    virtual void pre_branch_chain(AST_branch* in);
    virtual void pre_goto_chain(AST_goto* in);
    virtual void pre_label_chain(AST_label* in);
    virtual void pre_assignment_chain(AST_assignment* in);
    virtual void pre_op_assignment_chain(AST_op_assignment* in);
    virtual void pre_list_assignment_chain(AST_list_assignment* in);
    virtual void pre_nested_list_elements_chain(AST_nested_list_elements* in);
    virtual void pre_cast_chain(AST_cast* in);
    virtual void pre_unary_op_chain(AST_unary_op* in);
    virtual void pre_bin_op_chain(AST_bin_op* in);
    virtual void pre_conditional_expr_chain(AST_conditional_expr* in);
    virtual void pre_ignore_errors_chain(AST_ignore_errors* in);
    virtual void pre_constant_chain(AST_constant* in);
    virtual void pre_instanceof_chain(AST_instanceof* in);
    virtual void pre_variable_chain(AST_variable* in);
    virtual void pre_reflection_chain(AST_reflection* in);
    virtual void pre_pre_op_chain(AST_pre_op* in);
    virtual void pre_post_op_chain(AST_post_op* in);
    virtual void pre_array_chain(AST_array* in);
    virtual void pre_array_elem_chain(AST_array_elem* in);
    virtual void pre_method_invocation_chain(AST_method_invocation* in);
    virtual void pre_actual_parameter_chain(AST_actual_parameter* in);
    virtual void pre_new_chain(AST_new* in);
    virtual void pre_class_name_chain(Token_class_name* in);
    virtual void pre_interface_name_chain(Token_interface_name* in);
    virtual void pre_method_name_chain(Token_method_name* in);
    virtual void pre_variable_name_chain(Token_variable_name* in);
    virtual void pre_directive_name_chain(Token_directive_name* in);
    virtual void pre_label_name_chain(Token_label_name* in);
    virtual void pre_int_chain(Token_int* in);
    virtual void pre_real_chain(Token_real* in);
    virtual void pre_string_chain(Token_string* in);
    virtual void pre_bool_chain(Token_bool* in);
    virtual void pre_null_chain(Token_null* in);
    virtual void pre_op_chain(Token_op* in);
    virtual void pre_cast_chain(Token_cast* in);
    virtual void pre_constant_name_chain(Token_constant_name* in);
// Invoke the chain of post-visit methods along the inheritance hierarchy
// (invoked in opposite order to the pre-chain)
// Do not override unless you know what you are doing
public:
    virtual void post_php_script_chain(AST_php_script* in);
    virtual void post_class_def_chain(AST_class_def* in);
    virtual void post_class_mod_chain(AST_class_mod* in);
    virtual void post_interface_def_chain(AST_interface_def* in);
    virtual void post_method_chain(AST_method* in);
    virtual void post_signature_chain(AST_signature* in);
    virtual void post_method_mod_chain(AST_method_mod* in);
    virtual void post_formal_parameter_chain(AST_formal_parameter* in);
    virtual void post_type_chain(AST_type* in);
    virtual void post_attribute_chain(AST_attribute* in);
    virtual void post_attr_mod_chain(AST_attr_mod* in);
    virtual void post_name_with_default_chain(AST_name_with_default* in);
    virtual void post_if_chain(AST_if* in);
    virtual void post_while_chain(AST_while* in);
    virtual void post_do_chain(AST_do* in);
    virtual void post_for_chain(AST_for* in);
    virtual void post_foreach_chain(AST_foreach* in);
    virtual void post_switch_chain(AST_switch* in);
    virtual void post_switch_case_chain(AST_switch_case* in);
    virtual void post_break_chain(AST_break* in);
    virtual void post_continue_chain(AST_continue* in);
    virtual void post_return_chain(AST_return* in);
    virtual void post_static_declaration_chain(AST_static_declaration* in);
    virtual void post_global_chain(AST_global* in);
    virtual void post_declare_chain(AST_declare* in);
    virtual void post_directive_chain(AST_directive* in);
    virtual void post_try_chain(AST_try* in);
    virtual void post_catch_chain(AST_catch* in);
    virtual void post_throw_chain(AST_throw* in);
    virtual void post_eval_expr_chain(AST_eval_expr* in);
    virtual void post_nop_chain(AST_nop* in);
    virtual void post_branch_chain(AST_branch* in);
    virtual void post_goto_chain(AST_goto* in);
    virtual void post_label_chain(AST_label* in);
    virtual void post_assignment_chain(AST_assignment* in);
    virtual void post_op_assignment_chain(AST_op_assignment* in);
    virtual void post_list_assignment_chain(AST_list_assignment* in);
    virtual void post_nested_list_elements_chain(AST_nested_list_elements* in);
    virtual void post_cast_chain(AST_cast* in);
    virtual void post_unary_op_chain(AST_unary_op* in);
    virtual void post_bin_op_chain(AST_bin_op* in);
    virtual void post_conditional_expr_chain(AST_conditional_expr* in);
    virtual void post_ignore_errors_chain(AST_ignore_errors* in);
    virtual void post_constant_chain(AST_constant* in);
    virtual void post_instanceof_chain(AST_instanceof* in);
    virtual void post_variable_chain(AST_variable* in);
    virtual void post_reflection_chain(AST_reflection* in);
    virtual void post_pre_op_chain(AST_pre_op* in);
    virtual void post_post_op_chain(AST_post_op* in);
    virtual void post_array_chain(AST_array* in);
    virtual void post_array_elem_chain(AST_array_elem* in);
    virtual void post_method_invocation_chain(AST_method_invocation* in);
    virtual void post_actual_parameter_chain(AST_actual_parameter* in);
    virtual void post_new_chain(AST_new* in);
    virtual void post_class_name_chain(Token_class_name* in);
    virtual void post_interface_name_chain(Token_interface_name* in);
    virtual void post_method_name_chain(Token_method_name* in);
    virtual void post_variable_name_chain(Token_variable_name* in);
    virtual void post_directive_name_chain(Token_directive_name* in);
    virtual void post_label_name_chain(Token_label_name* in);
    virtual void post_int_chain(Token_int* in);
    virtual void post_real_chain(Token_real* in);
    virtual void post_string_chain(Token_string* in);
    virtual void post_bool_chain(Token_bool* in);
    virtual void post_null_chain(Token_null* in);
    virtual void post_op_chain(Token_op* in);
    virtual void post_cast_chain(Token_cast* in);
    virtual void post_constant_name_chain(Token_constant_name* in);
// Call the pre-chain, visit children and post-chain in order
// Do not override unless you know what you are doing
public:
    virtual void visit_statement_list(List<AST_statement*>* in);
    virtual void visit_statement(AST_statement* in);
    virtual void visit_class_mod(AST_class_mod* in);
    virtual void visit_class_name(Token_class_name* in);
    virtual void visit_interface_name_list(List<Token_interface_name*>* in);
    virtual void visit_member_list(List<AST_member*>* in);
    virtual void visit_member(AST_member* in);
    virtual void visit_interface_name(Token_interface_name* in);
    virtual void visit_signature(AST_signature* in);
    virtual void visit_method_mod(AST_method_mod* in);
    virtual void visit_method_name(Token_method_name* in);
    virtual void visit_formal_parameter_list(List<AST_formal_parameter*>* in);
    virtual void visit_formal_parameter(AST_formal_parameter* in);
    virtual void visit_type(AST_type* in);
    virtual void visit_name_with_default(AST_name_with_default* in);
    virtual void visit_attr_mod(AST_attr_mod* in);
    virtual void visit_name_with_default_list(List<AST_name_with_default*>* in);
    virtual void visit_variable_name(Token_variable_name* in);
    virtual void visit_expr(AST_expr* in);
    virtual void visit_variable(AST_variable* in);
    virtual void visit_switch_case_list(List<AST_switch_case*>* in);
    virtual void visit_switch_case(AST_switch_case* in);
    virtual void visit_variable_name_list(List<AST_variable_name*>* in);
    virtual void visit_directive_list(List<AST_directive*>* in);
    virtual void visit_directive(AST_directive* in);
    virtual void visit_directive_name(Token_directive_name* in);
    virtual void visit_catch_list(List<AST_catch*>* in);
    virtual void visit_catch(AST_catch* in);
    virtual void visit_label_name(Token_label_name* in);
    virtual void visit_op(Token_op* in);
    virtual void visit_list_element_list(List<AST_list_element*>* in);
    virtual void visit_list_element(AST_list_element* in);
    virtual void visit_cast(Token_cast* in);
    virtual void visit_constant_name(Token_constant_name* in);
    virtual void visit_class_name(AST_class_name* in);
    virtual void visit_target(AST_target* in);
    virtual void visit_variable_name(AST_variable_name* in);
    virtual void visit_expr_list(List<AST_expr*>* in);
    virtual void visit_array_elem_list(List<AST_array_elem*>* in);
    virtual void visit_array_elem(AST_array_elem* in);
    virtual void visit_method_name(AST_method_name* in);
    virtual void visit_actual_parameter_list(List<AST_actual_parameter*>* in);
    virtual void visit_actual_parameter(AST_actual_parameter* in);
    virtual void visit_php_script(AST_php_script* in);
// Invoke the right pre-chain (manual dispatching)
// Do not override unless you know what you are doing
public:
    virtual void pre_statement_chain(AST_statement* in);
    virtual void pre_member_chain(AST_member* in);
    virtual void pre_expr_chain(AST_expr* in);
    virtual void pre_variable_name_chain(AST_variable_name* in);
    virtual void pre_list_element_chain(AST_list_element* in);
    virtual void pre_class_name_chain(AST_class_name* in);
    virtual void pre_target_chain(AST_target* in);
    virtual void pre_method_name_chain(AST_method_name* in);
// Invoke the right post-chain (manual dispatching)
// Do not override unless you know what you are doing
public:
    virtual void post_statement_chain(AST_statement* in);
    virtual void post_member_chain(AST_member* in);
    virtual void post_expr_chain(AST_expr* in);
    virtual void post_variable_name_chain(AST_variable_name* in);
    virtual void post_list_element_chain(AST_list_element* in);
    virtual void post_class_name_chain(AST_class_name* in);
    virtual void post_target_chain(AST_target* in);
    virtual void post_method_name_chain(AST_method_name* in);
// Invoke the right visit-children (manual dispatching)
// Do not override unless you know what you are doing
public:
    virtual void children_statement(AST_statement* in);
    virtual void children_member(AST_member* in);
    virtual void children_expr(AST_expr* in);
    virtual void children_variable_name(AST_variable_name* in);
    virtual void children_list_element(AST_list_element* in);
    virtual void children_class_name(AST_class_name* in);
    virtual void children_target(AST_target* in);
    virtual void children_method_name(AST_method_name* in);
};
}


#endif
