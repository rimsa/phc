<?php

foreach (array (true, false) as $a)
{
	foreach (array (true, false) as $b)
	{
		foreach (array (true, false) as $c)
		{
			foreach (array (true, false) as $d)
			{
				var_dump ($a || $b || $c || $d);
				var_dump ($a || $b || $c && $d);
				var_dump ($a || $b && $c || $d);
				var_dump ($a || $b && $c && $d);
				var_dump ($a && $b || $c || $d);
				var_dump ($a && $b || $c && $d);
				var_dump ($a && $b && $c || $d);
				var_dump ($a && $b && $c && $d);
			}
		}
	}
}

# all 4 bit permutations
foreach (range (0, 15) as $a)
{
	foreach (range (0, 15) as $b)
	{
		foreach (range (0, 15) as $c)
		{
			foreach (range (0, 15) as $d)
			{
				# trinary :)
				var_dump ($a | $b | $c | $d);
				var_dump ($a | $b | $c & $d);
				var_dump ($a | $b | $c ^ $d);

				var_dump ($a | $b & $c | $d);
				var_dump ($a | $b & $c & $d);
				var_dump ($a | $b & $c ^ $d);
				
				var_dump ($a | $b ^ $c | $d);
				var_dump ($a | $b ^ $c & $d);
				var_dump ($a | $b ^ $c ^ $d);



				var_dump ($a & $b | $c | $d);
				var_dump ($a & $b | $c & $d);
				var_dump ($a & $b | $c ^ $d);

				var_dump ($a & $b & $c | $d);
				var_dump ($a & $b & $c & $d);
				var_dump ($a & $b & $c ^ $d);
				
				var_dump ($a & $b ^ $c | $d);
				var_dump ($a & $b ^ $c & $d);
				var_dump ($a & $b ^ $c ^ $d);



				var_dump ($a ^ $b | $c | $d);
				var_dump ($a ^ $b | $c & $d);
				var_dump ($a ^ $b | $c ^ $d);

				var_dump ($a ^ $b & $c | $d);
				var_dump ($a ^ $b & $c & $d);
				var_dump ($a ^ $b & $c ^ $d);

				var_dump ($a ^ $b ^ $c | $d);
				var_dump ($a ^ $b ^ $c & $d);
				var_dump ($a ^ $b ^ $c ^ $d);
			}
		}
	}
}

?>
