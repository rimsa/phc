<?php
	$a = 5;
	$b = $a;
	$c = 6;
	$b = $c;
	debug_zval_dump($a);
	debug_zval_dump($b);
	debug_zval_dump($c);
?>
