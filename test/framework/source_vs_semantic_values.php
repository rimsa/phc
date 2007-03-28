<?php
/*
 * phc -- the open source PHP compiler
 * See doc/license/README.license for licensing information
 *
 * Compares phc's internal values with the source representations of them
 */

array_push($tests, new SourceVsSemanticTest ());
class SourceVsSemanticTest extends PluginTest
{
	function __construct ()
	{
		parent::__construct("source_vs_semantic_values");
	}

	function get_command_line ($subject)
	{
		global $phc, $php;
		return "$phc --run plugins/tests/source_vs_semantic_values.la $subject 2>&1 | $php -d memory_limit=25M 2>&1";
	}
	
}


