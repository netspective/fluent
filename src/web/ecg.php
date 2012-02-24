<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
include "config.php"
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Netspective Fluent</title>
<link rel="shortcut icon" href="<?php echo DOMAIN_URL;?>/favicon.ico">
<meta name="csrf-param" content="authenticity_token">
<meta name="csrf-token" content="qN+/7gTXZbj2seEMlDjmP8FDLi9QjFkC39h/ENDT/aY=">
<link href="<?php echo FLUENT_PATH;?>/style.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/reset.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/core.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/sections.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/messages.css" media="screen" rel="stylesheet" type="text/css">
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/jquery-1.6.3.min.js"></script>
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/jquery.flot.js"></script>
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/cookie.js"></script>
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/index_files/table.js"></script>
</head>
<body>

<div id="pageload" style="display: none;">1325742135</div>
<div id="gln" style="display: none;">&nbsp;Loading…</div>
	<div id="modal-stream-chooser" style="display: none;">
	<h2>Streams</h2>
	<h4>Go to the <a href="<?php echo GRAYLOG_URL;?>/streams">streams overview</a>.</h4>
	<ul id="modal-stream-chooser-list">
	<li>No streams defined</li>
	</ul>
	</div>

	<div id="modal-host-chooser" style="display: none;">
		<h2>Hosts</h2>
		<h4>Go to the <a href="<?php echo GRAYLOG_URL;?>/hosts">hosts overview</a>.</h4>
	</div>

	<div id="notification-notice" class="notification notification-flash" style="display: none; ">
      		<span class="notification-title">Success!</span>
      		Logged in successfully
    	</div>
	<div id="subtitle" style=" color:white;  width: 400px; position: absolute; text-align: center; left: 32%;">
	</div>
  	<div id="top">
    	<div id="userbar">
      		Logged in as admin &nbsp;<a href="<?php echo GRAYLOG_URL;?>/logout">(Log out)</a>
    	</div>
	
<a href="<?php echo DOMAIN_URL;?>/messages"><img alt="Netspective" id="toplogo" src="<?php echo FLUENT_PATH;?>/index_files/covidien.png"></a>	
	
	<ul id="topmenu">
	<li class=""><a href="<?php echo GRAYLOG_URL;?>/messages">Messages</a></li>
	<li class=""><a href="<?php echo GRAYLOG_URL;?>/streams">Streams</a></li>
	<li class="topmenu-active"> <a href="<?php echo APACHE_URL;?>/index.php">DEVICES</a></li>
      	<li class=""><a href="<?php echo GRAYLOG_URL;?>/analytics">Analytics</a></li>
      	<li class=""><a href="<?php echo GRAYLOG_URL;?>/hosts">Hosts</a></li>
      	<li class=""><a href="<?php echo GRAYLOG_URL;?>/blacklists">Blacklists</a></li>
      	<li class=""><a href="<?php echo GRAYLOG_URL;?>/settings">Settings</a></li>
      	<li class=""><a href="<?php echo GRAYLOG_URL;?>/users">Users</a></li>
    	</ul>
</div>
<br style="clear: both;">
<div id="content-top">
	<div id="content-tabs">
<div id="main-right">
	<div id="sidebar" style="width:800px;height:150px ">
        	<div id="sidebar-inner">
		             <button name='backbtn' value='BACK' onclick="location='devicelist.php'"/>Back To DeviceLists</button> 
			<div id="messages-content">
 <div id="placeholder" style="width:650px;height:400px; "></div> 

			</div>
			
	        </div>
	        <div class="content-bottom content-bottom-sidebar">
          		<div class="content-bottom-left"><img alt="Contentedge_l" src="<?php echo FLUENT_PATH;?>/index_files/contentedge_l.png"></div>
          		<div class="content-bottom-right"><img alt="Contentedge_r" src="<?php echo FLUENT_PATH;?>/index_files/contentedge_r.png"></div>
       		 	<br style="clear: both;">
        	</div>
	</div>

</div>

<div id="main-right-new">
<div id="devicebar-inner">
<div id="device-stat">
	<h2>Device Statistics</h2>
	
	 <h3>Start Time </h3> <span id="start_time">00:00:00</span>
	 <h3>Current  Time </h3> <span id="nxt_time">00:00:00</span>
	 <h3> Total Bytes Recevied </h3> <span id="total_bytes">0</span> KBytes
	 <h3> Size of a message</h3> <span id="msg_size"></span> KBytes	
	 <h3> Total Number of Devices </h3>  <span id="total_devices">1</span>
	 <h3> Total Messages</h3> <span id="total_msg">0</span>
	 <h3> Average Bytes Per Messsage</h3> <span id="avg">0.0</span> KBytes
	 <h3> Messages Per Minute</h3> <span id="tempmesg">0</span>
	 <h3> Available Devices </h3>  <span id="devices"></span>
	        
