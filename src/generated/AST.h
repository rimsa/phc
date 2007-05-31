#ifndef _AST_H_
#define _AST_H_

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


class AST_node;
class AST_php_script;
class AST_class_mod;
class AST_signature;
class AST_method_mod;
class AST_formal_parameter;
class AST_type;
class AST_attr_mod;
class AST_directive;
class AST_list_element;
class AST_variable_name;
class AST_target;
class AST_array_elem;
class AST_method_name;
class AST_actual_parameter;
class AST_class_name;
class AST_commented_node;
class AST_identifier;
class AST_statement;
class AST_member;
class AST_switch_case;
class AST_catch;
class AST_expr;
class AST_nested_list_elements;
class AST_reflection;
class Token_class_name;
class Token_interface_name;
class Token_method_name;
class Token_variable_name;
class Token_label_name;
class Token_directive_name;
class Token_cast;
class Token_op;
class Token_constant_name;
class AST_class_def;
class AST_interface_def;
class AST_method;
class AST_attribute;
class AST_if;
class AST_while;
class AST_do;
class AST_for;
class AST_foreach;
class AST_switch;
class AST_break;
class AST_continue;
class AST_return;
class AST_branch;
class AST_goto;
class AST_label;
class AST_static_declaration;
class AST_global;
class AST_unset;
class AST_declare;
class AST_try;
class AST_throw;
class AST_eval_expr;
class AST_nop;
class AST_literal;
class AST_assignment;
class AST_list_assignment;
class AST_cast;
class AST_unary_op;
class AST_bin_op;
class AST_conditional_expr;
class AST_ignore_errors;
class AST_constant;
class AST_instanceof;
class AST_variable;
class AST_pre_op;
class AST_post_op;
class AST_array;
class AST_method_invocation;
class AST_new;
class AST_clone;
class Token_int;
class Token_real;
class Token_string;
class Token_bool;
class Token_null;

class AST_transform;
class AST_visitor;

// node ::= php_script | class_mod | signature | method_mod | formal_parameter | type | attr_mod | directive | list_element | variable_name | target | array_elem | method_name | actual_parameter | class_name | commented_node | expr | identifier;
class AST_node : virtual public Object
{
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    AttrMap* attrs;
    //  Return the line number of the node (or 0 if unknown)
    int get_line_number();
    //  Return the filename of the node (or NULL if unknown)
    String* get_filename();
    AST_node();
    void clone_mixin_from(AST_node* in);
public:
    virtual AST_node* clone() = 0;
};

// php_script ::= statement* ;
class AST_php_script : virtual public AST_node
{
public:
    AST_php_script(List<AST_statement*>* statements);
protected:
    AST_php_script();
public:
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 1;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_php_script* clone();
};

// class_mod ::= "abstract"? "final"? ;
class AST_class_mod : virtual public AST_node
{
public:
    AST_class_mod(bool is_abstract, bool is_final);
protected:
    AST_class_mod();
public:
    bool is_abstract;
    bool is_final;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 3;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_class_mod* clone();
};

// signature ::= method_mod is_ref:"&" METHOD_NAME formal_parameter* ;
class AST_signature : virtual public AST_node
{
public:
    AST_signature(AST_method_mod* method_mod, bool is_ref, Token_method_name* method_name, List<AST_formal_parameter*>* formal_parameters);
protected:
    AST_signature();
public:
    AST_method_mod* method_mod;
    bool is_ref;
    Token_method_name* method_name;
    List<AST_formal_parameter*>* formal_parameters;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 6;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_signature(const char* name);
public:
    virtual AST_signature* clone();
};

// method_mod ::= "public"? "protected"? "private"? "static"? "abstract"? "final"? ;
class AST_method_mod : virtual public AST_node
{
public:
    AST_method_mod(bool is_public, bool is_protected, bool is_private, bool is_static, bool is_abstract, bool is_final);
public:
    bool is_public;
    bool is_protected;
    bool is_private;
    bool is_static;
    bool is_abstract;
    bool is_final;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 7;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_method_mod();
    AST_method_mod(AST_method_mod* a, AST_method_mod* b);
    static AST_method_mod* new_PUBLIC();
    static AST_method_mod* new_PROTECTED();
    static AST_method_mod* new_PRIVATE();
    static AST_method_mod* new_STATIC();
    static AST_method_mod* new_ABSTRACT();
    static AST_method_mod* new_FINAL();
public:
    virtual AST_method_mod* clone();
};

