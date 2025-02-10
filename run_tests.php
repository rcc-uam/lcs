<?php
   $main = (PHP_OS_FAMILY == 'Windows' ? 'main.exe' : './main');
   $dirs = glob('tests/*');
   
   natsort($dirs);
   foreach ($dirs as $directory) {
      echo "*** Running $directory...\n";
      $files = glob("$directory/*");
      for ($i = 1; $i < count($files); ++$i) {
         $file_a = $files[0];
         $file_b = $files[$i];
         echo "***   $file_a VS $file_b\n";
         system("$main $file_a $file_b");
         echo "\n";
      }
      echo "\n";
   }
   