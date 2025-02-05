<?php
   $generator = (PHP_OS_FAMILY == 'Windows' ? 'generator.exe' : './generator');
   
   $n = 2;           // number of strings to generate for each (size, sigma, dtype) tuple
   $size = 50000;
   $dtypes = [ 'linear', 'binomial' ];
   
   foreach ([ 4, 8, 16, 32, 64, 128, 256 ] as $sigma) {
      foreach ($dtypes as $dtype) {
         file_put_contents('temporary_input.txt', "$n $size $sigma $dtype\n"); 
         system("$generator < temporary_input.txt");
      }
   }
   unlink('temporary_input.txt');
   