// formal_parameter ::= type is_ref:"&" VARIABLE_NAME expr? ;
class AST_formal_parameter : virtual public AST_node
{
public:
    AST_formal_parameter(AST_type* type, bool is_ref, Token_variable_name* variable_name, AST_expr* expr);
protected:
    AST_formal_parameter();
public:
    AST_type* type;
    bool is_ref;
    Token_variable_name* variable_name;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 8;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_formal_parameter(AST_type* type, Token_variable_name* name);
    AST_formal_parameter(AST_type* type, bool is_ref, Token_variable_name* name);
public:
    virtual AST_formal_parameter* clone();
};

// type ::= "array"? CLASS_NAME? ;
class AST_type : virtual public AST_node
{
public:
    AST_type(bool is_array, Token_class_name* class_name);
protected:
    AST_type();
public:
    bool is_array;
    Token_class_name* class_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 9;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_type* clone();
};

// attr_mod ::= "public"? "protected"? "private"? "static"? "const"? ;
class AST_attr_mod : virtual public AST_node
{
public:
    AST_attr_mod(bool is_public, bool is_protected, bool is_private, bool is_static, bool is_const);
public:
    bool is_public;
    bool is_protected;
    bool is_private;
    bool is_static;
    bool is_const;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 11;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_attr_mod();
    AST_attr_mod(AST_method_mod* mm);
    static AST_attr_mod* new_PUBLIC();
    static AST_attr_mod* new_PROTECTED();
    static AST_attr_mod* new_PRIVATE();
    static AST_attr_mod* new_STATIC();
    static AST_attr_mod* new_CONST();
public:
    virtual AST_attr_mod* clone();
};

// directive ::= DIRECTIVE_NAME expr ;
class AST_directive : virtual public AST_node
{
public:
    AST_directive(Token_directive_name* directive_name, AST_expr* expr);
protected:
    AST_directive();
public:
    Token_directive_name* directive_name;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 29;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_directive* clone();
};

// list_element ::= variable | nested_list_elements;
class AST_list_element : virtual public AST_node
{
public:
    AST_list_element();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_list_element* clone() = 0;
};

// variable_name ::= VARIABLE_NAME | reflection;
class AST_variable_name : virtual public AST_node
{
public:
    AST_variable_name();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_variable_name* clone() = 0;
};

// target ::= expr | CLASS_NAME;
class AST_target : virtual public AST_node
{
public:
    AST_target();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_target* clone() = 0;
};

// array_elem ::= key:expr? is_ref:"&" val:expr ;
class AST_array_elem : virtual public AST_node
{
public:
    AST_array_elem(AST_expr* key, bool is_ref, AST_expr* val);
protected:
    AST_array_elem();
public:
    AST_expr* key;
    bool is_ref;
    AST_expr* val;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 50;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_array_elem* clone();
};

// method_name ::= METHOD_NAME | reflection;
class AST_method_name : virtual public AST_node
{
public:
    AST_method_name();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_method_name* clone() = 0;
};

// actual_parameter ::= is_ref:"&" expr ;
class AST_actual_parameter : virtual public AST_node
{
public:
    AST_actual_parameter(bool is_ref, AST_expr* expr);
protected:
    AST_actual_parameter();
public:
    bool is_ref;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 52;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_actual_parameter* clone();
};

// class_name ::= CLASS_NAME | reflection;
class AST_class_name : virtual public AST_node
{
public:
    AST_class_name();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_class_name* clone() = 0;
};

// commented_node ::= member | statement | interface_def | class_def | switch_case | catch;
class AST_commented_node : virtual public AST_node
{
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    AST_commented_node();
    //  Return the comments associated with the node
    List<String*>* get_comments();
public:
    virtual AST_commented_node* clone() = 0;
};

// identifier ::= INTERFACE_NAME | CLASS_NAME | METHOD_NAME | VARIABLE_NAME | DIRECTIVE_NAME | CAST | OP | CONSTANT_NAME | LABEL_NAME;
class AST_identifier : virtual public AST_node
{
public:
    AST_identifier();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual String* get_value_as_string() = 0;
public:
    virtual AST_identifier* clone() = 0;
};

// statement ::= class_def | interface_def | method | if | while | do | for | foreach | switch | break | continue | return | static_declaration | global | unset | declare | try | throw | eval_expr | nop | label | goto | branch;
class AST_statement : virtual public AST_commented_node
{
public:
    AST_statement();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_statement* clone() = 0;
};

