<?php

require_once("functions.php");

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Soon to be the new home of: buy.mytcg.net</title>
<link rel="stylesheet" type="text/css" href="css/mytcgbuy.css" />
<link rel="stylesheet" href="css/mytcgbuy_bigscreen.css" type="text/css" media="screen and (min-width:310px)" />
</head>

<?php

//Display message if form input invalid

function displayInvalidMessage($formFieldType) {
	global $invalidInputType;
	global $formValid;
	if ($formValid[$formFieldType]=='invalid') {
		switch ($formFieldType) {
			case 'username': 
				switch($invalidInputType) {
					case 'noneGiven': echo 'username not given';
						break;
					case 'doesntExist': echo 'username does not exist';
						break;
					case 'inputIncorrect': echo 'Incorrect username';
						break;
				}
			break;
		}
	} else {
		return true;
	}
}

//select correct price as per item

function price($item) {
	switch ($item) {
		case '350': echo '5';
			break;
		case '700': echo '10';
			break;
		case '1050': echo '15';
	} 
}

//test display all keys & values in post superglobal array

// foreach ($_POST as $key => $value) {
	// echo $key.': '.$value.'<br>';
// }

//Check if username exists

if (isset($_POST['username']) && $_POST['username']!='') {
	
   $query = "SELECT user_id, username FROM " . $pre . "_user WHERE username='" . $_POST['username'] . "'";
	$result = myqu($query);
	
	if ($result==true) {
		
		$formValid['username'] = 'valid';
		$validUserName = $result[0]['username'];
		$userID = $result[0]['user_id'];
		// echo 'valid User';
		// echo '<br>';
		// echo 'userID: ';
		// echo($result[0]['user_id']);
		// echo '<br>';
	}  elseif ($result==false) {
		$formValid['username'] = 'invalid';
		$invalidInputType = 'doesntExist';
	}

} else if (isset($_POST['username']) && $_POST['username']=='') {
	$formValid['username'] = 'invalid';
	$invalidInputType = 'noneGiven';
}

if (isset($_POST['payment']) && ($_POST['payment']!='sms') && ($formValid['username']=='valid')) {
	$referenceNumber = paymentGateway(); 
	// echo 'getRefNumberResult: '.$getRefNumberResult;
	// echo '<br>reference number: '.$referenceNumber;
	}

if (isset($_POST['payment']) && ($formValid['username']=='valid') && ($getRefNumberResult=='success'||$_POST['payment']=='sms')) {
	
	//include relevant confirmation page depending on payment type
	
	$confirmationPage = $_POST['payment'];
	switch ($confirmationPage) {
		case 'sms':include 'sms.php';
			break;
		case 'paypal':include 'paypal.php';
			break;
		case 'creditcard':include 'creditcard.php';
			break;
	  }
	
} else {

?>

<body>
  <div id="header-image"><img src="images/crosslogo_mytcg.png" width="100%"></div>
  <div id="form-container">
  		<?php if ($getRefNumberResult == 'failed') { ?>
  	  <div id="noService">
	  				Unfortunately the transaction cannot be processed at this time. We apologize. Please try again later. 
	  </div>
	  	<?php } else { ?>
	  <form id="payment-methods" action="index.php" method="post">
	  		<fieldset id="usernameInput">
	  			<legend for="recipientUsername">Username of Credits Recipient</legend>
	  			<input name="username" id="recipientUsername" type="text">
	  			<div class="invalidInputMessage"><?php displayInvalidMessage('username'); ?></div>
	  		</fieldset>
	  		<fieldset id="select-package">
	  			<legend><span>Select Package</span></legend>
	  			<div class="option-container">
	  				<div class="option">
		  				<div class="icon"></div>
		  				<input size='20em' type="radio" name="item" id="price-point-1" value="350" checked="checked"/>
		  				<label for="price-point-1">350<span> TCG</span></label>
		  				<div class="price">R5.00</div>
	  				</div>
	  			</div>
	  			<div class="option-container">
		  			<div class="option">
		  				<div class="icon"></div>
		  				<input type="radio" name="item" id="price-point-2" value="700"/>
		  				<label for="price-point-2">700<span> TCG</span></label>
		  				<div class="price">R10.00</div>
		  			</div>
	  			</div>
	  			<div class="option-container">
		  			<div class="option">
		  				<div class="icon"></div>
		  				<input type="radio" name="item" id="price-point-3" value="1050"/>
		  				<label for="price-point-3">1050<span> TCG</span></label>
		  				<div class="price">R15.00</div>
		  			</div>
	  			</div>
	  		</fieldset>
	  		<fieldset  id="select-payment-method">
	  			<legend><span>Select Payment Method</span></legend>
	  			<div class="option-container">
		  			<div class="option">
		  				<input type="radio" name="payment" id="sms-payment" value="sms" checked="checked"/>
		  				<label for="sms-payment">SMS</label>
		  				<div class="payment-logo" id="sms-payment-logo"></div>
		  			</div>
	  			</div>
	  			<div class="option-container">
		  			<div class="option">
		  				<input type="radio" name="payment" id="paypal" value="paypal"/>
		  				<label for="paypal">PayPal</label>
		  				<div class="payment-logo" id="paypal-payment-logo"></div>
		  			</div>
	  			</div>
	  			<div class="option-container">
		  			<div class="option">
		  				<input type="radio" name="payment" id="credit-card" value="creditcard"/>
		  				<label for="credit-card">Credit Card</label>
		  				<div class="payment-logo" id="creditcard-payment-logo"></div>
		  			</div>
	  			</div>
	  		</fieldset>
	  		<div id="check-out-container">
	  			<input type="submit" value="Check Out" id="check-out"/>
	  		</div>
	  		</form>	
	  	<?php } ?>
  </div>
</body>

<?php 
 }
?>
</html>