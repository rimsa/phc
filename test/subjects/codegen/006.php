<?php
	eval("\$a = 5;");
	eval("\$b = \$a;");
	$b = $b + 1;
	var_dump($a);
	var_dump($b);
?>
