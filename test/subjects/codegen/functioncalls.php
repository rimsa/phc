<?php
/*
 * Test of function calls
 *
 * We define two functions fun and fun_r, both of which take a single argument
 * $x; but fun_r is compile-time-by-reference and fun is not.
 *
 * Tests are designed to check copy-on-write etc. stuff
 *
 * To be able to test function invocation separate from the implementation
 * of functions, we duplicate the tests, once for functions that have been
 * eval'ed, and once for functions that we have compiled.
 */

	eval('function e_fun($x) { $x = \'x\'; }');
	eval('function e_fun_r(&$x) { $x = \'x\'; }');

	function fun($x)
	{
		$x = 'x'; 
	}

	function fun_r(&$x)
	{
		$x = 'x'; 
	}

/*
 * Using eval'ed functions
 */
	
	// Test proper calling conventions in simple cases

	$a = 10;
	e_fun($a);
	var_export($a);

	$b = 20;
	e_fun(&$b);
	var_export($b);

	$c = 30;
	e_fun_r($c);
	var_export($c);

	$d = 40;
	e_fun_r(&$d);
	var_export($d);

	// Same as before, but the variable now passed in is part of a
	// copy-on-write set. In all cases where a pass-by-reference is involved,
	// only one of the two variables should be changed.

	$e = 50;
	$f = $e;
	e_fun($f);
	var_export($e);
	var_export($f);

	$g = 60;
	$h = $g;
	e_fun(&$h);
	var_export($g);
	var_export($h);

	$i = 70;
	$j = $i;
	e_fun_r($j);
	var_export($i);
	var_export($j);

	$k = 80;
	$l = $k;
	e_fun_r(&$l);
	var_export($k);
	var_export($l);

	// Same again, but the variable passed in is now part of a 
	// change-on-write set (references another variable). 
	// In this section, in all cases where a pass-by-reference is involved,
	// both variables should be changed.

	$m = 90;
	$n =& $m;
	e_fun($n);
	var_export($m);
	var_export($n);

	$o = 100;
	$p =& $o;
	e_fun(&$p);
	var_export($o);
	var_export($p);

	$q = 110;
	$r =& $q;
	e_fun_r($r);
	var_export($q);
	var_export($r);

	$s = 120;
	$t =& $s;
	e_fun_r(&$t);
	var_export($s);
	var_export($t);

/*
 * Using compiled functions
 */

	// Test proper calling conventions in simple cases

	$a = 10;
	fun($a);
	var_export($a);

	$b = 20;
	fun(&$b);
	var_export($b);

	$c = 30;
	fun_r($c);
	var_export($c);

	$d = 40;
	fun_r(&$d);
	var_export($d);

	// Same as before, but the variable now passed in is part of a
	// copy-on-write set. In all cases where a pass-by-reference is involved,
	// only one of the two variables should be changed.

	$e = 50;
	$f = $e;
	fun($f);
	var_export($e);
	var_export($f);

	$g = 60;
	$h = $g;
	fun(&$h);
	var_export($g);
	var_export($h);

	$i = 70;
	$j = $i;
	fun_r($j);
	var_export($i);
	var_export($j);

	$k = 80;
	$l = $k;
	fun_r(&$l);
	var_export($k);
	var_export($l);

	// Same again, but the variable passed in is now part of a 
	// change-on-write set (references another variable). 
	// In this section, in all cases where a pass-by-reference is involved,
	// both variables should be changed.

	$m = 90;
	$n =& $m;
	fun($n);
	var_export($m);
	var_export($n);

	$o = 100;
	$p =& $o;
	fun(&$p);
	var_export($o);
	var_export($p);

	$q = 110;
	$r =& $q;
	fun_r($r);
	var_export($q);
	var_export($r);

	$s = 120;
	$t =& $s;
	fun_r(&$t);
	var_export($s);
	var_export($t);
?>
