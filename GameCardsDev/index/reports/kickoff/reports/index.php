<?php

require_once("configuration.php");
require_once("functions.php");

?>

<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title>Kickoff Reporting</title>
    <link rel="icon" href="" />
    <link href="stylesheet.css" type="text/css" rel="stylesheet" media="all" />
  </head>
  <body>
  	 <h1>Kickoff Reporting</h1>
  	 <table>
  	 	<tr>
  	 		<td>Total Kickoff Users:</td>
  	 		<td><?php echo (getTotalKickoffMobileUsers() + getTotalKickoffWebUsers() + getTotalKickoffFacebookUsers()); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Mobile Users:</td>
  	 		<td><?php echo getTotalKickoffMobileUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Web Users:</td>
  	 		<td><?php echo getTotalKickoffWebUsers(); ?></td>
  	 	</tr>
		<tr>
  	 		<td>Total Facebook Users:</td>
  	 		<td><?php echo getTotalKickoffFacebookUsers(); ?></td>
  	 	</tr>
			
  	 </table>
  	 
	 <h2>Kickoff Votes</h2>
	 <table>
  	 	<tr>
  	 		<td>Total Voters:</td>
  	 		<td><?php echo getTotalVoters(); ?></td>
  	 	</tr>
	</table>
	 
	 <table>
		<?php echo getVotes(); ?>
	 </table>
		
	 
  	 <h2>Distribution per OS</h2>
  	 <table>
  	 	<tr>
  	 		<td>Android Platform 3</td>
  	 		<td><?php echo getPhoneOSDistribution("android_3"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Android Platform 4</td>
  	 		<td><?php echo getPhoneOSDistribution("android_4"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Android Platform 7</td>
  	 		<td><?php echo getPhoneOSDistribution("android_7"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Symbian S60v3</td>
  	 		<td><?php echo getPhoneOSDistribution("symbian_s60v3"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Symbian S60v5</td>
  	 		<td><?php echo getPhoneOSDistribution("symbian_s60v5"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Blackberry</td>
  	 		<td><?php echo getPhoneOSDistribution("blackberry"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>JavaME</td>
  	 		<td><?php echo getPhoneOSDistribution("JavaME"); ?></td>
  	 	</tr>
		<tr>
  	 		<td>Apple iPhone</td>
  	 		<td><?php  echo getPhoneOSDistribution("iOS"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Windows Phone OS</td>
  	 		<td><?php  echo getPhoneOSDistribution("WndowsPhoneOS"); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>MTK/Nucleus OS (J2ME)</td>
  	 		<td><?php  echo getPhoneOSDistribution("MTKNucleusOSJ2ME"); ?></td>
  	 	</tr>
		<tr>
  	 		<td>Unknown</td>
  	 		<td><?php  echo getPhoneOSDistribution("unknown"); ?></td>
  	 	</tr>
  	 </table>
  	
  	 
  </body>
</html>