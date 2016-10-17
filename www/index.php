<html>
<head>
<meta name="apple-mobile-web-app-capable" content="yes" />
<title>
Car Control
</title>

<script type="text/javascript" charset="utf-8" src="js/ajax.js"></script>
<script type="text/javascript" charset="utf-8">

function controller() {
	send('s',0,true);
	window.location='car.php';
}

function init() {
	var mapDiv=document.getElementById('map');
	mapDiv.appendChild(createButton(1,0,10));
	mapDiv.appendChild(createButton(2,3,10));
	mapDiv.appendChild(createButton(3,6,10));
	mapDiv.appendChild(createButton(4,8,10));
	mapDiv.appendChild(createButton(5,0,7));
	mapDiv.appendChild(createButton(6,3,7));
	mapDiv.appendChild(createButton(7,6,7));
	mapDiv.appendChild(createButton(8,8,7));
	mapDiv.appendChild(createButton(9,0,3));
	mapDiv.appendChild(createButton(10,3,3));
	mapDiv.appendChild(createButton(11,6,3));
	mapDiv.appendChild(createButton(12,8,3));
	mapDiv.appendChild(createButton(13,0,0));
	mapDiv.appendChild(createButton(14,3,0));
	mapDiv.appendChild(createButton(15,6,0));
	mapDiv.appendChild(createButton(16,8,0));
}

function createButton(number,x,y) {
	var recWidth=102;
	var recHeight=92;
	var img=document.createElement('img');
	img.className="button";
	img.src="images/transparent.gif";
	img.style.position='absolute';
	img.border=0;
	img.style.top=y*recHeight+25+'px';
	img.style.left=x*recWidth+18+'px';
	img.width=recWidth;
	img.height=recHeight;
	var a=document.createElement('a');
	a.href="javascript:send('g',"+number+");";
	a.appendChild(img);
	return a;
}

</script>

<style>
img.button {
}
input {
	width: 350px;
	height: 175px;
	font-size: 70px;
}
div#map {
	postion: absolute;
	left: 0;
	top: 0;
	padding:0;
	margin:0;
	background-image: url("images/map.jpg");
	width: 950px;
	height: 1050px;
}
input#stop {
	position: absolute;
	left: 50;
	top: 1095px;
}
input#remote {
	position: absolute;
	left: 500px;
	top: 1095px;
}
</style>
</head>



<body onload="init();" style="background-repeat:no-repeat;background-position: center center;">

<div id='map'>&nbsp;</div>
<input id="stop" type="button" value="stop" onclick="send('s');">
<input id="remote" type="button" value="remote" onclick="controller();">


</body>
</html>
