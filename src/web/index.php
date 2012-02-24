<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
include "config.php"
?>

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  <title>Netspective Fluent </title>
  
  <link rel="shortcut icon"  href="<?php echo DOMAIN_URL?>/favicon.ico">
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

<ul id="modal-host-chooser-list">
    <li><a href="<?php echo DOMAIN_URL;?>/hosts/PUBLISHER/messages">PUBLISHER</a></li>
    <li><a href="<?php echo DOMAIN_URL;?>/hosts/SUBSCRIBER/messages">SUBSCRIBER</a></li>
</ul>
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
      <br style="clear: both;">
    </div>
  </div>

  <div id="main-left">

  <div id="content">
    <div id="content-inner">
      



<div id="messages-content">
<ul id="device-manage">
        </br>
      <li style="font-family: Georgia,'Times New Roman',Times,serif;font-size:14px;color:#000000;" class=""><a href="<?php echo APACHE_URL;?>/devicelist.php">MANAGE DEVICES</a></li></br>

    </ul>

  <h2>Domains</h2>

 <ul id="domain-menu">
	</br>
      <li class=""><a href="<?php echo APACHE_URL;?>/bp.php">BP</a></li></br>	
      <li class=""><a href="<?php echo APACHE_URL;?>/pulseox.php">PULSEOX</a></li></br>
      <li class=""> <a href="<?php echo APACHE_URL;?>/temp.php">TEMPERATURE</a></li></br>
      <li class=""><a href="<?php echo APACHE_URL;?>/ecg.php">ECG</a></li></br>
    </ul>

</div>


<div id="messages-bottom">

</div>

<br style="clear: both;">

</div>

    </div>

    <div class="content-bottom">
      <div class="content-bottom-left"><img alt="Contentedge_l" src="<?php echo FLUENT_PATH;?>/index_files/contentedge_l.png"></div>
      <div class="content-bottom-right"><img alt="Contentedge_r" src="<?php echo FLUENT_PATH;?>/index_files/contentedge_r.png"></div>
      <br style="clear: both;">
    </div>
  </div>

    </div>
    <div id="main-right">
      <div id="sidebar" style="height: 732px; ">
        <div id="sidebar-inner">
	
		<ul>
		<h2><li type=square> Domain </h2> </br>
		<p>  Each domain contains a list of related devices. Select the domain of your choice to display all the data that pertains to the devices which belongs to that particular domain. </p> </br> </br>
		<h2> <li type=square> Devices </h2> </br>
		 <p> In order to view the graph and statistical data of a particular device, click on the device name under the domain    </p></br>
		</br>
		<h2> <li type=square>Graph And Statistical Data </h2> </br>	
		<p> Graph will display the dynamic data in formation and based on the information the statistical data will be displayed </p> </br>
		</br>
		<h2> <li type=square> Statistical Data Summary</h2> </br>
		<p> It displays the total data size receviced, number of devices available and device names </p> </br>
		</ul>

        </div>

        <div class="content-bottom content-bottom-sidebar">
          <div class="content-bottom-left"><img alt="Contentedge_l" src="<?php echo GRAYLOG_URL;?>/index_files/contentedge_l.png"></div>
          <div class="content-bottom-right"><img alt="Contentedge_r" src="<?php echo GRAYLOG_URL;?>/index_files/contentedge_r.png"></div>
          <br style="clear: both;">
        </div>
      </div>
    </div>

  <br style="clear: both;"><br>


</body></html>