// member ::= method | attribute;
class AST_member : virtual public AST_commented_node
{
public:
    AST_member();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual AST_member* clone() = 0;
};

// switch_case ::= expr? statement* ;
class AST_switch_case : virtual public AST_commented_node
{
public:
    AST_switch_case(AST_expr* expr, List<AST_statement*>* statements);
protected:
    AST_switch_case();
public:
    AST_expr* expr;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 18;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_switch_case* clone();
};

// catch ::= CLASS_NAME VARIABLE_NAME statement* ;
class AST_catch : virtual public AST_commented_node
{
public:
    AST_catch(Token_class_name* class_name, Token_variable_name* variable_name, List<AST_statement*>* statements);
protected:
    AST_catch();
public:
    Token_class_name* class_name;
    Token_variable_name* variable_name;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 31;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_catch* clone();
};

// expr ::= assignment | list_assignment | cast | unary_op | bin_op | conditional_expr | ignore_errors | constant | instanceof | variable | pre_op | post_op | array | method_invocation | new | clone | literal;
class AST_expr : virtual public AST_target, virtual public AST_node
{
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    AST_expr();
public:
    virtual AST_expr* clone() = 0;
};

// nested_list_elements ::= list_element?* ;
class AST_nested_list_elements : virtual public AST_list_element
{
public:
    AST_nested_list_elements(List<AST_list_element*>* list_elements);
protected:
    AST_nested_list_elements();
public:
    List<AST_list_element*>* list_elements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 37;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_nested_list_elements* clone();
};

// reflection ::= expr ;
class AST_reflection : virtual public AST_variable_name, virtual public AST_method_name, virtual public AST_class_name
{
public:
    AST_reflection(AST_expr* expr);
protected:
    AST_reflection();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 46;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_reflection* clone();
};

class Token_class_name : virtual public AST_target, virtual public AST_class_name, virtual public AST_identifier
{
public:
    Token_class_name(String* value);
protected:
    Token_class_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 55;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_class_name* clone();
};

class Token_interface_name : virtual public AST_identifier
{
public:
    Token_interface_name(String* value);
protected:
    Token_interface_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 56;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_interface_name* clone();
};

class Token_method_name : virtual public AST_method_name, virtual public AST_identifier
{
public:
    Token_method_name(String* value);
protected:
    Token_method_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 57;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_method_name* clone();
};

class Token_variable_name : virtual public AST_variable_name, virtual public AST_identifier
{
public:
    Token_variable_name(String* value);
protected:
    Token_variable_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 58;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_variable_name* clone();
};

class Token_label_name : virtual public AST_identifier
{
public:
    Token_label_name(String* value);
protected:
    Token_label_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 59;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_label_name* clone();
};

class Token_directive_name : virtual public AST_identifier
{
public:
    Token_directive_name(String* value);
protected:
    Token_directive_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 60;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_directive_name* clone();
};

class Token_cast : virtual public AST_identifier
{
public:
    Token_cast(String* value);
protected:
    Token_cast();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 66;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_cast* clone();
};

class Token_op : virtual public AST_identifier
{
public:
    Token_op(String* value);
protected:
    Token_op();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 67;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_op* clone();
};

class Token_constant_name : virtual public AST_identifier
{
public:
    Token_constant_name(String* value);
protected:
    Token_constant_name();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    virtual String* get_value_as_string();
public:
    static const int ID = 68;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual Token_constant_name* clone();
};

// class_def ::= class_mod CLASS_NAME extends:CLASS_NAME? implements:INTERFACE_NAME* member* ;
class AST_class_def : virtual public AST_statement, virtual public AST_commented_node
{
public:
    AST_class_def(AST_class_mod* class_mod, Token_class_name* class_name, Token_class_name* extends, List<Token_interface_name*>* implements, List<AST_member*>* members);
protected:
    AST_class_def();
public:
    AST_class_mod* class_mod;
    Token_class_name* class_name;
    Token_class_name* extends;
    List<Token_interface_name*>* implements;
    List<AST_member*>* members;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 2;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_class_def(AST_class_mod* mod);
    AST_class_def(char* name);
    void add_member(AST_member* member);
    //  Returns NULL if the method could not be found
    AST_method* get_method(const char* name);
public:
    virtual AST_class_def* clone();
};

