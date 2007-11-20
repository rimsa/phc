<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Check that all scripts parse, with expected errors taken into account.
 */

// This is a regression test to check that all the scripts parse as we
// expect them to
require_once ("lib/async_test.php");
class BasicParseTest extends AsyncTest
{
	function get_test_subjects ()
	{
		return get_all_scripts();
	}

	function run_test ($subject)
	{
		global $phc;
		$bundle = new AsyncBundle ($this, $subject);

		$bundle->expected = $this->get_expected ($subject);

		$command = "$phc ";

		// add subject
		if (empty ($bundle->expected["no_subject"]))
			$command .= " $subject";

		// add options
		if (isset ($bundle->expected["options"]))
			$command .= " ". $bundle->expected["options"];

		$bundle->commands[0] = $command;
		$bundle->final = "finish";
		$bundle->start ();
	}

	function finish ($bundle)
	{
		$expected = $bundle->expected;
		$failed = false;

		// check exit code
		if (isset ($expected["exit_code"]))
		{
			if ($bundle->exits[0] == $expected["exit_code"])
			{
				$this->mark_success ($bundle->subject);
				$this->expected_failure_count++;
				write_dependencies ($this->get_name (), $bundle->subject, false);
				return;
			}
			else
			{
				$this->mark_failure ("Exit doesnt match", $bundle);
				return;
			}
		}
		else if ($bundle->exits[0])
		{
			$this->mark_failure ("Unexpected exit", $bundle);
			return;
		}


		// check error
		if (isset ($expected["err_regex"]))
		{
			$err = $bundle->errs[0];
			preg_replace ("!{$expected["err_regex"]}!", "", $err);
			if ($err !== "")
			{
				$this->mark_failure ("Error doesnt match: ". $expected["err_regex"], $bundle);
				return;
			}
		}

		// check output
		if (isset ($expected["out_regex"]))
		{
			preg_replace ("!{$expected["out_regex"]}!", "", $bundle->exits[0]);
			if ($bundle->outs [0] !== "")
			{
				$this->mark_failure ("output doesnt match", $bundle);
				return;
			}
		}

		$this->mark_success ($bundle->subject);
	}

	// The format of the brace info is 
	//		{ options :: output_regex :: err_regex :: exit_code :: no subject }
	//	or { exact_error }

	/* Get a list of expected results from the file
	 *		These should be instances of class to check.
	 *		There should only be 1 per file.
	 */
	function get_expected ($subject)
	{
		$out = file_get_contents ($subject);

		// match the errors
		$lines = split ("\n", $out);
		$pregs = array ("/#(.*)$/", "/\/\/(.*)$/", "/\/\*(.*)\*\//");
		foreach ($lines as $line)
		{
			// get single line comments
			foreach ($pregs as $preg)
			{
				if (preg_match ("$preg", $line, $line_match))
				{
					// extract the error/warning from the comment
					if (preg_match ("/\s*\{\s*(\S.*\S)\s*\}\s*$/", $line_match[1], $comment_match))
					{

						if (isset ($full_expected))
						{
							echo "Tests should have only 1 warning/error etc per file\n";
							assert (0);
						}
						$full_expected = $comment_match[1];
					}
				}
			}
		}

		if (empty ($full_expected))
			return NULL;

		// allow braces which just contain the error
		if (strpos ("::", $full_expected) === FALSE)
		{
			$expected = array ("", "", "[^:]\d+\s$full_expected", "255", "");
		}
		else
		{
		$expected = split ("::", $full_expected);
		}
		array_map ("rtrim", $expected);
		array_map ("trim", $expected);

		//	{ options :: output_regex :: err_regex :: exit_code :: no subject }
		return array_combine (array ("options", "output_regex", "err_regex", "exit_code", "no_subject"), $expected);
	}

	# we override tests run in order to add a line at the end
	function run ()
	{
		parent::run ();
		$num_skipped = $this->expected_failure_count;
		echo "($num_skipped expected errors)\n";
	}
}
array_push($tests, new BasicParseTest());

?>
