<?php

echo "llllllll";
$domain_id = $_REQUEST['domain_id'];
echo $domain_id;

switch($domain_id) {
	case 'bp': 
		include_once "bp.php";
	break;
	
	case 'pulseox':
		include_once "pulseox.php";
	break;
	case 'temp':
		include_once "temp.php";
	break;
	default: 
		echo "<H1> WELCOME TO NETSPECTIVE WEB BASED DATA INTELLIGENCE </H1>";
		
	break;
}
?>

