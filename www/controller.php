<?php

$cmd = $_GET["command"];
$value = $_GET["value"];

//Open pipe and write some text to it.
//Mode must be r+ or fopen will get stuck.
$pipe = fopen('/home/bernard/carnavigation/datapipe','r+');
fwrite($pipe,strtoupper(substr($cmd,0,1))." ".intval($value)." ");
fclose($pipe);
if ($cmd=='g') {
	sleep(5);
	$pipe = fopen('/home/bernard/carnavigation/datapipe','r+');
	fwrite($pipe,"L ".intval($value)." ");
	fclose($pipe);
}


print strtoupper(substr($cmd,0,2))." ".intval($value);


?>