// interface_def ::= INTERFACE_NAME extends:INTERFACE_NAME* member* ;
class AST_interface_def : virtual public AST_statement, virtual public AST_commented_node
{
public:
    AST_interface_def(Token_interface_name* interface_name, List<Token_interface_name*>* extends, List<AST_member*>* members);
protected:
    AST_interface_def();
public:
    Token_interface_name* interface_name;
    List<Token_interface_name*>* extends;
    List<AST_member*>* members;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 4;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_interface_def* clone();
};

// method ::= signature statement*? ;
class AST_method : virtual public AST_statement, virtual public AST_member
{
public:
    AST_method(AST_signature* signature, List<AST_statement*>* statements);
protected:
    AST_method();
public:
    AST_signature* signature;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 5;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_method* clone();
};

// attribute ::= attr_mod VARIABLE_NAME expr? ;
class AST_attribute : virtual public AST_member
{
public:
    AST_attribute(AST_attr_mod* attr_mod, Token_variable_name* variable_name, AST_expr* expr);
protected:
    AST_attribute();
public:
    AST_attr_mod* attr_mod;
    Token_variable_name* variable_name;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 10;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_attribute* clone();
};

// if ::= expr iftrue:statement* iffalse:statement* ;
class AST_if : virtual public AST_statement
{
public:
    AST_if(AST_expr* expr, List<AST_statement*>* iftrue, List<AST_statement*>* iffalse);
protected:
    AST_if();
public:
    AST_expr* expr;
    List<AST_statement*>* iftrue;
    List<AST_statement*>* iffalse;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 12;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_if(AST_expr* expr);
public:
    virtual AST_if* clone();
};

// while ::= expr statement* ;
class AST_while : virtual public AST_statement
{
public:
    AST_while(AST_expr* expr, List<AST_statement*>* statements);
protected:
    AST_while();
public:
    AST_expr* expr;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 13;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_while* clone();
};

// do ::= statement* expr ;
class AST_do : virtual public AST_statement
{
public:
    AST_do(List<AST_statement*>* statements, AST_expr* expr);
protected:
    AST_do();
public:
    List<AST_statement*>* statements;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 14;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_do* clone();
};

// for ::= init:expr? cond:expr? incr:expr? statement* ;
class AST_for : virtual public AST_statement
{
public:
    AST_for(AST_expr* init, AST_expr* cond, AST_expr* incr, List<AST_statement*>* statements);
protected:
    AST_for();
public:
    AST_expr* init;
    AST_expr* cond;
    AST_expr* incr;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 15;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_for* clone();
};

// foreach ::= expr key:variable? is_ref:"&" val:variable statement* ;
class AST_foreach : virtual public AST_statement
{
public:
    AST_foreach(AST_expr* expr, AST_variable* key, bool is_ref, AST_variable* val, List<AST_statement*>* statements);
protected:
    AST_foreach();
public:
    AST_expr* expr;
    AST_variable* key;
    bool is_ref;
    AST_variable* val;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 16;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_foreach* clone();
};

// switch ::= expr switch_case* ;
class AST_switch : virtual public AST_statement
{
public:
    AST_switch(AST_expr* expr, List<AST_switch_case*>* switch_cases);
protected:
    AST_switch();
public:
    AST_expr* expr;
    List<AST_switch_case*>* switch_cases;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 17;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_switch* clone();
};

// break ::= expr? ;
class AST_break : virtual public AST_statement
{
public:
    AST_break(AST_expr* expr);
protected:
    AST_break();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 19;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_break* clone();
};

// continue ::= expr? ;
class AST_continue : virtual public AST_statement
{
public:
    AST_continue(AST_expr* expr);
protected:
    AST_continue();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 20;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_continue* clone();
};

// return ::= expr? ;
class AST_return : virtual public AST_statement
{
public:
    AST_return(AST_expr* expr);
protected:
    AST_return();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 21;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_return* clone();
};

// branch ::= expr iftrue:LABEL_NAME iffalse:LABEL_NAME ;
class AST_branch : virtual public AST_statement
{
public:
    AST_branch(AST_expr* expr, Token_label_name* iftrue, Token_label_name* iffalse);
protected:
    AST_branch();
public:
    AST_expr* expr;
    Token_label_name* iftrue;
    Token_label_name* iffalse;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 22;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_branch* clone();
};

