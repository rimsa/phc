<?php
	function f()
	{
		global $x;
		$x = 5;
	}

	f();
	var_dump($x);
?>
