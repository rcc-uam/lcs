<?php
   $generator = (PHP_OS_FAMILY == 'Windows' ? 'generator.exe' : './generator');

   foreach ([ 5000 ] as $size) {
      foreach ([ 4, 8, 16, 32, 64, 128, 256 ] as $sigma) {
         foreach ([ 0, 25, 50, 75, 100 ] as $percentage) {
            file_put_contents('temporary_input.txt', "$size $sigma $percentage\n"); 
            system("$generator < temporary_input.txt");
         }
      }
   }
   unlink('temporary_input.txt');
   