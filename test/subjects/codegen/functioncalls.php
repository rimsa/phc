<?php
/*
 * Test of function calls
 *
 * We define two functions f and g, both of which take a single argument $x;
 * but g is compile-time-by-reference and f is not.
 *
 * Tests are designed to check copy-on-write etc. stuff
 */

	function f($x)
	{
		$x = $x + 1;
	}

	function g(&$x)
	{
		$x = $x + 1;
	}

	// Test proper calling conventions in simple cases

	$a = 10;
	f($a);
	var_export($a);

	$b = 20;
	f(&$b);
	var_export($b);

	$c = 30;
	g($c);
	var_export($c);

	$d = 40;
	g(&$d);
	var_export($d);

	// Same as before, but the variable now passed in is part of a
	// copy-on-write set. In all cases where a pass-by-reference is involved,
	// only one of the two variables should be changed.

	$e = 50;
	$f = $e;
	f($f);
	var_export($e);
	var_export($f);

	$g = 60;
	$h = $g;
	f(&$h);
	var_export($g);
	var_export($h);

	$i = 70;
	$j = $i;
	g($j);
	var_export($i);
	var_export($j);

	$k = 80;
	$l = $k;
	g(&$l);
	var_export($k);
	var_export($l);

	// Same again, but the variable passed in is now part of a 
	// change-on-write set (references another variable). 
	// In this section, in all cases where a pass-by-reference is involved,
	// both variables should be changed.

	$m = 90;
	$n =& $m;
	f($n);
	var_export($m);
	var_export($n);

	$o = 100;
	$p =& $o;
	f(&$p);
	var_export($o);
	var_export($p);

	$q = 110;
	$r =& $q;
	g($r);
	var_export($q);
	var_export($r);

	$s = 120;
	$t =& $s;
	g(&$t);
	var_export($s);
	var_export($t);
?>
