<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<!-- saved from url=(0024)http://demo.fluent.netspective.com/ -->
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Netspective Fluent</title>
<link rel="shortcut icon" href="http://demo.fluent.netspective.com/favicon.ico">
<meta name="csrf-param" content="authenticity_token">
<meta name="csrf-token" content="qN+/7gTXZbj2seEMlDjmP8FDLi9QjFkC39h/ENDT/aY=">
<link href="http://demo.fluent.netspective.com/medi/index_files/reset.css" media="screen" rel="stylesheet" type="text/css">
<link href="http://demo.fluent.netspective.com/medi/index_files/core.css" media="screen" rel="stylesheet" type="text/css">
<link href="http://demo.fluent.netspective.com/medi/index_files/sections.css" media="screen" rel="stylesheet" type="text/css">
<link href="http://demo.fluent.netspective.com/medi/index_files/messages.css" media="screen" rel="stylesheet" type="text/css">
<script type="text/javascript" src="jquery-1.6.3.min.js"></script>
<script type="text/javascript" src="jquery.flot.js"></script>
<script type="text/javascript" src="cookie.js"></script>
<body>

<div id="pageload" style="display: none;">1325742135</div>
<div id="gln" style="display: none;">&nbsp;Loading…</div>
	<div id="modal-stream-chooser" style="display: none;">
	<h2>Streams</h2>
	<h4>Go to the <a href="http://demo.fluent.netspective.com/streams">streams overview</a>.</h4>
	<ul id="modal-stream-chooser-list">
	<li>No streams defined</li>
	</ul>
	</div>

	<div id="modal-host-chooser" style="display: none;">
		<h2>Hosts</h2>
		<h4>Go to the <a href="http://demo.fluent.netspective.com/hosts">hosts overview</a>.</h4>
	</div>

	<div id="back-to-top">Back to top</div>
	<div id="notification-notice" class="notification notification-flash" style="display: none; ">
      		<span class="notification-title">Success!</span>
      		Logged in successfully
    	</div>
	<div id="subtitle" style=" color:white;  width: 400px; position: absolute; text-align: center; left: 32%;">
	</div>
  	<div id="top">
    	<div id="userbar">
      		Logged in as admin &nbsp;<a href="http://demo.fluent.netspective.com/logout">(Log out)</a>
    	</div>
	
	<a href="http://demo.fluent.netspective.com/messages"><img alt="Netspective" id="toplogo" src="http://demo.fluent.netspective.com/medi/index_files/netspective.png"></a>
	
	<ul id="topmenu">
	<li class=""><a href="http://demo.fluent.netspective.com/messages">Messages</a></li>
	<li class=""><a href="http://demo.fluent.netspective.com/streams">Streams</a></li>
      	<li class="topmenu-active"> <a href="http://demo.fluent.netspective.com/medi/index.php">DEVICES</a></li>
      	<li class=""><a href="http://demo.fluent.netspective.com/analytics">Analytics</a></li>
      	<li class=""><a href="http://demo.fluent.netspective.com/hosts">Hosts</a></li>
      	<li class=""><a href="http://demo.fluent.netspective.com/blacklists">Blacklists</a></li>
      	<li class=""><a href="http://demo.fluent.netspective.com/settings">Settings</a></li>
      	<li class=""><a href="http://demo.fluent.netspective.com/users">Users</a></li>
    	</ul>
</div>
<br style="clear: both;">
<div id="content-top">
	<div id="content-tabs">
	      <br style="clear: both;">
	</div>
</div>

<div id="main-left">
	<div id="content">
		<div id="content-inner">
				<h3><a href="http://demo.fluent.netspective.com/medi/index.php" >HOME</a></h3> </br>
				  <h2>Domains</h2>

 			<ul id="domain-menu">
				</br>

    			  <li class=""><a href="http://demo.fluent.netspective.com/medi/bp.php">BP</a></li></br>	
    			  <li class=""><a href="http://demo.fluent.netspective.com/medi/pulseox.php">PULSEOX</a></li></br>
     			 <li class=""> <a href="http://demo.fluent.netspective.com/medi/temp.php">TEMPERATURE</a></li></br>
    			  <li class=""><a href="http://demo.fluent.netspective.com/medi/ecg.php">ECG</a></li></br>

      
				<br style="clear: both;">

		</div>

	</div>
	<div class="content-bottom">
		<div class="content-bottom-left"><img alt="Contentedge_l" src="http://demo.fluent.netspective.com/medi/index_files/contentedge_l.png"></div>
      		<div class="content-bottom-right"><img alt="Contentedge_r" src="http://demo.fluent.netspective.com/medi/index_files/contentedge_r.png"></div>
      		<br style="clear: both;">
    	</div>
</div>

<div id="main-right">
	<div id="sidebar" style="height: 732px; ">
	        <!--<a href="http://demo.fluent.netspective.com/#" id="sidebar-hide-link" title="Close sidebar">x</a>-->
        	<div id="sidebar-inner">
		       
			<div id="messages-content">
			
		<h2> Device Management </h2> </br></br></br>

