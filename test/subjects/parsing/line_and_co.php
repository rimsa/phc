<?php
	$x = __LINE__;
	$x = __FILE__;
	$x = __FUNCTION__;
	$x = __METHOD__;
	$x = __CLASS__;

	function some_function()
	{
		$x = __LINE__;
		$x = __FILE__;
		$x = __FUNCTION__;
		$x = __METHOD__;
		$x = __CLASS__;
	}

	class SOME_CLASS
	{
		public function some_method() 
		{
			$x = __LINE__;
			$x = __FILE__;
			$x = __FUNCTION__;
			$x = __METHOD__;
			$x = __CLASS__;
		}
	}
	
	$x = __LINE__;
	$x = __FILE__;
	$x = __FUNCTION__;
	$x = __METHOD__;
	$x = __CLASS__;
?>
