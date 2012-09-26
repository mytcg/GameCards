<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.0//EN" 
"http://www.wapforum.org/DTD/xhtml-mobile10.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
include('dbconnection.php');

//SEND MAIL FUNCTION
function sendEmail($sEmailAddress,$sFromEmailAddress,$sSubject,$sMessage){
	$sHeaders='From: '.$sFromEmailAddress;
	mail($sEmailAddress,$sSubject,$sMessage,$sHeaders);
	return;
}

function myqu($sQuery) {
	$conn = new dbconnection();
	return $conn->_myqu($sQuery);
}

//Display message for relevant input
function displayInvalidMessage($formFieldType) {
	global $invalidInputType;
	global $validInputType;
	global $formValid;
	if ($formValid[$formFieldType]=='invalid') {
		switch ($formFieldType) {
			case 'email': 
				switch($invalidInputType) {
					case 'noneGiven': echo 'Email not given';
						break;
					case 'doesntExist': echo 'Email does not exist';
						break;
					case 'deactivated': echo 'This user has been deactivated';
						break;
				}
			break;
		}
	} else if ($formValid[$formFieldType]=='valid') {
		switch ($formFieldType) {
			case 'email': 
				switch($validInputType) {
					case 'sent': echo 'Email sent! Password reset!';
						break;
				}
			break;
		}
	} else {
		return true;
	}
}
//MAIL CALL SCRIPT
if (isset($_POST['email']) && $_POST['email']!='') {
	$sEmail = $_POST['email'];

	$aUser=myqu('SELECT * FROM mytcg_user WHERE email_address = "'.$sEmail.'"');
	$iUsercheck = sizeof($aUser);
  
	if ($iUsercheck==1){
		if($aUser[0]['is_active']=="1"){
       
			//RANDOM GENERATED PASSWORD
			$password=substr(time()*rand(5,25), -6);
			$iUserID=intval($aUser[0]["user_id"]);
			$iMod=($iUserID % 10)+1;
			$sSalt=substr(md5($iUserID),$iMod,10);
			$sPassword = $sSalt.md5($password);
       
			$a = myqu("UPDATE mytcg_user SET password = '".$sPassword."' WHERE user_id = ".$iUserID);
       
			sendEmail($aUser[0]['email_address'], 'info@mytcg.net', 'Password Update - Mobile Game Card Applications',
				    'So you forgot your password. Don\'t worry, it happens to everyone at some point.
				
				We have randomly generated a new password for you to use. 
				    
				If you are not happy with this password, feel free to change it to whatever you want in the `Profile` section of the site.
				Make sure you log in with this new password first to get to that screen.
				
				Username: '.$aUser[0]['username'].'
				New Password: '.$password);
			
			$formValid['email'] = 'valid';
			$validInputType = 'sent';
		}else{
			$formValid['email'] = 'invalid';
			$invalidInputType = 'deactivated';
		}
	}else {
		$formValid['email'] = 'invalid';
		$invalidInputType = 'doesntExist';
	}
}
else if (isset($_POST['email']) && $_POST['email']=='') {
	$formValid['email'] = 'invalid';
	$invalidInputType = 'noneGiven';
}
?>
<head>
<title>Mobidex Password Recovery</title>
<meta http-equiv="content-type" content="application/xhtml+xml" />
<meta http-equiv="cache-control" content="max-age=300" />
<style>body{
	margin:0px 6px 0px 0px;
	font-size:14px;
	color:#BEBEBE;
	background-color:#252525;
	padding:5px;
	margin-top: 30px;
}
#head{
	width:100%;
	text-align:center;
}
</style>
</head>
<body>
	<div id="head">
	   <form action="index.php" method="POST"><br/>
       		<div>
				<img src="icon.png"></img>
	  			<p>Enter your Email Address</p>
	  			<input name="email" value="" type="text">
	  			<p><?php displayInvalidMessage('email'); ?></p>
	  		</div>
	  		<input type="submit" value="Continue" />
		</form>
	</div>	
</body>
</html>