// goto ::= LABEL_NAME ;
class AST_goto : virtual public AST_statement
{
public:
    AST_goto(Token_label_name* label_name);
protected:
    AST_goto();
public:
    Token_label_name* label_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 23;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_goto* clone();
};

// label ::= LABEL_NAME ;
class AST_label : virtual public AST_statement
{
public:
    AST_label(Token_label_name* label_name);
protected:
    AST_label();
public:
    Token_label_name* label_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 24;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_label* clone();
};

// static_declaration ::= VARIABLE_NAME expr? ;
class AST_static_declaration : virtual public AST_statement
{
public:
    AST_static_declaration(Token_variable_name* variable_name, AST_expr* expr);
protected:
    AST_static_declaration();
public:
    Token_variable_name* variable_name;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 25;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_static_declaration* clone();
};

// global ::= variable_name ;
class AST_global : virtual public AST_statement
{
public:
    AST_global(AST_variable_name* variable_name);
protected:
    AST_global();
public:
    AST_variable_name* variable_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 26;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_global* clone();
};

// unset ::= variable ;
class AST_unset : virtual public AST_statement
{
public:
    AST_unset(AST_variable* variable);
protected:
    AST_unset();
public:
    AST_variable* variable;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 27;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_unset* clone();
};

// declare ::= directive* statement* ;
class AST_declare : virtual public AST_statement
{
public:
    AST_declare(List<AST_directive*>* directives, List<AST_statement*>* statements);
protected:
    AST_declare();
public:
    List<AST_directive*>* directives;
    List<AST_statement*>* statements;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 28;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_declare* clone();
};

// try ::= statement* catches:catch* ;
class AST_try : virtual public AST_statement
{
public:
    AST_try(List<AST_statement*>* statements, List<AST_catch*>* catches);
protected:
    AST_try();
public:
    List<AST_statement*>* statements;
    List<AST_catch*>* catches;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 30;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_try* clone();
};

// throw ::= expr ;
class AST_throw : virtual public AST_statement
{
public:
    AST_throw(AST_expr* expr);
protected:
    AST_throw();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 32;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_throw* clone();
};

// eval_expr ::= expr ;
class AST_eval_expr : virtual public AST_statement
{
public:
    AST_eval_expr(AST_expr* expr);
protected:
    AST_eval_expr();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 33;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    void _init();
public:
    virtual AST_eval_expr* clone();
};

// nop ::= ;
class AST_nop : virtual public AST_statement
{
public:
    AST_nop();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 34;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_nop* clone();
};

// literal ::= INT<long> | REAL<double> | STRING<String*> | BOOL<bool> | NULL<>;
class AST_literal : virtual public AST_expr
{
public:
    AST_literal();
public:
    virtual void visit(AST_visitor* visitor) = 0;
    virtual void transform_children(AST_transform* transform) = 0;
public:
    virtual int classid() = 0;
public:
    virtual bool match(AST_node* in) = 0;
public:
    virtual bool equals(AST_node* in) = 0;
public:
    virtual String* get_value_as_string() = 0;
    virtual String* get_source_rep() = 0;
public:
    virtual AST_literal* clone() = 0;
};

// assignment ::= variable is_ref:"&" expr ;
class AST_assignment : virtual public AST_expr
{
public:
    AST_assignment(AST_variable* variable, bool is_ref, AST_expr* expr);
protected:
    AST_assignment();
public:
    AST_variable* variable;
    bool is_ref;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 35;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_assignment* clone();
};

// list_assignment ::= list_element?* expr ;
class AST_list_assignment : virtual public AST_expr
{
public:
    AST_list_assignment(List<AST_list_element*>* list_elements, AST_expr* expr);
protected:
    AST_list_assignment();
public:
    List<AST_list_element*>* list_elements;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 36;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_list_assignment* clone();
};

// cast ::= CAST expr ;
class AST_cast : virtual public AST_expr
{
public:
    AST_cast(Token_cast* cast, AST_expr* expr);
protected:
    AST_cast();
public:
    Token_cast* cast;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 38;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_cast(char* cast, AST_expr* expr);
public:
    virtual AST_cast* clone();
};

// unary_op ::= OP expr ;
class AST_unary_op : virtual public AST_expr
{
public:
    AST_unary_op(Token_op* op, AST_expr* expr);
protected:
    AST_unary_op();
public:
    Token_op* op;
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 39;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_unary_op(AST_expr* expr, char* op);
public:
    virtual AST_unary_op* clone();
};

