/* Quick intro, using this macro as an example:
 *
 * assign_expr_var (token LHS, token RHS)
 *    where LHS.st_entry_not_required
 *    where LHS.is_uninitialized
 * @@@
 *   zval** p_lhs = &local_$LHS;
 *   \read_rvalue ("rhs", RHS);
 *   \write_var ("p_lhs", "rhs", LHS, RHS);
 * @@@
 *
 * Macros are defined by their name, a list of argument, a list of rules, and a
 * body. Generate_C will attempt to 'instantiate' a macro with its name and
 * some arguments. Each macro of that name will be matched against in the order
 * it appears. When all of pattern's rules match the parameters, that macro
 * will be evaluated. If no macro matches, and error is given.
 *
 * Matching:
 *    Before we look at the rules, we need to look at the signature. The
 *    signature is a list of parameters, in capital letters (LHS and RHS), and
 *    types (token in both cases).
 *
 *    A rule is a line starting with 'where', and followed by a boolean expression. There are only two kinds of boolean expr:
 *	 Lookup: Parameters which are 'node's (ie correspond to an MIR::Node*)
 *	 can have their attrs looked up. If they have do not have the
 *	 attribute, or they have a Boolean false stored, the lookup fails.
 *	 Otherwise it succeeds.
 *
 *	 Equals: In the form 'LHS == RHS' - this does a string comparison
 *	 between the parameters, where they are coerced to strings. Coersions
 *	 will be explained in the type section later.
 *
 *	 The operands of an Equals are expressions: Parameters, strings, calls
 *	 to other macros or callbacks. Macro calls and callbacks are explained
 *	 later.
 *
 * Types:
 *    There are only 3 types, whose name must be used in a type signature:
 *       node: Can have attributes lookup up. Evaluating it is a evaluation-time error.
 *
 *       string: Represents a String* (or a quoted string when created in 'user code').
 *
 *	 token: A token is an MIR::Identifier*. Attributes can be looked up, or
 *	 it can be evaluated itself, which coerces it to its 'value' string.
 *	 Tokens can be used as either strings or nodes.
 *
 *    Evaluating a lookup can also produce a boolean, but these are always
 *    coerced to either "TRUE" or "FALSE".
 *
 *
 * Bodies:
 *    The body is simply a C string, interspersed with interpolations, macro calls and callbacks. It starts and
 *    ends with @@@. The evalutation of a macro is simply a string of its
 *    contents, with appropriate strings substituted in for interpolations etc.
 *    Whitespace is preserved.
 *
 *    Bodies allow two kinds of interpolations:
 *      $LHS: coerces LHS into a string (which works for tokens and strings,
 *      and leads to evalutation-time error for nodes).
 *
 *       ${LHS.attr_name}: the same as a lookup.
 *
 * Macro calls:
 *    Rules and bodies both support macro calls. In the example above,
 *    \read_value (...) is  macro call. Parameters can be nodes, tokens, or
 *    strings. Strings are written as quoted strings: "rhs". Nodes and tokens
 *    must, of course, be passed using their name in the caller.
 *
 * Callbacks:
 *    Callbacks are the same as macros, except they call a c++ function, which
 *    must be registed in Generate_C. They are called using:
*     \cb:call_back_name (PARAM). For now, only 1 parameter is allowed, but its
*     easy to fix that.
 *
 * Comments:
 *    C or C++ comments, are allowed outside bodies. Comments inside bodies and
 *    will appear in the generated code.
 *
 *  Note: I may be wrong about some of the evaluation-time errors for node
 *  coersions. If you see some stray "TRUE"s in your code, this may be the
 *  problem.
 */


/*
 * Simple assignments
 * $x = $y;
 */

assign_expr_var (token LHS, token RHS)
   where LHS.st_entry_not_required
   where LHS.is_uninitialized
@@@
  zval** p_lhs = &local_$LHS;
  \read_rvalue ("rhs", RHS);
  \write_var ("p_lhs", "rhs", LHS, RHS);
@@@

