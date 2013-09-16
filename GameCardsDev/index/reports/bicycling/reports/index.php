<?php

require_once("configuration.php");
require_once("functions.php");

?>

<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title>Bicycling Reporting</title>
    <link rel="icon" href="" />
    <link href="stylesheet.css" type="text/css" rel="stylesheet" media="all" />
  </head>
  <body>
  	 <h1>Bicycling Reporting</h1>
  	 <table>
  	 	<tr>
  	 		<td>Total Bicycling Users:</td>
  	 		<td><?php echo getTotalUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Mobile Users:</td>
  	 		<td><?php echo getMobileUsers(); ?></td>
  	 	</tr>
  	 	<tr>
  	 		<td>Total Web Users:</td>
  	 		<td><?php echo getWebUsers(); ?></td>
  	 	</tr>
			
  	 </table>
  	 
	 <h2>Bicycling Votes</h2>
	 <table>
  	 	<tr>
  	 		<td>Users who submitted predictions:</td>
  	 		<td><?php echo getTotalVoters(); ?></td>
  	 	</tr>
	</table>
  	 
  </body>
</html>