// bin_op ::= left:expr OP right:expr ;
class AST_bin_op : virtual public AST_expr
{
public:
    AST_bin_op(AST_expr* left, Token_op* op, AST_expr* right);
protected:
    AST_bin_op();
public:
    AST_expr* left;
    Token_op* op;
    AST_expr* right;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 40;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_bin_op(AST_expr* left, AST_expr* right, char* op);
public:
    virtual AST_bin_op* clone();
};

// conditional_expr ::= cond:expr iftrue:expr iffalse:expr ;
class AST_conditional_expr : virtual public AST_expr
{
public:
    AST_conditional_expr(AST_expr* cond, AST_expr* iftrue, AST_expr* iffalse);
protected:
    AST_conditional_expr();
public:
    AST_expr* cond;
    AST_expr* iftrue;
    AST_expr* iffalse;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 41;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_conditional_expr* clone();
};

// ignore_errors ::= expr ;
class AST_ignore_errors : virtual public AST_expr
{
public:
    AST_ignore_errors(AST_expr* expr);
protected:
    AST_ignore_errors();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 42;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_ignore_errors* clone();
};

// constant ::= CLASS_NAME? CONSTANT_NAME ;
class AST_constant : virtual public AST_expr
{
public:
    AST_constant(Token_class_name* class_name, Token_constant_name* constant_name);
protected:
    AST_constant();
public:
    Token_class_name* class_name;
    Token_constant_name* constant_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 43;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_constant* clone();
};

// instanceof ::= expr class_name ;
class AST_instanceof : virtual public AST_expr
{
public:
    AST_instanceof(AST_expr* expr, AST_class_name* class_name);
protected:
    AST_instanceof();
public:
    AST_expr* expr;
    AST_class_name* class_name;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 44;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_instanceof* clone();
};

// variable ::= target? variable_name array_indices:expr?* ;
class AST_variable : virtual public AST_expr, virtual public AST_list_element
{
public:
    AST_variable(AST_target* target, AST_variable_name* variable_name, List<AST_expr*>* array_indices);
protected:
    AST_variable();
public:
    AST_target* target;
    AST_variable_name* variable_name;
    List<AST_expr*>* array_indices;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 45;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_variable(AST_variable_name* name);
    void _init();
public:
    virtual AST_variable* clone();
};

// pre_op ::= OP variable ;
class AST_pre_op : virtual public AST_expr
{
public:
    AST_pre_op(Token_op* op, AST_variable* variable);
protected:
    AST_pre_op();
public:
    Token_op* op;
    AST_variable* variable;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 47;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_pre_op(AST_variable* var, char* op);
public:
    virtual AST_pre_op* clone();
};

// post_op ::= variable OP ;
class AST_post_op : virtual public AST_expr
{
public:
    AST_post_op(AST_variable* variable, Token_op* op);
protected:
    AST_post_op();
public:
    AST_variable* variable;
    Token_op* op;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 48;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    AST_post_op(AST_variable* var, char* op);
public:
    virtual AST_post_op* clone();
};

// array ::= array_elem* ;
class AST_array : virtual public AST_expr
{
public:
    AST_array(List<AST_array_elem*>* array_elems);
protected:
    AST_array();
public:
    List<AST_array_elem*>* array_elems;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 49;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_array* clone();
};

// method_invocation ::= target? method_name actual_parameter* ;
class AST_method_invocation : virtual public AST_expr
{
public:
    AST_method_invocation(AST_target* target, AST_method_name* method_name, List<AST_actual_parameter*>* actual_parameters);
protected:
    AST_method_invocation();
public:
    AST_target* target;
    AST_method_name* method_name;
    List<AST_actual_parameter*>* actual_parameters;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 51;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    //  For internal use only!
    AST_method_invocation(const char* name, AST_expr* arg);
    //  For internal use only!
    AST_method_invocation(Token_method_name* name, AST_expr* arg);
    //  This does in fact create a valid subtree
    AST_method_invocation(const char* target, const char* name, AST_expr* arg);
public:
    virtual AST_method_invocation* clone();
};

// new ::= class_name actual_parameter* ;
class AST_new : virtual public AST_expr
{
public:
    AST_new(AST_class_name* class_name, List<AST_actual_parameter*>* actual_parameters);
protected:
    AST_new();
public:
    AST_class_name* class_name;
    List<AST_actual_parameter*>* actual_parameters;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 53;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_new* clone();
};