assign_expr_var (token LHS, token RHS)
@@@
  \get_st_entry ("LOCAL", "p_lhs", LHS);
  \read_rvalue ("rhs", RHS);
  if (*p_lhs != rhs)
    {
      \write_var ("p_lhs", "rhs", LHS, RHS);
    }
@@@



/*
 * $x =& $y;
 */
assign_expr_ref_var (token LHS, token RHS)
@@@
  \get_st_entry ("LOCAL", "p_lhs", LHS);
  \get_st_entry ("LOCAL", "p_rhs", RHS);
  sep_copy_on_write (p_rhs);
  copy_into_ref (p_lhs, p_rhs);
@@@

/*
 * Casts
 */
assign_expr_cast (token LHS, token RHS, string TYPE)
@@@
  \assign_expr_var (LHS, RHS);
  \cast_var ("p_lhs", TYPE);
@@@

cast_var (string LHS, string TYPE)
@@@
  assert ($TYPE >= 0 && $TYPE <= 6);
  if ((*$LHS)->type != $TYPE)
  {
    sep_copy_on_write ($LHS);
    \convert_to (TYPE) (*$LHS);
  }
@@@

convert_to (string TYPE) where TYPE == "IS_ARRAY" @@@convert_to_array@@@
convert_to (string TYPE) where TYPE == "IS_BOOL" @@@convert_to_boolean@@@
convert_to (string TYPE) where TYPE == "IS_DOUBLE" @@@convert_to_double@@@
convert_to (string TYPE) where TYPE == "IS_LONG" @@@convert_to_long@@@
convert_to (string TYPE) where TYPE == "IS_NULL" @@@convert_to_null@@@
convert_to (string TYPE) where TYPE == "IS_STRING" @@@convert_to_string@@@
convert_to (string TYPE) where TYPE == "IS_OBJECT" @@@convert_to_object@@@

/*
 * Bin-ops
 */

// We could do this for non-LOCAL, but we'd only be saving an refcount++ and a refcount--.
assign_expr_bin_op (token LHS, node LEFT, node RIGHT, string OP_FN)
   where LHS.st_entry_not_required
   where LHS.is_uninitialized
@@@
  \read_rvalue ("left", LEFT);
  \read_rvalue ("right", RIGHT);

  zval** p_lhs = &local_$LHS;
  ALLOC_INIT_ZVAL (*p_lhs);

  $OP_FN (*p_lhs, left, right TSRMLS_CC);
@@@

// OP_FN: for example "add_function"
// The caller must sort out the order of LEFT and RIGHT for > and >=
// We use NODE for LEFT and RIGHT, since they might be literals
assign_expr_bin_op (token LHS, node LEFT, node RIGHT, string OP_FN)
@@@
  \get_st_entry ("LOCAL", "p_lhs", LHS);
  \read_rvalue ("left", LEFT);
  \read_rvalue ("right", RIGHT);
  if (in_copy_on_write (*p_lhs))
    {
      zval_ptr_dtor (p_lhs);
      ALLOC_INIT_ZVAL (*p_lhs);
    }

  zval old = **p_lhs;
  int result_is_operand = (*p_lhs == left || *p_lhs == right);
  $OP_FN (*p_lhs, left, right TSRMLS_CC);

  // If the result is one of the operands, the operator function
  // will already have cleaned up the result
  if (!result_is_operand)
    zval_dtor (&old);
@@@

/*
 * Unary-ops
 */
assign_expr_unary_op (token LHS, node RHS, string OP_FN)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   \read_rvalue ("rhs", RHS);
   if (in_copy_on_write (*p_lhs))
   {
     zval_ptr_dtor (p_lhs);
     ALLOC_INIT_ZVAL (*p_lhs);
   }

   zval old = **p_lhs;
   int result_is_operand = (*p_lhs == rhs);
   $OP_FN (*p_lhs, rhs TSRMLS_CC);
   if (!result_is_operand)
	zval_dtor (&old);
@@@

/*
 * Pre-op
 */
