<?php
	$x[0];
	$x[0][0];
	$x[];
	$x[][];
	$x[0][];
	$x[][0];

	${$x[0]};

	array('a');
	array(&$x);
	array(1 => 'a');
	array(1 => &$x); 
	
	array('a', 'b', 'c');
	array(&$x, &$y, &$y);
	array(1 => 'a', 2 => 'b', 3 => 'c');
	array(1 => &$x, 2 => &$x, 3 => &$x);
?>
