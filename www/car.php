<html>
<head>
<meta name="apple-mobile-web-app-capable" content="yes" />

<title>
Car Control
</title>

<script type="text/javascript" charset="utf-8" src="js/ajax.js"></script>
<script type="text/javascript" charset="utf-8">

function map() {
	send('s',0,true);
	window.location='index.php';
}

var startx = 0;
var starty = 0;
var xvector = 0;
var yvector = 0;


document.ontouchstart = function(e){
	var touch = e.touches[0];
	startx = touch.pageX;
	starty = touch.pageY;
	document.getElementById("touch").innerHTML = "Starting";
}

document.ontouchend = function(e){
	startx = 0;
	starty = 0;
	xvector = 0;
	yvector = 0;
	document.getElementById("touch").innerHTML = "Touch Ended";
	
	send('s');
}

document.ontouchmove = function(e){
  e.preventDefault();
  if(e.touches.length == 1){ 
    var touch = e.touches[0]; 
    xvector = touch.pageX - startx;
    yvector = - (touch.pageY - starty);
    document.getElementById("touch").innerHTML = "Current Vector " + xvector + " " + yvector;

	var xThreshold=100;
	var yThreshold=100;
	if (yvector>yThreshold) {
		if (xvector>xThreshold) {
			send('e');
		} else if (xvector<-xThreshold) {
			send('q');
		} else {
			send('f');
		}
	} else if (yvector<-yThreshold) {
		if (xvector>xThreshold) {
			send('c');
		} else if (xvector<-xThreshold) {
			send('z');
		} else {
			send('b');
		}
	}
  }
}

</script>
<style>

input {
	width: 350px;
	height: 175px;
	font-size: 70px;
}
input#map {
	position: absolute;
	left: 300;
	top: 1095px;
}
</style>
</head>



<body style="background-image:url('logo.jpg');background-repeat:no-repeat;background-position: center center;">

<div style="font-size:50px" id="touch">
None
</div>
<input id="map" type="button" value="map" onclick="map();">

</body>
</html>
