var oldVar="";
var timer=null;
function send(cmd,value,synchronous) {
	if (typeof(synchronous)=='undefined') { synchronous=false; }
	if (typeof(value)=='undefined') { value=0; }
	/*var curVar=value+cmd;
	if (curVar==oldVar) {
		clearTimeout(timer);
		timer=setTimeout("oldVar='';",500);
		return;
	}
	oldVar=curVar;*/
	value=parseInt(value);
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.open("GET", "controller.php?command="+cmd+"&value="+value,!synchronous);
	xmlhttp.send(null);
}
