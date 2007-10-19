<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Test the output of a compiled script against php's output for the same script. 
 */

require_once ("lib/async_test.php");

class CompiledVsInterpreted extends AsyncTest
{
	function check_prerequisites ()
	{
		global $gcc, $libphp;
#		if (!check_for_program ($gcc)) return false;
		if (!check_for_program ("$libphp/libphp5.so")) return false;
		return true;
	}

	function get_dependent_test_names ()
	{
		return array ("Generate_C");
	}

	function get_test_subjects ()
	{
		return get_interpretable_scripts ();
	}

	function finish ($async)
	{
		if ($async->outs[0] !== $async->outs[2]
			or $async->errs[0] !== $async->errs[2]
			or $async->exits[0] !== $async->exits[2])
		{
			$output = diff ($async->outs[0], $async->outs[2]);
			$async->outs = $output;
			$this->mark_failure ("Outputs dont match PHP outputs", $async);
		}
		else
		{
			$this->mark_success ($async->subject);
		}

	}

	function homogenize_output ($string)
	{
		$string = homogenize_filenames_and_line_numbers ($string);
		$string = homogenize_reference_count ($string);
		return $string;
	}

	function handle_exit_failure ($exit, $async)
	{
		if ($exit != 0)
		{
			$this->mark_failure ($exit, $async);
			return false;
		}
		return $exit;
	}

	function handle_err_failure (&$err, $async)
	{
		if ($err != "")
		{
			$this->mark_failure ($err, $async);
			return false;
		}
		return $err;
	}

	function get_phc_command ($subject, $exe_name)
	{
		global $phc;
		return "$phc -c $subject -o $exe_name";
	}

	function run_test ($subject)
	{
		$async = new Async_steps ($this, $subject);

		$async->commands[0] = get_php_command_line ($subject);
		$async->out_handlers[0] = "homogenize_output";

		$exe_name = wd_name ("$subject.out");
		$async->commands[1] = $this->get_phc_command ($subject, $exe_name);
		$async->err_handlers[1] = "handle_err_failure";
		$async->exit_handlers[1] = "handle_exit_failure";

		$async->commands[2] = "$exe_name";
		$async->out_handlers[2] = "homogenize_output";

		$async->final = "finish";

		$async->start ();
	}
}
array_push($tests, new CompiledVsInterpreted ());

?>
