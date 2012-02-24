<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<?php
include "config.php"
?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Netspective Fluent</title>
<link rel="shortcut icon"  href="<?php echo DOMAIN_URL?>/favicon.ico">
<meta name="csrf-param" content="authenticity_token">
<meta name="csrf-token" content="qN+/7gTXZbj2seEMlDjmP8FDLi9QjFkC39h/ENDT/aY=">

<link href="<?php echo FLUENT_PATH;?>/index_files/reset.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/core.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/sections.css" media="screen" rel="stylesheet" type="text/css">
<link href="<?php echo FLUENT_PATH;?>/index_files/messages.css" media="screen" rel="stylesheet" type="text/css">
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/jquery-1.6.3.min.js"></script>
<link href="<?php echo FLUENT_PATH;?>/index_files/style.css" media="screen" rel="stylesheet" type="text/css">
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/jquery.flot.js"></script>
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/cookie.js"></script>
<script type="text/javascript" src="<?php echo FLUENT_PATH;?>/index_files/table.js"></script>
</head>
<body>

	<div id="pageload" style="display: none;">1325742135</div>
	<div id="gln" style="display: none;">&nbsp;Loading…</div>
	<div id="modal-stream-chooser" style="display: none;">
		<h2>Streams</h2>
		<h4>Go to the <a href="<?php echo GRAYLOG_URL;?>/streams">streams overview</a></h4>
		<ul id="modal-stream-chooser-list">
		<li>No streams defined</li>
		</ul>
		</div>

	<div id="modal-host-chooser" style="display: none;">
		<h2>Hosts</h2>
		<h4>Go to the <a href="<?php echo GRAYLOG_URL;?>/hosts">hosts overview</a>.</h4>
	</div>

	<div id="back-to-top">Back to top</div>
	<div id="notification-notice" class="notification notification-flash" style="display: none; ">
      		<span class="notification-title">Success!</span>
      		Logged in successfully
    	</div>
	<div id="subtitle" style=" color:white;  width: 400px; position: absolute; text-align: center; left: 32%;"></div>
  	<div id="top">
    	<div id="userbar">
      		Logged in as admin &nbsp;<a href="<?php echo DOMAIN_URL;?>/logout">(Log out)</a>
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
</div>

