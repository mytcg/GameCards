<?php

require_once("configuration.php");
require_once("functions.php");

?>

<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title>TopCar Reporting</title>
    <link rel="icon" href="" />
    <link href="stylesheet.css" type="text/css" rel="stylesheet" media="all" />
  </head>
  <body>
  	 <h1>TopCar Reporting</h1>
  	 <table>
  	 	<tr>
  	 		<td>Total Users:</td>
  	 		<td><?php echo getTotalUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Mobile Users:</td>
  	 		<td><?php echo getTotalMobileUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Web Users:</td>
  	 		<td><?php echo getTotalWebUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Full System Users:</td>
  	 		<td><?php echo getTotalFullSystemUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Active Users:</td>
  	 		<td><?php echo getTotalActiveUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Active Web Users:</td>
  	 		<td><?php echo getTotalActiveWebUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Active Mobile Users:</td>
  	 		<td><?php echo getTotalActiveMobileUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total SMS sent for credits:</td>
  	 		<td><?php echo getTotalSMSForCredits(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total PayPal Purchases</td>
  	 		<td><?php echo getTotalPaypalPurchases();  ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total PayPal purchase worth</td>
  	 		<td><?php echo getTotalPaypalPurchasesWorth(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Free to paid % Active</td>
  	 		<td><?php echo getFreeToPaidPercActive();  ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Free to paid % Total</td>
  	 		<td><?php echo getFreeToPaidPercTotal(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Average Spend per user</td>
  	 		<td><?php echo getAveSpendPerUser(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Average Auction activity per user per day</td>
  	 		<td><?php echo getAveAuctActivityPerUserPerDay(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total boosters purchased</td>
  	 		<td><?php echo getTotalBoostersPurchased(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Games Played</td>
  	 		<td><?php echo getTotalGamesPlayed() ; ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Average Actions per User per Day</td>
  	 		<td><?php echo getAveActionsPerUserPerDay(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Average Game Length</td>
  	 		<td><?php echo getAveTimeSpentGaming(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Players answered information</td>
  	 		<td><?php echo getPlayersAnswered(); ?></td>
  	 	</tr>
			
  	 </table>
  	 
  	 <h2>Distribution per OS</h2>
  	 
  	 <?php 
  	 	// $array = (getPhoneOSDistribution(""));
		// $index=0; 
		// foreach ($array as $line){
			// echo $index;
			// print_r($line);
			// echo "<br>";
			// $index++;
		// }
  	 ?>
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
  	 		<td>Nokia Symbian</td>
  	 		<td><?php // echo getPhoneOSDistribution(""); ?></td>
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
  	 </table>
  	
  	 
  </body>
</html>