pre_op (token VAR, string OP_FN)
@@@
   \get_st_entry ("LOCAL", "p_var", VAR);
   sep_copy_on_write (p_var);
   $OP_FN (*p_var);
@@@

/*
 * Return
 */

return (token RETVAL, node RET)
   where RET.return_by_ref
@@@
   \get_st_entry ("LOCAL", "p_rhs", RETVAL);
   sep_copy_on_write (p_rhs);
   zval_ptr_dtor (return_value_ptr);
   (*p_rhs)->is_ref = 1;
   (*p_rhs)->refcount++;
   *return_value_ptr = *p_rhs;
   goto end_of_function;
@@@

// Not return-by-ref
return (token RETVAL, node RET)
@@@
   \read_rvalue ("rhs", RETVAL)
   // Run-time return by reference has different semantics to compile-time.
   // If the function has CTRBR and RTRBR, the the assignment will be
   // reference. If one or the other is return-by-copy, the result will be
   // by copy. Its a question of whether its separated at return-time (which
   // we do here) or at the call-site.
   return_value->value = rhs->value;
   return_value->type = rhs->type;
   zval_copy_ctor (return_value);
   goto end_of_function;
@@@


/*
 * Branch
 */
branch (token COND, string TRUE_TARGET, string FALSE_TARGET)
@@@
   \read_rvalue ("p_cond", COND);
   zend_bool bcond = zend_is_true (p_cond);
   if (bcond)
      goto $TRUE_TARGET;
   else
      goto $FALSE_TARGET;
@@@

   

/*
 * Var-vars
 */
assign_expr_var_var (token LHS, token INDEX)
@@@
  \get_st_entry ("LOCAL", "p_lhs", LHS);
  \read_rvalue ("index", INDEX);
  zval* rhs;
  \read_var_var ("rhs", "index");
  if (*p_lhs != rhs)
    {
      \write_var ("p_lhs", "rhs", LHS, INDEX);
    }
@@@

assign_expr_ref_var_var (token LHS, token INDEX)
@@@
  \get_st_entry ("LOCAL", "p_lhs", LHS);
  \read_rvalue ("index", INDEX);
  zval** p_rhs;
  \get_var_var ("LOCAL", "p_rhs", "index");
  sep_copy_on_write (p_rhs);
  copy_into_ref (p_lhs, p_rhs);
@@@

/*
 * Array access
 */
assign_expr_array_access (token LHS, token ARRAY, token INDEX)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   \read_rvalue ("r_array", ARRAY);
   \read_rvalue ("r_index", INDEX);

   zval* rhs;
   int is_rhs_new = 0;
    if (Z_TYPE_P (r_array) != IS_ARRAY)
    {
      if (Z_TYPE_P (r_array) == IS_STRING)
	{
	  is_rhs_new = 1;
	  rhs = read_string_index (r_array, r_index TSRMLS_CC);
	}
      else
	// TODO: warning here?
	rhs = EG (uninitialized_zval_ptr);
    }
    else
    {
      if (check_array_index_type (r_index TSRMLS_CC))
	{
	  // Read array variable
	  read_array (&rhs, r_array, r_index TSRMLS_CC);
	}
      else
	rhs = *p_lhs; // HACK to fail  *p_lhs != rhs
    }

   if (*p_lhs != rhs)
      write_var (p_lhs, rhs);

   if (is_rhs_new) zval_ptr_dtor (&rhs);
@@@

assign_expr_ref_array_access (token LHS, token ARRAY, token INDEX)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   \get_st_entry ("LOCAL", "p_r_array", ARRAY);
   \read_rvalue ("r_index", INDEX);
   check_array_type (p_r_array TSRMLS_CC);
   zval** p_rhs = get_ht_entry (p_r_array, r_index TSRMLS_CC);
   sep_copy_on_write (p_rhs);
   copy_into_ref (p_lhs, p_rhs);
@@@

/*
 * Constants
 */
