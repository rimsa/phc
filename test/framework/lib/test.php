<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Base class for tests
 */


function homogenize_xml ($string)
{
	$string = preg_replace("/(<attr key=\"phc.line_number\">)\d+(<\/attr>)/", "$1$2", $string);
	$string = preg_replace("/(<attr key=\"phc.filename\">).*?(<\/attr>)/", "$1$2", $string);
	return $string;
}

function homogenize_filenames_and_line_numbers ($string)
{
	$string = preg_replace( "/(Warning: .* in ).*( on line )\d+/", "$1$2", $string);
	$string = preg_replace( "/(Fatal error: .* in ).*( on line )\d+/", "$1$2", $string);
	$string = preg_replace( "/(Catchable fatal error: .* in ).*( on line )\d+/", "$1$2", $string);
	$string = preg_replace( "/(Fatal error: Cannot redeclare .*\(\) \(previously declared in ).*(:)\d+(\))/" , "$1$2$3", $string);
//	$string = preg_replace("/(Fatal error: Allowed memory size of )\d+( bytes exhausted at ).*( \(tried to allocate )\d+( bytes\) in ).*( on line )\d+/", "$1$2$3$4", $string);
	return $string;
}

function homogenize_break_levels ($string)
{
	$string = preg_replace(	"/Fatal error: Cannot break\/continue \d+ level(s)? in .*/",
									"Fatal error: Too many break/continue levels", $string);

# just clear these warnings
	$string = preg_replace(	"/Warning: Invalid argument supplied for foreach\(\) in/",
									"", $string);
	$string = preg_replace(	"/Warning: Variable passed to each\(\) is not an array or object in/",
									"", $string);
	return $string;
}

