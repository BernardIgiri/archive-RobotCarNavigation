<?php

$x = $_GET["x"];
$y = $_GET["y"];

//Open pipe and write some text to it.
//Mode must be r+ or fopen will get stuck.
$pipe = fopen('/home/bernard/carnavigation/pipe','r+');
fwrite($pipe,$x . "," . $y);
fclose($pipe);



?>
