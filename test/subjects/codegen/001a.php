<?php
	$a = 5;
	$b = $a;
	$b = $b + 1;
	debug_zval_dump($a);
	debug_zval_dump($b);
?>
