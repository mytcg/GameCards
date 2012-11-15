<head>
	<link rel="stylesheet" type="text/css" href="css/stylesheet.css" />
</head>

<body>
<div id="confirm-container">
	<div id="paypal-logo"></div>
	<form method="post" action="https://www.paypal.com/cgi-bin/webscr" id="frmPaypal">
		<input type="hidden" value="_s-xclick" name="cmd">
		<input type="hidden" value="ZT4SREPVEM5J8" name="hosted_button_id">
		<input name="on0" type="hidden" value="TCG Credits">
		<div>TCG Credits</div>
		<select name="os0">
			<option title="$1.00" alt="350" value="350 TCG Credits">350 TCG Credits $1.00</option>
			<option title="$1.50" alt="700" value="700 TCG Credits">700 TCG Credits $1.50</option>
			<option title="$2.00" alt="1050" value="1050 TCG Credits">1050 TCG Credits $2.00</option>
		</select>
		<input type="hidden" name="on1" value="reference">
		<input type="hidden" name="os1" value="<?php echo $referenceNumber; ?>" id="referenceNumber">
		<input type="hidden" value="USD" name="currency_code">
		<input type="image" alt="PayPal - The safer, easier way to pay online!" name="submit" src="https://www.paypalobjects.com/en_US/i/btn/btn_buynowCC_LG.gif" id="cmdPayByPaypal">
		<img width="1" height="1" src="https://www.paypalobjects.com/en_US/i/scr/pixel.gif" alt="">
	</form>
	
</div>
<div id="closeCheckout" class="cmdButton"><a href="index.php">Back</a></div>
</body>
