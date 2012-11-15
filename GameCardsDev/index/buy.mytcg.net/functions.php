<?php
require_once("config.php");

//SETUP PREFIX FOR TABLES
$pre = $Conf["database"]["table_prefix"];

//LOGGED IN USER ID
$userID = $_POST['userID'];

function myqu($sQuery){	  
	global $Conf;
  $db = $Conf["database"];
  $aOutput=array();
  $pattern = '/INSERT/i';
  
	$aLink=mysqli_connect($db["host"],$db["username"],$db["password"],$db["databasename"]);
	$sQuery=str_replace("&nbsp;","",$sQuery);
	$sQueryCut=substr($sQuery,0,1500);
  
	if($aResult=@mysqli_query($aLink, $sQuery))
	{
		//If insert - return last insert id
		if(preg_match($pattern, $sQuery)){
			$mp = mysqli_insert_id($aLink);
			@mysqli_free_result($aResult);
      mysqli_close($aLink);
			return $mp;
		}
    //Else build return array
		while ($aRow=@mysqli_fetch_array($aResult,MYSQL_BOTH)){
			$aOutput[]=$aRow;
		}
		return $aOutput;
	}
  else{
    // $aFileHandle=fopen("/usr/www/users/mytcga/var/sqlq.log","a+");
    // fwrite($aFileHandle,"[".date("Y-m-d H:i:s")."] Err:".mysqli_errno($aLink)." - ".mysqli_error($aLink)." - ".$_SERVER['PHP_SELF']."\r\n");
    // fclose($aFileHandle);
    $aFileHandle = null;
    @mysqli_free_result($aResult);
    mysqli_close($aLink);
  }
}

function addTransaction($type, $gateway, $amount, $cost, $userID, $pre) {
	
	//insert new transaction log and set to submitted (status=1)
	$sql = "INSERT INTO mytcg_transactionlog(user_id,description,date,val,transactiontype_id,transactionstatus_id, transactionlogtype_id) VALUES (
				{$userID},
				'Purchase {$amount} TCG credits via {$gateway} for {$cost}',
				NOW(),
				{$amount},
				{$type},
				1,
				2
			)";
	myqu($sql);
	
	//get and return unique transactionlog_id for transaction reference number to be passed to payment gateway
	$sql = "SELECT MAX(transaction_id) AS 'ref' FROM mytcg_transactionlog
				WHERE user_id={$userID}
				AND transactiontype_id={$type}
				AND transactionstatus_id=1";
	$query = myqu($sql);
	
	return $query[0]['ref'];
	echo '<br>query result: '.$query[0]['ref'];
}

/*
 * New payment gateway transaction
 */
function paymentGateway() {
	
	global $userID;
	global $getRefNumberResult;
	
	$item = $_POST['item'];
	
	switch ($item) {
		case '350': $cost = '5';
			break;
		case '700': $cost = '10';
			break;
		case '1050': $cost = '15';
		} 

	if(isset($_POST['payment'])) {
		
		global $pre;
		$amount = $_POST['item'];
		$getRefNumberResult = 'success';
		$type = '1';
		
		switch($_POST['payment'])
		{
			case 'sms':
				$type = '2';
				$gateway = 'SMS';
				$cost = 'R'.$cost.'.00';
				break;
			case 'paypal':
				switch ($item) {
					case '350': $cost = '1';
						break;
					case '700': $cost = '1.50';
						break;
					case '1050': $cost = '2';
						break;
				} 
				$type = '3';
				$gateway = 'PayPal';
				$cost = '$'.$cost.'.00';
				break;
			case 'creditcard':
				$type = '4';
				$gateway = 'Credit Card';
				$cost = 'R'.$cost.'.00';
				break;
		}
		
		// echo '<br>';	
		// echo 'hello';
		// echo '<br>';
		// echo 'type: '.$type;
		// echo '<br>';
		// echo 'gateway: '.$gateway;
		// echo '<br>';
		// echo 'amount: '.$amount;
		// echo '<br>';
		// echo 'cost: '.$cost;
		// echo '<br>';
		// echo 'userID: '.$userID;
		// echo '<br>';
		// echo 'prefix: '.$pre;
		// echo '<br>';
		
		$referenceNumber = addTransaction($type,$gateway,$amount,$cost,$userID,$pre);
		
		if(is_null($referenceNumber))
		{
			$getRefNumberResult = 'failed';
		}
		return $referenceNumber;
	}
}

?>