<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Test the output of a compiled script against php's output for the same script. 
 */


array_push($tests, new CompiledVsInterpreted ());
class CompiledVsInterpreted extends Test
{
	function check_prerequisites ()
	{
		global $gcc, $libphp;
#		if (!check_for_program ($gcc)) return false;
		if (!check_for_program ("$libphp/libphp5.so")) return false;
		return true;
	}

	function get_test_subjects ()
	{
		return get_interpretable_scripts ();
	}

	function run_test ($subject)
	{
		global $gcc, $phc, $php;
		$dir_name = dirname($subject);

		// get the output from the interpreter for the file
		$command1 = get_php_command_line ($subject);
		list ($out1, $err1, $exit1) = complete_exec($command1);
		$out1 = $this->homogenize_output ($out1); 

		// first check that phc returns 0
		$phc_command1 = "$phc --generate-c $subject";
		list ($phc_out1, $phc_err1, $phc_exit1) = complete_exec($phc_command1);
		if ($phc_exit1 != 0)
		{
			$this->mark_skipped($subject, $phc_out1); 
			return;
		}

		// generate C
		$phc_command2 = "$phc -c $subject";
		list ($phc_out2, $phc_err2, $phc_exit2) = complete_exec($phc_command2);
		if ($phc_exit2 or $phc_err2)
		{
			$this->mark_failure ($subject,
				array($command1, $phc_command1, $phc_command2), 
				array($exit1, $phc_exit1, $phc_exit2),
				$phc_out2,
				array($err1, $phc_err1, $phc_err2));
			return;
		}

		// run the program
		$command2 = " ./a.out";
		list ($out2, $err2, $exit2) = complete_exec($command2);
		$out2 = $this->homogenize_output ($out2); 

		if ($out1 !== $out2 
			or $err1 !== $err2 
			or $exit1 
			or $exit2)
		{
			$output = diff ($out1, $out2);
			$this->mark_failure ($subject,
				array($command1, $phc_command1, $phc_command2, $command2),
				array($exit1, $phc_exit1, $phc_exit2, $exit2),
				$output,
				array ($err1, $phc_err1, $phc_err2, $err2));
		}
		else
		{
			$this->mark_success ($subject);
		}
	}
}

?>
