<head>
	<link rel="stylesheet" type="text/css" href="css/stylesheet.css" />
</head>

<body>
<div id="confirm-container">
	<div id="creditcard-logo">
	</div>
	<div class="credit-amount"><?php echo $_POST['item'];?>
		<span>TCG</span>
	</div>
	<div id="creditcard-price">R
	<?php 
		price($_POST['item']); 
		echo '.00';
	?></div>
	<form action="https://www.vcs.co.za/vvonline/ccform.asp" method="POST" id="frmCreditCard">
		<input type="hidden" value="<?php price($_POST['item']); ?>" name="cost" />
		<input type="hidden" value="<?php echo $_POST['item'];?>" name="amount" />
		<input type="hidden" value="1" name="payment" />
		<input type="hidden" value="creditcard" name="gateway" />
		<input type="hidden" value="<?php echo $userID; ?>" name="userID" />
		<input type="hidden" value="8043" name="p1">
		<input type="hidden" value="<?php echo $referenceNumber; ?>" id="referenceNumber" name="p2">
		<input type="hidden" value="<?php echo $_POST['item'];?> TCG credits" name="p3">
		<input type="hidden" value="<?php echo price($_POST['item']); ?>" name="p4">
		<input type="hidden" value="http://mytcg.net/vcs/cancel/" name="p10">
		<input type="hidden" value="N" name="Budget">
		<input type="hidden" value="350" name="m_1">
		<input type="submit" value="Pay by Credit Card" class="cmdButton" id="cmdPayByCreditCard">
	</form>
	<div id="credit-cards-image"></div>
</div>
<div id="closeCheckout" class="cmdButton"><a href="index.php">Back</a></div>
</body>