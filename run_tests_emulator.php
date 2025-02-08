<?php
   $main = (PHP_OS_FAMILY == 'Windows' ? 'main.exe' : './main');
   $dirs = glob('test_*');
   
   natsort($dirs);
   foreach ($dirs as $directory) {
      echo "*** Running $directory...\n";
      $files = glob("$directory/*");
      for ($i = 1; $i < count($files); ++$i) {
         $file_a = $files[0];
         $file_b = $files[$i];
         echo "***   $file_a VS $file_b\n";
         system("/usr/bin/sde/sde -icl -- $main $file_a $file_b");
         echo "\n";
      }
      echo "\n";
   }
   