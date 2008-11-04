<?php
/*
 * (c) 2003 John Lim    jlim#natsoft.com
 */

include_once("../init.inc.php");

// METADATA THAT IS READ BY ../bench.php:
//~~ Objects: Return by partial ref vs by value, Return by Partial Ref <i>$x = f()</i>, Return by Value

//================================================ INIT
class X {
	var $a=1;
	var $b=2;
	var $c=3;
	var $d=4;
	var $e=5;
	var $f=6;
	var $str1 = "The The The The The The The The";
	var $str2 = "The The The The The The The The";
	var $str3 = "The The The The The The The The";
	var $str4 = "The The The The The The The The";
	var $str5 = "The The The The The The The The";
	var $str6 = "The The The The The The The The";
	
	var $str11 = "The The The The The The The The";
	var $str12 = "The The The The The The The The";
	var $str13 = "The The The The The The The The";
	var $str14 = "The The The The The The The The";
	var $str15 = "The The The The The The The The";
	var $str16 = "The The The The The The The The";
}

$X = new X();

//================================================ TEST CODE

function &_b1()
{
global $X;

	return $X;
}

function _b2()
{
global $X;

	return $X;
}

function bench1() 
{
	$x = _b1();
}


function bench2() 
{
	$x = _b2();
}


//================================================ BENCHMARK!
include_once("../bench.inc.php");
?>