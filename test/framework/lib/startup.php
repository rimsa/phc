<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Handle command line parameters
 */

require_once ("Console/Getopt.php");
require_once ("Console/ProgressBar.php");

$cg = new Console_Getopt();
list ($opts, $arguments) = $cg->getOpt($cg->readPHPArgv(), "lvVshndpi", array ("long", "verbose", "valgrind", "support", "help", "number", "no-delete", "no-progress"));
foreach ($opts as $opt) 
{
	if ($opt[1] == NULL) $options{$opt[0]} = "";
	else $options{$opt[0]} = ($opt[1]);
}
$opt_verbose = isset($options{"v"});
$opt_long = isset($options{"l"});
$opt_valgrind = isset($options{"V"});
$opt_support = isset($options{"s"});
$opt_numbered = isset($options{"n"});
$opt_help = isset($options{"h"});
$opt_no_delete = (isset($options{"d"})) or $opt_numbered;
$opt_no_progress_bar = isset($options{"p"});
$opt_installed = isset($options{"i"});

if ($opt_help)
{
	die (<<<EOL
driver.php - Front-end to the phc test framework - phpcompiler.org

Usage: php driver.php [OPTIONS] [regex]
    Note that [OPTIONS] cannot come after [regex]

Options:
    -h     Print this help message
    -V     Run phc through valgrind
    -s     Generate support files (mostly for regression tests)
    -v     Print verbose messages after test failure
    -l     Run long tests (default: run only tests marked short in labels file)
    -n     Print a numbered list of the files to be processed (implies -d)
    -d     Don't delete test logs before running
    -p     Don't use a progress bar (useful for nightly testing)
    -i     Use installed program and plugins for tests

Regex:
    A list of regular expressions matching the tests to be run. Any test name
    matching a regex in the list will be allowed run. By default, all tests
    will be run.

EOL
	);
}


?>
