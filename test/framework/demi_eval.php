<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Adds debug_zval_dump after each statement, then compared the outputs of the
 * compiled and interpreted code.
 */


array_push($tests, new Demi_eval ("true"));
array_push($tests, new Demi_eval ("false"));
class Demi_eval extends CompiledVsInterpreted
{
	function __construct ($init)
	{
		$this->init = $init;
		parent::__construct ();
	}

	function get_dependent_test_names ()
	{
		return array ("CompiledVsInterpreted");
	}

	function get_name ()
	{
		$init = $this->init;
		return "Demi_eval_$init";
	}

	function get_php_command ($subject)
	{
		global $phc;
		$init = $this->init;
		return "$phc --run plugins/tools/demi_eval.la --r-option=\"$init\" --dump-uppered=plugins/tools/demi_eval.la $subject | ". get_php_command_line ("");
	}

	function get_phc_command ($subject, $exe_name)
	{
		global $phc;
		$init = $this->init;
		return "$phc -c --run plugins/tools/demi_eval.la --r-option=\"$init\" $subject -o $exe_name";
	}
}

?>