<div id="content_block">
	<div id="inner" style="height: 732px; ">
	        <!--<a href="http://edge.fluent.netspective.com/#" id="sidebar-hide-link" title="Close sidebar">x</a>-->
        	<div id="sidebar-inner">
		       
			<div id="messages-content">
			
		<h2> <strong>DEVICE LISTS </strong></h2></br>

		<table width="150%" id="TABLE_4" class="example table-stripeclass:alternate" style="border:none">
			<thead>
			<tr>
		<th colspan="2" style="background:none; border:none;color:black;">SEARCH BY DOMIAN 
                    <input onkeyup="Table.filteR(this,this)" name="filter" class=search  size="6"><br><br></th>
			<th colspan="2" style="background:none; border:none;color:black;"><strong>SEARCH BY DEVICE </strong>
			<input onkeyup="Table.filter(this,this)" size="6"name="filter" class=search><br><br></th>
		</tr>						

		<tr>
                           	<th width="10%">DOMAINS</th>
				<th width="10%">DEVICES</th>
				<th width="10%">ACTIONS</th>
				<th width="10%">COMMENTS</th>
               </tr>
        
		</thead>
	<tbody>
		<tr>
                	<td align="center"><strong><a href="bp.php">BP</a></strong></td>
			<td><a href="bp.php?device_id=BP001">BP001</a></td>
			<td><div id="BP001"><input type="button" title="Click here to start the device" value="START" id="startBt" />  <input id="stopBt" 				type="button" value="STOP" disabled /> <input type="hidden" id="BP001_text" value="BP" name="BP001_text" /></div></td>
			<td><span id="BP001_result" class="stop"></span></td>
		</tr>

		<tr>
                      	<td align="center"><strong><a href="bp.php">BP</a></strong></td>
			<td><a href="bp.php?device_id=BP002">BP002</a></td>
			<td><div id="BP002"><input type="button" value="START" title="Click here to start the device" id="startBt" />  <input id="stopBt" 				type="button" value="STOP" disabled /><input type="hidden" id="BP002_text" value="BP" name="BP002_text" /></div></td>
			<td><span id="BP002_result" class="stop"></span></td>
		</tr>

		<tr>
                            	<td align="center"><strong><a href="bp.php">BP</a></strong></td>
				<td><a href="bp.php?device_id=BP003">BP003</a></td>
				<td><div id="BP003"><input type="button" value="START" title="Click here to start the device" id="startBt" />  					<input id="stopBt" type="button" value="STOP" disabled /><input type="hidden" id="BP003_text" value="BP" 					name="BP003_text" /></div></td>
				<td><span id="BP003_result" class="stop"></span></td>
		</tr>

		<tr>
                            	<td align="center"><strong><a href="pulseox.php">PULSEOX</a></strong></td>
				<td><a href="pulseox.php?device_id=PULSE001">PULSE001</a></td>
				<td><div id="PULSE001"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 					type="button" value="STOP" id="stopBt"  disabled /><input type="hidden" id="PULSE001_text" value="PULSEOX" 					name="PULSE001_text" /></div></td>
				<td><span id="PULSE001_result" class="stop"></span></td>
		</tr>
		<tr>
                           	<td align="center"><strong><a href="pulseox.php">PULSEOX</a></strong></td>
				<td><a href="pulseox.php?device_id=PULSE002">PULSE002</a></td>
				<td><div id="PULSE002"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 					type="button" value="STOP" id="stopBt"  disabled /></div></td><input type="hidden" id="PULSE002_text" value="PULSEOX" 					name="PULSE002_text" /></div></td>
				<td><span id="PULSE002_result" class="stop"></span></td>
                                </tr>
				
		<tr>
		               	<td align="center"><strong><a href="pulseox.php">PULSEOX</a></strong></td>
				<td><a href="pulseox.php?device_id=PULSE003">PULSE003</a></td>
				<td><div id="PULSE003"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 					type="button" value="STOP" id="stopBt" disabled /></div></td>
				<td><span id="PULSE003_result" class="stop"></span></td>
		</tr>
		<tr>
                            <td align="center"><strong><a href="temp.php">TEMPERATURE</a></strong></td>
		           <td><a href="temp.php?device_id=TEMP111">TEMP111</a></td>
			  <td><div id="TEMP111"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 					type="button" value="STOP" id="stopBt" disabled /></div></td><input type="hidden" id="TEMP111_text" value="TEMPERATURE" 				name="TEMP111_text" /></div></td>
			 <td><span id="TEMP111_result" class="stop"></span></td></tr>
                        <td align="center"><strong><a href="temp.php">TEMPERATURE</a></strong></td>
			<td><a href="temp.php?device_id=TEMP123">TEMP123</a></td>
			<td><div id="TEMP123"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 				type="button" value="STOP" id="stopBt" disabled /></div></td><input type="hidden" id="TEMP123_text" value="TEMPERATURE" 			name="TEMP123_text" /></div></td>
			<td><span id="TEMP123_result" class="stop"></span></td></td>
               </tr>
		<tr>
                 	<td> align="center"><strong><a href="temp.php">TEMPERATURE</a></strong></td>
			<td><a href="temp.php?device_id=TEMP110">TEMP110</a></td>
			<td><div id="TEMP110"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 				type="button" value="STOP" id="stopBt" disabled /></div></td><input type="hidden" id="TEMP110_text" value="TEMPERATURE" 			name="TEMP110_text" /></div></td>
			<td><span id="TEMP110_result" class="stop"></span></td></td>
              </tr>
 		<tr>
 	        	<td align="center"><strong><a href="ecg.php">ECG</a></strong></td>
 	 		<td><a href="ecg.php?device_id=ECG112">ECG112</a></td>
			<td><div id="ECG112"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 			type="button" value="STOP" id="stopBt" disabled /></div></td><input type="hidden" id="ECG112_text" value="ECG" 
			name="ECG112_text" /></div></td>
	               <td><span id="ECG112_result" class="stop"></span></td></td>
                </tr>
		
		<tr>
                            	<td align="center"><strong><a href="ecg.php">ECG</a></strong></td>
				<td><a href="ecg.php?device_id=ECG109">ECG109</a></td>
				<td><div id="ECG109"><input type="button" value="START" title="Click here to start the device" id="startBt" />   <input 				type="button" value="STOP" id="stopBt" disabled /></div></td><input type="hidden" id="ECG109_text" value="ECG"  				name="ECG109_text" /></div></td>
                                <td><span id="ECG109_result" class="stop"></span></td></tr>
		</tbody>
		</table>
