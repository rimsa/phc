<?php
	function f(&$arg)
	{
		debug_zval_dump(&$arg);
		$arg = 6;
	}

	$a = 5;
	$b = &$a;
	f($b);

	debug_zval_dump($a);
?>