assign_expr_constant (token LHS, string CONSTANT)
@@@
   // No null-terminator in length for get_constant.
   // zend_get_constant always returns a copy of the constant.
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   if (!(*p_lhs)->is_ref)
   {
     zval_ptr_dtor (p_lhs);
     get_constant ("$CONSTANT", \cb:length(CONSTANT), p_lhs TSRMLS_CC);
   }
   else
   {
     zval* constant;
     get_constant ("$CONSTANT", \cb:length(CONSTANT), p_lhs TSRMLS_CC);
     overwrite_lhs_no_copy (*p_lhs, constant);
     safe_free_zval_ptr (constant);
   }
@@@



/*
 * SCOPE: LOCAL/GLOBAL
 * NAME: A name for the zvpp.
 * VAR: Attribute map
 */
get_st_entry (string SCOPE, string ZVP, token VAR)
   where SCOPE == "LOCAL"
   where VAR.st_entry_not_required
@@@
  if (local_$VAR == NULL)
    {
      local_$VAR = EG (uninitialized_zval_ptr);
      local_$VAR->refcount++;
    }
  zval** $ZVP = &local_$VAR;
@@@

// TODO: inline better
get_st_entry (string SCOPE, string ZVP, token VAR)
@@@
  zval** $ZVP = get_st_entry (\scope(SCOPE), "$VAR", \cb:length(VAR) + 1, \cb:hash(VAR) TSRMLS_CC);
@@@

scope (string SCOPE) where SCOPE == "LOCAL" @@@EG(active_symbol_table)@@@
scope (string SCOPE) where SCOPE == "GLOBAL" @@@&EG(symbol_table)@@@


/*
 * read_value
 */

// For literals
read_rvalue (string ZVP, node LIT)
   where \cb:is_literal(LIT) == "TRUE"
   where LIT.pool_name
@@@
  zval* $ZVP = ${LIT.pool_name};
@@@

read_rvalue (string ZVP, node LIT)
   where \cb:is_literal(LIT) == "TRUE"
@@@
  zval* lit_tmp_$ZVP;
  INIT_ZVAL (lit_tmp_$ZVP);
  zval* $ZVP = &lit_tmp_$ZVP;
  \cb:write_literal_directly_into_zval (ZVP, LIT);
@@@

// Not for literals (not that the signature changes here - thats intentional)
read_rvalue (string ZVP, token VAR)
   where VAR.st_entry_not_required
   where VAR.is_uninitialized
@@@
  zval* $ZVP = EG (uninitialized_zval_ptr);
@@@

read_rvalue (string ZVP, token VAR)
   where VAR.st_entry_not_required
   where VAR.is_initialized
@@@
  zval* $ZVP = local_$VAR;
@@@

read_rvalue (string ZVP, token VAR)
   where VAR.st_entry_not_required
@@@
  zval* $ZVP;
  if (local_$VAR == NULL)
    $ZVP = EG (uninitialized_zval_ptr);
  else
    $ZVP = local_$VAR;
@@@

read_rvalue (string ZVP, token TVAR)
@@@
  zval* $ZVP = read_var (\scope("LOCAL");, "$TVAR", \cb:length(TVAR) + 1, \cb:hash(TVAR) TSRMLS_CC);
@@@


/*
 * write_var
 */

write_var (string LHS, string RHS, token TLHS, token TRHS)
   where TLHS.is_uninitialized
@@@
  \write_var_inner (LHS, RHS, TLHS, TRHS);
@@@

write_var (string LHS, string RHS, token TLHS, token TRHS)
@@@
  if ((*$LHS)->is_ref)
      overwrite_lhs (*$LHS, $RHS);
  else
    {
      zval_ptr_dtor ($LHS);
      \write_var_inner (LHS, RHS, TLHS, TRHS);
    }
@@@

write_var_inner (string LHS, string RHS, token TLHS, token TRHS)
   where TRHS.is_uninitialized
@@@
  // Share a copy
  $RHS->refcount++;
  *$LHS = $RHS;
@@@