</br></br>

</div>
			
	        </div>
	        <div class="content-bottom content-bottom-sidebar">
          		<div class="content-bottom-left"><img alt="Contentedge_l" src="<?php echo GRAYLOG_URL;?>/index_files/contentedge_l.png"></div>
          		<div class="content-bottom-right"><img alt="Contentedge_r" src="<?php echo GRAYLOG_URL;?>/index_files/contentedge_r.png"></div>
       		 	<br style="clear: both;">
        	</div>
	</div>

</div>


<br style="clear: both;"><br>

<script type="text/javascript">
$(document).ready(function() {
$('input#startBt').click(function() {
	
var sel_id = $(this).parent().attr('id');
var device = $('#'+sel_id+"_text").val();
//alert(device+":"+sel_id);
send(device+":"+sel_id);
			$('#'+ sel_id +' #startBt').attr("disabled","disabled");
			$('#'+ sel_id +' #stopBt').removeAttr('disabled');
//$('#'+ sel_id +'_result').removeClass("stop").addClass("start").html('Started');
});

$('input#stopBt').click(function() {
	
var sel_id = $(this).parent().attr('id');
var device = $('#'+sel_id+"_text").val();
//alert(device+":"+sel_id);
send_stop(device+":"+sel_id);
			$('#'+ sel_id +' #startBt').removeAttr('disabled');
			$('#'+ sel_id +' #stopBt').attr("disabled","disabled");
//$('#'+ sel_id +'_result').removeClass("start").addClass("stop").html('Stopped');
});
});


	var msgsize;
	var ws;
	var url;
	var avgbytes;
	var device_dynamic;
	var enabled_button =  new Array();
	var device_list =  new Array();
				


function checkdata() {
	for(var i=0; i<device_list.length; i++) {

if(device_list[i] == '') { continue; }

if($("#"+device_list[i]).get(0)){
			$('#'+ device_list[i] +' #startBt').attr("disabled","disabled");
			$('#'+ device_list[i] +' #stopBt').removeAttr('disabled');
} 
		}	
}
				
				
				
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
	        url = "<?php echo WEBSOCKET;?>/device";
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

	if(message!="DLIST")
	{
	var devices=message;

	
	}

var device_dynamic = "";
	var d1 = devices.split(';');
var d2 = new Array();
	for(var i=0; i<d1.length; i++) {

device_dynamic += d1[i]+",";
	}
//alert(device_dynamic);	
device_list = device_dynamic.split(',');

	checkdata();

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

function send(v) {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	var devices_sel=v;

	ws.send("START:"+devices_sel);
}

function send_stop(v) {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	var devices_sel=v;
	ws.send("STOP:"+devices_sel);
}

function null_send() {
	if (ws === undefined || ws.readyState != 1) {
		chat_message("Websocket is not avaliable for writing");
		return;
	}
	ws.send("dynamiclist");
}

function dlist_send() {
        if (ws === undefined || ws.readyState != 1) {
                chat_message("Websocket is not avaliable for writing");
                return;
        }
        ws.send("DLIST");
}

toggle_connect();	
setInterval("dlist_send()",10000);

</script>
</body></html>