// clone ::= expr ;
class AST_clone : virtual public AST_expr
{
public:
    AST_clone(AST_expr* expr);
protected:
    AST_clone();
public:
    AST_expr* expr;
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    static const int ID = 54;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual AST_clone* clone();
};

class Token_int : virtual public AST_literal
{
public:
    Token_int(long value, String* source_rep);
protected:
    Token_int();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    long value;
    String* source_rep;
    virtual String* get_source_rep();
public:
    static const int ID = 61;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
    virtual bool match_value(Token_int* that);
public:
    virtual bool equals(AST_node* in);
    virtual bool equals_value(Token_int* that);
public:
    virtual String* get_value_as_string();
    Token_int(int v);
private:
    void call_initializing_virtuals();
public:
    virtual Token_int* clone();
    virtual long clone_value();
};

class Token_real : virtual public AST_literal
{
public:
    Token_real(double value, String* source_rep);
protected:
    Token_real();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    double value;
    String* source_rep;
    virtual String* get_source_rep();
public:
    static const int ID = 62;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
    virtual bool match_value(Token_real* that);
public:
    virtual bool equals(AST_node* in);
    virtual bool equals_value(Token_real* that);
public:
    virtual String* get_value_as_string();
    Token_real(double v);
private:
    void call_initializing_virtuals();
public:
    virtual Token_real* clone();
    virtual double clone_value();
};

class Token_string : virtual public AST_literal
{
public:
    Token_string(String* value, String* source_rep);
protected:
    Token_string();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* value;
    String* source_rep;
    virtual String* get_source_rep();
public:
    static const int ID = 63;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
    virtual bool match_value(Token_string* that);
public:
    virtual bool equals(AST_node* in);
    virtual bool equals_value(Token_string* that);
public:
    virtual String* get_value_as_string();
    Token_string(String* v);
public:
    virtual Token_string* clone();
    virtual String* clone_value();
};

class Token_bool : virtual public AST_literal
{
public:
    Token_bool(bool value, String* source_rep);
protected:
    Token_bool();
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    bool value;
    String* source_rep;
    virtual String* get_source_rep();
public:
    static const int ID = 64;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
    virtual bool match_value(Token_bool* that);
public:
    virtual bool equals(AST_node* in);
    virtual bool equals_value(Token_bool* that);
public:
    virtual String* get_value_as_string();
    Token_bool(bool v);
private:
    void call_initializing_virtuals();
public:
    virtual Token_bool* clone();
    virtual bool clone_value();
};

class Token_null : virtual public AST_literal
{
public:
    Token_null(String* source_rep);
public:
    virtual void visit(AST_visitor* visitor);
    virtual void transform_children(AST_transform* transform);
public:
    String* source_rep;
    virtual String* get_source_rep();
public:
    static const int ID = 65;
    virtual int classid();
public:
    virtual bool match(AST_node* in);
public:
    virtual bool equals(AST_node* in);
public:
    virtual String* get_value_as_string();
    Token_null();
private:
    void call_initializing_virtuals();
public:
    virtual Token_null* clone();
};


class __WILDCARD__
{
public:
	virtual ~__WILDCARD__() {}

public:
	virtual bool match(AST_node* in) = 0;
};

template<class C>
class Wildcard : public virtual C, public __WILDCARD__
{
public:
	Wildcard() : value(NULL) {}
	Wildcard(C* v) : value(v) {}
	virtual ~Wildcard() {}

public:
	C* value;

	virtual bool match(AST_node* in)
	{
		C* that = dynamic_cast<C*>(in);
		if(in == NULL || that != NULL)
		{
			value = that;
			return true;
		}
		return false;
	}

	virtual Wildcard* clone()
	{
		if(value != NULL)
			return new Wildcard(value->clone());
		else
			return new Wildcard(NULL);
	}

	virtual bool equals(AST_node* in)
	{
		Wildcard* that = dynamic_cast<Wildcard*>(in);
		if(that == NULL) return false;

		if(this->value == NULL || that->value == NULL)
		{
			if(this->value != NULL || that->value != NULL)
				return false;
		}

		return value->equals(that->value);
	}

	virtual void visit(AST_visitor* visitor)
	{
		if(value != NULL)
			value->visit(visitor);
	}

	virtual void transform_children(AST_transform* transform)
	{
		if(value != NULL)
			value->transform_children(transform);
	}

public:
	static const int ID = 69;
	int classid()
	{
		return ID;
	}
};


#endif