write_var_inner (string LHS, string RHS, token TLHS, token TRHS)
@@@
  if ($RHS->is_ref)
    {
      // Take a copy of RHS for LHS
      *$LHS = zvp_clone_ex ($RHS);
    }
  else
    {
      // Share a copy
      $RHS->refcount++;
      *$LHS = $RHS;
    }
@@@

/*
 * Var-vars
 */

read_var_var (string ZVP, string INDEX)
@@@
   $ZVP = read_var_var (\scope("LOCAL"), $INDEX TSRMLS_CC);
@@@

get_var_var (string ST, string ZVP, string INDEX)
@@@
   $ZVP = get_var_var (\scope(ST), $INDEX TSRMLS_CC);
@@@

assign_var_var (token INDEX, node RHS)
@@@
   zval** p_lhs;
   \read_rvalue ("index", INDEX);
   \get_var_var ("LOCAL", "p_lhs", "index");
   \read_rvalue ("rhs", RHS);
   if (*p_lhs != rhs)
   {
      // TODO: we dont have node for p_lhs to call \write_var
      write_var (p_lhs, rhs);
   }
@@@

assign_var_var_ref (token INDEX, node RHS)
@@@
   zval** p_lhs;
   \read_rvalue ("index", INDEX);
   \get_var_var ("LOCAL", "p_lhs", "index");
   \get_st_entry ("LOCAL", "p_rhs", RHS);
   sep_copy_on_write (p_rhs);
   copy_into_ref (p_lhs, p_rhs);
@@@

/*
 * Assign_next
 * TODO: These are are more than a bit of a mess.
 */
assign_next (token LHS, token RHS)
@@@
   \get_st_entry ("LOCAL", "p_array", LHS);
   // Push EG(uninit) and get a pointer to the symtable entry
   zval** p_lhs = push_and_index_ht (p_array TSRMLS_CC);
   if (p_lhs != NULL)
   {
      \read_rvalue ("rhs", RHS);
      if (*p_lhs != rhs)
	 write_var (p_lhs, rhs);
   }
   // I think if this is NULL, then the LHS is a bool or similar, and you cant
   // push onto it.
@@@

assign_next_ref (token LHS, token RHS)
@@@
   \get_st_entry ("LOCAL", "p_array", LHS);
   // Push EG(uninit) and get a pointer to the symtable entry
   zval** p_lhs = push_and_index_ht (p_array TSRMLS_CC);
   if (p_lhs != NULL)
   {
      // TODO: this is wrong (further note, not sure why, I wrote that ages ago - pb)
      \get_st_entry ("LOCAL", "p_rhs", RHS);
      sep_copy_on_write (p_rhs);
      copy_into_ref (p_lhs, p_rhs);
   }
@@@


/*
 * Assign_array
 */

assign_array (token ARRAY, token INDEX, token RHS)
@@@
   \get_st_entry ("LOCAL", "p_array", ARRAY);
   check_array_type (p_array TSRMLS_CC);

   \read_rvalue ("index", INDEX);

   // String indexing
   if (Z_TYPE_PP (p_array) == IS_STRING && Z_STRLEN_PP (p_array) > 0)
   {
      \read_rvalue ("rhs", RHS);
      write_string_index (p_array, index, rhs TSRMLS_CC);
   }
   else if (Z_TYPE_PP (p_array) == IS_ARRAY)
   {
      zval** p_lhs = get_ht_entry (p_array, index TSRMLS_CC);
      \read_rvalue ("rhs", RHS);
      if (*p_lhs != rhs)
      {
	 write_var (p_lhs, rhs);
      }
   }
@@@

assign_array_ref (token ARRAY, token INDEX, token RHS)
@@@
   \get_st_entry ("LOCAL", "p_array", ARRAY);
   check_array_type (p_array TSRMLS_CC);

   \read_rvalue ("index", INDEX);

   // String indexing
   if (Z_TYPE_PP (p_array) == IS_STRING && Z_STRLEN_PP (p_array) > 0)
   {
      php_error_docref (NULL TSRMLS_CC, E_ERROR,
		       "Cannot create references to/from string offsets nor overloaded objects");
   }
   else if (Z_TYPE_PP (p_array) == IS_ARRAY)
   {
      zval** p_lhs = get_ht_entry (p_array, index TSRMLS_CC);
      \get_st_entry ("LOCAL", "p_rhs", RHS);
      sep_copy_on_write (p_rhs);
      copy_into_ref (p_lhs, p_rhs);
   }
