<!doctype html>
<html>
<head>
<!--[if IE ]>
		<script type="text/javascript" src="http://bobcravens.com/files/excanvas.min.js"></script>		
		<![endif]-->
<script type="text/javascript" src="jquery-1.6.3.min.js"></script>
<script type="text/javascript" src="jquery.flot.js"></script>
</head>
<body onbeforeunload="javascript: disconnect();">


<style>
body,html {
	margin: 0px;
	padding: 0px;
	height: 100%;
	background-color: #999;
	font-family: sans-serif;
	font-size: 14px;
}

h3 {

}

#controls {
	padding: 4px;
	height: 20%;
}

input {
	width: 200px;
}

#chart {
	height: 1%;
	overflow: auto;
	background-color: #fff;
}

#chart .client {
	color: #fff;
}

#chart .server {
	color: yellow;
}

#chart .message {
	color: white;
}

</style>
<div align="center"><h1>Netspective Web-based Data Intelligence</h1></div>
<div id="Graph"></div>		
<div id="chart"></div>
<div id="controls">
	<div id="messages"></div>	
</div>
<script type="text/javascript">
var ws;
var url;

				var xVal = 0;
				var timestamp = 0;
				var countarr = 0;
				var temp = 0;
				
				var device_id = "<?php echo $_GET['device_id'];?>";
				
				var series1 = new Array();
				var series2 = new Array();
				//var series3 = new Array();
				//var vdat = [ series1, series2, series3 ];
			
				var vdat = new Array();
			    
			    var options = {
			    	legend: { show: true, position: "nw" },
			    	grid: { hoverable: true, clickable: true }	};
			    var plot = new Array();
				var arr = new Array();
				var total=0;
				
$(document).ready(init);

function init() {
	$(document).keypress(function(event) {
		if ( event.which == 13 ) {
			event.preventDefault();
			send();
		}
	});
}

function connect() {
	url = "ws://netspective-webserver.com:9003/pulseox";
	console.log(url);
	
	if ("WebSocket" in window) {
		ws = new WebSocket(url);
	} else if ("MozWebSocket" in window) {
		ws = new MozWebSocket(url);
	} else {
		chat_message("This Browser does not support WebSockets");
		return;
	}
	ws.onopen = function(e) {
		chat_message("A connection to "+url+" has been opened.");
		send();
	};
	
	ws.onerror = function(e) {
		chat_message("An error occured, see console log for more details.");
		console.log(e);
	};
	
	ws.onclose = function(e) {
		chat_message("The connection to "+url+" was closed.");
	};
	
	ws.onmessage = function(e) {
		var message = JSON.parse(e.data);
		
		if (message.type == "msg") {
			if(device_id) { 
				var message_string = message.value;
				var status = message_string.indexOf(device_id);
				if(status != "-1") { chat_message2(message.value,message.sender); }
				else {chat_message(status+device_id+"sorry not found"+"-----"+message.value,message.sender);}
			} else {
				chat_message2(message.value,message.sender);
			}
		} else if (message.type == "participants") {}
	};
}

function chat_message(message,sender) {
	if (arguments.length == 1) {
		sender = "";
	}
	
	var style;
	
	if (sender == "") {
		style = "client";
	} else if (sender == "server") {
		style = "server";
		sender = "["+sender+"]";
	} else {
		style = "message";
		sender = "["+sender+"]";
	}
	$("#messages").append("<span class='"+style+"'><span class='sender'>"+sender+"</span> <span class='msg'>"+message+"</span></span><br />");
	$("#messages").prop({ scrollTop: $("#messages").prop("scrollHeight")  });
}

function chat_message2(message,sender) {
	if (arguments.length == 1) {
		sender = "";
	}
	
	var style;
	
	if (sender == "") {
		style = "client";
	} else if (sender == "server") {
		style = "server";
		sender = "["+sender+"]";
	} else {
		style = "message";
		sender = "["+sender+"]";
	}
	if(message == "connected") { chat_message(message,sender); }
	var values = message.split(",");
	
	var countarr = $.inArray(values[1], arr);
	temp = countarr;
	if(countarr=="-1") { 
			$('<h2/>', {
			id: "heading"+total,  
			}).appendTo('#Graph');
			
			$("#heading"+total).html("Record for "+values[1]);

			$('<div/>', {
			id: "chart"+total,  
			css: {  
				width:'650px',
				height:'360px'  
			}
			}).appendTo('#Graph');
			
			
		series1[total] = { label: "SPO2", data: [] };
		series2[total] = { label: "PULSE RATE", data: [] };
		//series3[total] = { label: "Pulse Rate", data: [] };
		vdat[total] = [ series1[total], series2[total] ];
		arr.push(values[1]); 
		plot[total] = $.plot($("#chart"+total), vdat[total], options);
		temp = total;
		total++;
	}
	if(total!=0) {
		getData(values[3], values[4], values[5], values[2], temp,"#chart"+temp);
	}
	$("#messages").append("<span class='"+style+"'><span class='sender'>"+sender+"</span> <span class='msg'>"+message+"</span></span><br />");
	$("#messages").prop({ scrollTop: $("#messages").prop("scrollHeight")  });
}

function disconnect() {
	ws.close();
	chat_message("Closed");
}


function toggle_connect() {
		connect();
}

function send() {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	
	ws.send("pulseox");
}


function getData(values3, values4, values5, timestamp, countarr,lab){
					// This could be an ajax call back.
					var yVal1 = values3;
					var yVal2 = values4;

					var datum1 = [xVal, yVal1];
					var datum2 = [xVal, yVal2];

					vdat[countarr][0].data.push(datum1);
					vdat[countarr][1].data.push(datum2);
					if(vdat[countarr][0].data.length>10){
						// only allow ten points
						vdat[countarr][0].data = vdat[countarr][0].data.splice(1);
						vdat[countarr][1].data = vdat[countarr][1].data.splice(1);
					}
					xVal++;
					plot[countarr].setData(vdat[countarr]);
					plot[countarr].setupGrid();
					plot[countarr].draw();
				}

			
			toggle_connect();	
</script>

</body>
</html>