</div>
</div>
</div>
	</div>
</div>

</div>



<br style="clear: both;"><br>
<script type="text/javascript">
function linkedlist(value) {
	if(value) {
		$(".block_class").css("display","none");
		$("#block"+value).css("display","block");
	} else { $(".block_class").css("display","block"); }
}
//CHANGE FROM HERE						<h3> Records For Device  <span id="deviceheading"></span> </h3>  
	var ws;
				var curr_time = new Date();
				var hours = curr_time.getHours();
				var minute = curr_time.getMinutes();
				var sec=curr_time.getSeconds();
				var temp1 = 0;
				var st_time;
				var total_msgs=0;
				var avgbytes=0.0;
				var url;
				var msgsize=0;
				var count=0;
				var merid = "";
				tim_diff=0
				sec_tim=0;
				sec_dat=0;
				sec_hrs=0;
				sec_min=0;
				sec_secs=0;
				time1=0;
                		timediff=0;


				if(sec<=9)
				sec="0"+sec;
				if(minute<=9)
				minute="0"+minute;
				if(hours<=9)
				hours="0"+hours;

				var sec_tim;
	
				st_time=hours+":"+minute+":"+sec;

				var xVal = 0;
				var timestamp = 0;
				var countarr = 0;
				var temp = 0;
				
				var device_id = "<?php echo $_GET['device_id'];?>";
				var series1 = new Array();
				var series2 = new Array();
				var series3 = new Array();
				var datalength = 0;
				var m_count=0;
				//var vdat = [ series1, series2, series3 ];
			
				var data = [];
				var datax = [];
			    
			        var options = 
			    	{
			      		series: { shadowSize: 0 }, 
			        	yaxis: { min: -1, max: 1 },
			        	xaxis: { show: false, mode: "time" }
			   	};

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
	url = "<?php echo WEBSOCKET;?>/bp";
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

		var cookie_val = readCookie('cookie_value');
		if(!cookie_val) { createCookie('cookie_value','set',1); send(); }
		else {disconnect(); toggle_connect(); }
		
		
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
		
		datalength=datalength+(e.data.length/1024);
		   msgsize=e.data.length/1024;
		 total_msgs++;
		 avgbytes=datalength/total_msgs;
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
		sec_dat= new Date();
		second_tim=sec_dat.getTime();
  		sec_hrs=sec_dat.getHours();
		sec_min=sec_dat.getMinutes();
		sec_secs=sec_dat.getSeconds();
		if(sec_secs<=9)
		sec_secs="0"+sec_secs;
		if(sec_min<=9)
		sec_min="0"+sec_min;
		if(sec_hrs<=9)
		sec_hrs="0"+sec_hrs;

		sec_tim=sec_hrs+":"+sec_min+":"+sec_secs;
		
		if(count==0)
		{
		time1=second_tim;
		}
		
		timediff=second_tim-time1;
		//alert(second_tim+" "+time1);
		
		if(timediff>=60000)
                {
		//alert(timediff);	
		 $('#tempmesg').html(count);
		  time1=second_tim;
		 count=0;
		 temp1=0;
		}
		count++;
	var countarr = $.inArray(values[1], arr);
	if(countarr == "-1")
	{
				
		$("#heading"+total).html("Record for "+values[1]);
		$("#total_devices").html(total++);
		$('#deviceheading').append(values[1]+"<br/><br/>");
		$('#devices').append(values[1]+"<br/><br/>");
		$('#devices_list').append("<a href='javascript:void()' onclick=\"linkedlist('"+total+"')\">"+values[1]+"</a><br /><br />");
		arr.push(values[1]); 



	}

	
	var y = values[4];
	var x = values[3];
	
      	
	if ( data.length > 1000)
	{
			
		        data = data.slice(1);
			datax = datax.slice(1);
	}
	
	
 	data.push(y);
	datax.push(x);
 	var res = [];
	m_count++;
        for (var i = 0; i < data.length; ++i) 
	{
        	res.push([datax[i],data[i]]);
	}

	if(m_count==400) {
	var plot = $.plot($("#placeholder"), [ res ], options);
	
	plot.setData([ res ]);
        plot.draw();
	m_count=0;
	}
	
	$('#start_time').html(st_time);	
	$('#nxt_time').html(sec_tim);			
	$('#total_bytes').html(datalength.toFixed(3));
	$('#msg_size').html(msgsize.toFixed(4));
	$('#total_devices').html(1);
	$('#total_msg').html(total_msgs);
	$('#avg').html(avgbytes.toFixed(4));
	
}

function disconnect() {
	eraseCookie('cookie_value');
	
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
	
	ws.send("ecg");
}


toggle_connect();

</script>

</body></html>