@@@



/*
 * Foreach
 */

// TODO: Find a nice way to avoid this duplication
assign_expr_ref_foreach_get_val (token LHS, token ARRAY, string ITERATOR)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   // TODO: we know this is an array
   \read_rvalue ("fe_array", ARRAY);

   zval** p_rhs = NULL;
   int result = zend_hash_get_current_data_ex (
					       fe_array->value.ht,
					       (void**)(&p_rhs),
					       &$ITERATOR);
   assert (result == SUCCESS);

   sep_copy_on_write (p_rhs);
   copy_into_ref (p_lhs, p_rhs);
@@@

assign_expr_foreach_get_val (token LHS, token ARRAY, string ITERATOR)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   // TODO: we know this is an array
   \read_rvalue ("fe_array", ARRAY);

   zval** p_rhs = NULL;
   int result = zend_hash_get_current_data_ex (
					       fe_array->value.ht,
					       (void**)(&p_rhs),
					       &$ITERATOR);
   assert (result == SUCCESS);

   if (*p_lhs != *p_rhs)
      write_var (p_lhs, *p_rhs);
@@@

assign_expr_foreach_has_key (token LHS, token ARRAY, string ITERATOR)
@@@
   \new_lhs (LHS, "value");
   \read_rvalue ("fe_array", ARRAY);
   int type = zend_hash_get_current_key_type_ex (fe_array->value.ht, &$ITERATOR);
   ZVAL_BOOL (value, type != HASH_KEY_NON_EXISTANT);
@@@

assign_expr_foreach_get_key (token LHS, token ARRAY, string ITERATOR)
@@@
   \new_lhs (LHS, "value");
   \read_rvalue ("fe_array", ARRAY);

   char* str_index = NULL;
   uint str_length;
   ulong num_index;

   int result = zend_hash_get_current_key_ex (fe_array->value.ht, &str_index,
					      &str_length, &num_index, 0,
					      &$ITERATOR);
   if (result == HASH_KEY_IS_LONG)
   {
      ZVAL_LONG (value, num_index);
   }
   else
   {
      ZVAL_STRINGL (value, str_index, str_length - 1, 1);
   }
@@@

foreach_reset (token ARRAY, string ITERATOR)
@@@
   \read_rvalue ("fe_array", ARRAY);
   zend_hash_internal_pointer_reset_ex (fe_array->value.ht, &$ITERATOR);
@@@

foreach_next (token ARRAY, string ITERATOR)
@@@
   \read_rvalue ("fe_array", ARRAY);
   int result = zend_hash_move_forward_ex (fe_array->value.ht, &$ITERATOR);
   assert (result == SUCCESS);
@@@

foreach_end (token ARRAY, string ITERATOR)
@@@
   \read_rvalue ("fe_array", ARRAY);
   zend_hash_internal_pointer_end_ex (fe_array->value.ht, &$ITERATOR);
@@@

/*
 * Lots of macros need to fetch the LHS, initialize/separate it, and add a
 * value. In Generate_C, this used to be Pattern_assign_var, but now they just
 * call this macro.
 */
new_lhs (token LHS, string VAL)
@@@
   \get_st_entry ("LOCAL", "p_lhs", LHS);
   zval* $VAL;
   if ((*p_lhs)->is_ref)
   {
     // Always overwrite the current value
     $VAL = *p_lhs;
     zval_dtor ($VAL);
   }
   else
   {
     ALLOC_INIT_ZVAL ($VAL);
     zval_ptr_dtor (p_lhs);
     *p_lhs = $VAL;
   }
@@@