// its still correct if the number of references is off, so look for var_dump output, and remove &s from it
function homogenize_reference_count ($string)
{
# this only actually finds differences in arrays
	$string = preg_replace(
		"/
					(\s+\[.*?\]=>\s+)		# key and newline
					&							# we want to delete this
					(.*?\s+)					# dont go too far
		/smx", "$1$2", $string);
	return $string;
}

// Run COMMAND and check if the warnings and errors returned are expected, and that there arent any unexpected ones. This handles warnings and errors at compile time, not run time. Run-time errors use homogenize_x, above.
function run_command ($command, $subject)
{
	// check if we're expecting errors or warnings
	$lines = split ("\n", file_get_contents ($subject));
	$pregs = array ("/#(.*)$/", "/\/\/(.*)$/", "/\/\*(.*)\*\//");
	$error = false;
	$warnings = array ();
	foreach ($lines as $line)
	{
		// get single line comments
		foreach ($pregs as $preg)
		{
			if (preg_match ("$preg", $line, $line_match))
				// extract the error/warning from the comment
				if (preg_match ("/\s*\{\s*(\S.*\S)\s*\}\s*$/", $line_match[1], $comment_match))
				{
					if (preg_match ("/^Error:/", $comment_match[1]))
						$error = $comment_match[1];
					elseif (preg_match ("/^Warning:/", $comment_match[1]))
						$warnings[] = $comment_match[1];
				}
		}
	}

	// $matches now has a full list of expected errors
	$failure = "";
	list ($out, $err, $exit) = complete_exec ($command);

	// if the exit code was non-zero, expect an error and vice-versa
	if ($exit xor ($error !== false))
		$failure = "No exit code for expected error: $error";

	// if theres something in stderr, it should match a comment
	foreach (split ("\n", $err) as $line)
	{
		if ($line == "")
			continue;

		if ($line == "Note that line numbers are inaccurate, and will be fixed in a later release")
			continue;

		$original_line = $line;
		$line = preg_replace ("/[^:]*:\d+:\s/", "", $line);

		$found = false;
		foreach ($warnings as &$warning)
		{
			if ($line == $warning)
			{
				unset ($warning);
				$found = true;
			}
		}
		if ($line === $error)
		{
			$error = false;
			$found = true;
		}

		if ($found == false)
			$failure = "Unexpected error or warning: $original_line";
	}

	// every comment should be matched
	if ($error !== false)
		$failure = "Expected error not found: $error";

	if (count ($warnings))
		$failure = "Expected warning not found: $warnings[0]";

	return array ($out, $err, $exit, $failure);
}

abstract class Test
{
	function __construct ()
	{
		$this->successes = 0;
		$this->failures = 0;
		$this->skipped = 0;
		$this->total = 0;
		$this->timers = array();
	}

	function get_name ()
	{
		return get_class($this);
	}

	function ready_progress_bar ($num_files)
	{
		global $opt_no_progress_bar;
		$this->num_files = $num_files;
		if (!$opt_no_progress_bar)
		{
			// this line is irrelevent, since it's reset below 
			$this->progress_bar = new Console_ProgressBar (
				$this->get_name()."%bar% Testing (%fraction%)",
				"#", "-", 80, $num_files); 
		}
		$this->update_count(0);
	}

	function homogenize_output ($string)
	{
		return $string;
	}

	function check_global_prerequisites ()
	{
		global $php_exe;
		return check_for_program ($php_exe);
	}

	function check_prerequisites ()
	{
		return true;
	}

	function check_test_prerequisites ($subject)
	{
		return true;
	}

	function get_dependent_test_names ()
	{
		return array ();
	}

	// we dont want this to be overridden
	function get_builtin_dependent_test_names ()
	{
		if (!isset($this->dependencies))
			return array ();

		return (array) ($this->dependencies);
	}

	function passed_test_dependencies ($subject)
	{
		global $successes;

		$dependencies = $this->get_dependent_test_names () 
							+ $this->get_builtin_dependent_test_names ();
		foreach ($dependencies as $dependency)
		{
			if (!isset($successes[$dependency][$subject]))
			{
				return false;
			}
		}
		return true;
	}

	abstract function run_test ($subject);
	abstract function get_test_subjects ();

	function print_numbered ()
	{
		$files = $this->get_test_subjects();
		$i = 0;
		foreach ($files as $file)
		{
			print "$i. $file\n";
			$i++;
		}
	}

	function run()
	{
		// do this first so that the progress bar is ready
		$files = $this->get_test_subjects ();
		$this->ready_progress_bar (count ($files));


		if (!$this->check_prerequisites () || !$this->check_global_prerequisites ())
		{
			$this->mark_skipped ("All", "Test prerequisties failed");
			$this->finish_test ();
			return false;
		}

		foreach ($files as $subject)
		{
			if (!$this->check_test_prerequisites ($subject))
			{
				$this->mark_skipped ($subject, "Individual prerequsite failed");
			}
			elseif (!$this->passed_test_dependencies ($subject))
			{
				$this->mark_skipped ($subject, "Test failed dependency");
			}
			elseif ($this->check_exception ($subject))
			{
				$this->mark_skipped ($subject, "Test excepted");
			}
			else
			{
				$this->start_timer ($subject);
				$this->run_test ($subject);
				$this->end_timer ($subject);
			}
		}

		$this->finish_test ();
	}

	function start_timer ($subject)
	{
		$this->timer_start = microtime(true);
	}

	function end_timer ($subject)
	{
		$number = 0;
		$time = microtime (true) - $this->timer_start;
		$this->timers[$subject] = $time;
	}

	function get_timing_string ()
	{
		if (count ($this->timers) == 0) return "";
		$blue = blue_string();
		$red = red_string();
		$reset = reset_string ();
		asort($this->timers, SORT_NUMERIC);
		$numbers = array();

		// need the number to address it
		$i = 0;
		$files = $this->get_test_subjects ();
		foreach ($files as $file)
		{
			$numbers{$file} = $i;
			$i++;
		}

		// get the median (you cant just index with ints)
		$i = 0;
		foreach ($this->timers as $key => $value)
		{
			if ($i++ >= count($this->timers) / 2)
			{
				$med_val = $value;
				$med_key = $numbers{$key};
				break;
			}
		}

		// get the mean
		$total = 0;
		foreach ($this->timers as $key => $value)
		{
			$total += $value;
		}
		$mean = $total / count ($this->timers);

		// get the std dev
		$total = 0;
		foreach ($this->timers as $key => $value)
		{
			$dev = $value - $mean;
			$total += ($dev * $dev);
		}
		$std_dev = sqrt ($total / (count ($this->timers)));

		// get the max
		$max_val = array_pop(array_values($this->timers));
		$max_key = $numbers{array_pop(array_keys($this->timers))};

		// more than a second apart, and 2 std deviations above average
		if (($max_val > ($mean + 2*$std_dev))
				and ($max_val > $mean + 1))
		{
			return sprintf ("{$blue}avg %2ss; {$red}max%5s%3ss$reset", 
					floor($mean), "($max_key)", floor($max_val));
		}
		else
		{
			return sprintf ("{$blue}avg %2ss$reset              ", floor($mean));
		}

	}

	function mark_success ($subject)
	{
		# alert dependent tests to our success
		global $successes;
		$successes{$this->get_name ()}{$subject} = true;

		$this->successes++;
		$this->total++;
		$this->update_count ();
	}

	function mark_skipped ($subject, $reason)
	{
		if ($subject == "All")
		{
			$this->skipped += count($this->get_test_subjects ());
			$this->total = $this->skipped;
		}
		else
		{
			$this->skipped++;
			$this->total++;
		}
		note_in_skipped_file ($this->get_name(), $subject, $reason);
		$this->update_count ();
	}

	function mark_failure ($subject, $commands, $exits = "Not relevent", $out = "Not relevent", $errs = "Not relevent")
	{
		global $log_directory, $opt_verbose;
		$red = red_string();
		$reset = reset_string();
		if (is_array ($commands))
		{
			if ($exits == "Not relevent")
			{
				$command_string = "";
				foreach ($commands as $command)
				{
					$command_string .= "{$red}Command:$reset $command\n";
				}
			}
			else
			{
				phc_assert (is_array ($exits), "Expected array of return values");
				phc_assert (count ($exits) == count ($commands), 
					"Expected same number of commands and return values");
				$command_string = "";
				$err_string = "";
				foreach ($commands as $i => $command)
				{
					$exit = $exits[$i];
					$err = $errs [$i];
					$command_string .= "{$red}Command $i$reset ($exit): $command\n";
					if ($err)
						$err_string .= "{$red}Error $i$reset: $err\n";
				}
			}

		}
		else
		{
			$command_string = "{$red}Command$reset ($exits): $commands\n";
			$err_string = "";
		}

		$command_string .= $err_string;
		// dump it to a file
		$file_header = 
			"Failure in test $subject:\n".
			$command_string.
			"Output:\n"; // this is added below to avoid the massive memory usage

		if (is_array ($out))
		{
			$out = join ("\n", $out);
		}

		log_failure ($this->get_name(), $subject, $command_string, $out);
		$this->erase_progress_bar();

		$this->display_progress_bar ();

		$this->failures++;
		$this->total++;
		$this->update_count ();
	}

	function get_triple_string ($brackets = true)
	{
		$red = red_string ();
		$blue = blue_string ();
		$green = green_string ();
		$reset = reset_string ();
		$passed = sprintf ("%5s", $this->successes." P");
		$failed = sprintf ("%4s", $this->failures." F");
		$skipped = sprintf ("%4s", $this->skipped." S");
		if ($brackets == false)
		{
			if ($this->failures > 0)
				return "$red$passed, $failed, $skipped$reset";
			else 
				return "$green$passed, $failed, $skipped$reset";
		}
		else
			return "$green($passed$reset, $red$failed$reset, $blue$skipped)$reset";
	}

	function update_count ()
	{
		global $opt_no_progress_bar;
		if (! $opt_no_progress_bar)
		{
			# check for divide by zero
			$target_num = $this->num_files;
			if ($target_num == 0) $target_num = 1;

			$this->erase_progress_bar ();
			$this->progress_bar->reset(
					"{$this->get_name()} %bar% %fraction% done {$this->get_triple_string ()}", 
					"#", "-", 112, $target_num);
			$this->progress_bar->update($this->total);
		}
	}

	function erase_progress_bar ()
	{
		global $opt_no_progress_bar;
		if (! $opt_no_progress_bar)
		{
			$this->progress_bar->erase();
		}
	}

	function display_progress_bar ()
	{
		global $opt_no_progress_bar;
		if (! $opt_no_progress_bar)
		{
			$this->progress_bar->display($this->total);
		}
	}

	function finish_test ()
	{
		$this->erase_progress_bar();

		$red = red_string ();
		$blue = blue_string ();
		$green = green_string ();
		$reset = reset_string ();

		$test = $this->get_name();
		$timing = $this->get_timing_string ();
		$triple = $this->get_triple_string (false);

		if ($this->failures > 0 or $this->successes == 0)
			$word = "{$red}Failure:$reset";
		else
			$word = "{$green}Success:$reset";

		// a color or a reset involves 6 characters, but gets displayed as zero.
		// Those 6 need to be taken into account for sprintf
		$string = sprintf("%-29s %-21s %20s %28s", $test, $timing, $word, $triple);
		print "$string\n";
	}

	// return true if the subject is marked as an exception
	function check_exception ($subject)
	{
		global $exceptions;
		if (!isset($exceptions{$this->get_name()})) return false;
		$array = $exceptions{$this->get_name()};
		assert (is_array ($array));
		return (in_array ($subject, $array));
	}
}


?>