<table id="start-stop-table" cellpadding="0" cellspacing="0" border=0"><tr><td style="padding: 0 15px;">
<tr>
<td style="padding: 0 15px;">
	<p style="font-size:14px">DOMAIN LIST</p> <br />
	<select name="domain_select" id="domain_select" multiple="multiple" style="height:160px;width:150px;font-size:14px;">
	<option value="BP">BP</option>
	<option value="PULSEOX">PULSEOX</option>
	<option value="TEMPERATURE">TEMPERATURE</option>
	<option value="ECG">ECG</option>
	</select>
</td>

<td style="padding: 0 15px;">
	<p style="font-size:14px">DEVICES LIST</p> <br />
 	<select name="device-select" id="device-select" multiple="multiple" style="height:160px;width:150px;font-size:14px;">
	<option>Devices</option>
	</select>
</td>
<td style="padding: 0 15px;">
	<p style="font-size:14px">DEVICE STATUS/MANAGE</p> <br />
 	<textarea name="device-status" id="device-status" style="height:80px;width:300px;font-size:14px;">
	</textarea>
	<br /> <br /> <br />
 <input id="startBt" style="height:30px;width:60px;font-size:14px;margin-left: 40px;margin-right: 15px;" type="button" value="Start" onclick="send()" />
 <input id="stopBt" style="height:30px;width:60px;font-size:14px;margin-left: 15px;margin-right: 15px;" type="button" value="Stop" onclick="send_stop()" />
	
	

</td>



</tr>
</table>
</br></br>

</div>
			
	        </div>
	        <div class="content-bottom content-bottom-sidebar">
          		<div class="content-bottom-left"><img alt="Contentedge_l" src="http://demo.fluent.netspective.com/medi/index_files/contentedge_l.png"></div>
          		<div class="content-bottom-right"><img alt="Contentedge_r" src="http://demo.fluent.netspective.com/medi/index_files/contentedge_r.png"></div>
       		 	<br style="clear: both;">
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
				var msgsize;
				var ws;
				var url;
				var avgbytes;
				var device_dynamic="EMPTY";
				
				

$('#domain_select').change(function() {
	var val = $(this).val();
	$('#device-select').empty();
	if(val == "BP") {
		


		$('#device-select').append($('<option></option>').val("BP:BP_LAB44").html("BP_LAB44"));
		$('#device-select').append($('<option></option>').val("BP:BP_LAB121").html("BP_LAB121"));
		$('#device-select').append($('<option></option>').val("BP:BP_LAB122").html("BP_LAB122"));		
		$('#device-select').append($('<option></option>').val("BP:BP_LAB123").html("BP_LAB123"));
		$('#device-select').append($('<option></option>').val("BP:BP_LAB125").html("BP_LAB125"));
	}
	if(val == "PULSEOX") {
		$('#device-select').append($('<option></option>').val("PULSEOX:PULSE_LAB1").html("PULSE_LAB1"));
		$('#device-select').append($('<option></option>').val("PULSEOX:PULSE_LAB2").html("PULSE_LAB2"));
		$('#device-select').append($('<option></option>').val("PULSEOX:PULSE_LAB3").html("PULSE_LAB3"));		
		$('#device-select').append($('<option></option>').val("PULSEOX:PULSE_LAB4").html("PULSE_LAB4"));
	}
	if(val == "TEMPERATURE") {
		$('#device-select').append($('<option></option>').val("TEMP:TEMP_LAB44").html("TEMP_LAB44"));
		$('#device-select').append($('<option></option>').val("TEMP:TEMP_LAB1").html("TEMP_LAB1"));
		$('#device-select').append($('<option></option>').val("TEMP:TEMP_LAB3").html("TEMP_LAB3"));	
		$('#device-select').append($('<option></option>').val("TEMP:TEMP_LAB3").html("TEMP_LAB121"));	

	}
	if(val == "ECG") {
		$('#device-select').append($('<option></option>').val("ECG:ECG_LAB44").html("ECG_LAB44"));
	}
});

$('#device-select').change(function() {
var newval = $(this).val().toString();

var spl = newval.split(":");
var status = device_dynamic.indexOf(spl[1]);

$('#startBt').removeAttr('disabled');
$('#stopBt').removeAttr('disabled');

if(status == -1) 
{ 	
$('#device-status').val("Device Is Not Running\nClick Start Button to Run the Device");
$('#stopBt').attr("disabled","disabled");

}
else 
{ 
	$('#device-status').val("Device Is Running\nClick Stop Button to turn off the Device");
	$('#startBt').attr("disabled","disabled");
}

});

				
				
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
	url = "ws://203.129.254.88:9003/bp";
	//device_dynamic="BP_LAB44,PULSE_LAB1,TEMP_LAB3,ECG_LAB44";

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
		if(!cookie_val) 
		{ 
			createCookie('cookie_value','set',1); 
		}
		else 
		{
			disconnect(); 
			toggle_connect(); 
		}
		null_send();
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
				chat_message2(message.value,message.sender);
			}
		 else if (message.type == "participants") {}
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

function chat_message2(message,sender) 
{


	device_dynamic=message;
	if (arguments.length == 1) 
	{
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
	var devices_sel=$('#device-select').val();
	ws.send("START:"+devices_sel);
}

function send_stop() {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	var devices_sel=$('#device-select').val();
	ws.send("STOP:"+devices_sel);
}

function null_send() {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	ws.send("dynamiclist");
}
toggle_connect();	


</script>
</body></html>
