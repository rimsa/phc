<?php
	// Simple write to a class attribute

	eval('$c = new StdClass();');
	$c->a = 1;
	var_dump($c);
?>
