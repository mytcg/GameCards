<?php

function getPhoneOSDistribution($os) {
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%android_3%' GROUP BY os";
	$result0 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%android_4%' GROUP BY os";
	$result1 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%android_7%' GROUP BY os";
	$result2 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%blackberry%' GROUP BY os";
	$result3 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%iphoneos%' GROUP BY os";
	$result4 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%JavaME%' GROUP BY os";
	$result5 = myqu($sql);
	
	// $sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%symbian'";
	// $result = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%s60v3%' GROUP BY os";
	$result6 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%s60v5%' GROUP BY os";
	$result7 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%Windows%' GROUP BY os";
	$result8 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%J2ME%' GROUP BY os";
	$result9 = myqu($sql);
	
	$sql = "SELECT COUNT(*),os FROM mytcg_userphone WHERE os LIKE '%iOS_%' OR os LIKE '%iphoneos%'";
	$result10 = myqu($sql);
	
	switch($os) {
		case "android_3": return $result0[0][0];
		case "android_4": return $result1[0][0];
		case "android_7": return $result2[0][0];
		case "blackberry": return $result3[0][0];
		case "iphoneos": return $result4[0][0];
		case "JavaME": return $result5[0][0];
		case "symbian_s60v3": return $result6[0][0];
		case "symbian_s60v5": return $result7[0][0];
		case "WndowsPhoneOS": return $result8[0][0];
		case "MTKNucleusOSJ2ME": return $result9[0][0];
		case "iOS": return $result10[0][0];
		default:return $result;
	}
}

function getPlayersAnswered() {
	$sql = "select count(*) as answered from (select distinct user_id from mytcg_user_answer where answered = 1) as a;";
	$result = myqu($sql);
	return $result[0][0];
}


function getAveTimeSpentGaming(){
	$sql = "SELECT AVG(length) FROM 
(select * from 
		(SELECT MAX(gl.date) - g.date_created length
		FROM mytcg_gamelog gl
		INNER JOIN mytcg_game g
		ON g.game_id = gl.game_id
		WHERE g.gamestatus_id = 2
		GROUP BY g.game_id) as game_lengths
where length < 100) as game_len;";
	$result = myqu($sql);
	return $result[0][0];
}

function getAveActionsPerUserPerDay(){
	$sql = "SELECT AVG(activity_per_user) FROM (SELECT user_id, COUNT(notification) AS activity_per_user FROM mytcg_notifications GROUP BY user_id) as activity";

	$result = myqu($sql);
	return $result[0][0];
}

function getTotalGamesPlayed(){
	$sql = "SELECT COUNT(*)+3800 FROM mytcg_game";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalBoostersPurchased(){
	$sql = "SELECT COUNT(transaction_id) FROM mytcg_transactionlog WHERE description LIKE '%booster%'";
	$result = myqu($sql);
	return $result[0][0];
}

function getAveAuctActivityPerUserPerDay(){
			
	$sql = "SELECT AVG(activity_p_day_p_user) 
			FROM (
				SELECT user_id, 
				DATE_FORMAT(date,'%y-%m-%d') AS activity_date,
				COUNT(user_id) AS activity_p_day_p_user
				FROM mytcg_transactionlog 
				WHERE (
					description LIKE '%bid%' 
					OR description LIKE '%win%' 
					OR description LIKE '%won%' 
					OR description LIKE '%creat%' 
					OR description LIKE '%bought%'
					) 
				AND description NOT LIKE '%refund%'
				AND user_id=122
				GROUP BY activity_date
			) AS activity";
				
	$result = myqu($sql);
	return $result[0][0];
}

function getAveSpendPerUser(){
	
	//this is only good for average credits spend per user
	// $sql = "SELECT AVG(avg_val) 
				// FROM (
					// SELECT -(AVG(val)) as avg_val 
					// FROM mytcg_transactionlog 
					// WHERE 0>val 
					// GROUP BY user_id
					// ) as avgs";
					
	$sql = "SELECT COUNT(DISTINCT(user_id)) as users_who_spent
 				FROM mytcg_transactionlog 
 				WHERE description LIKE '%purchase%' 
 				OR description LIKE '%sms%' 
 				OR description LIKE '%credit card%' 
 				OR description LIKE '%paypal%'";
					
	$result = myqu($sql);
	return $result[0][0];
}

function getFreeToPaidPercActive(){
	$sql = "SELECT (COUNT(DISTINCT(tl.user_id)))+161 AS spending_users 
		FROM mytcg_transactionlog tl
		INNER JOIN mytcg_user u ON tl.user_id=u.user_id
		WHERE tl.description LIKE '%paypal%' 
		OR tl.description LIKE '%sms%' 
		OR tl.description LIKE '%Credit Card%'
		AND u.is_active=1";
	$result1 = myqu($sql);
	
	$sql = "SELECT COUNT(*) FROM mytcg_user WHERE (is_active = 1)";
	$result2 = myqu($sql);
	
	$percentage = (($result1[0][0] / $result2[0][0])*100);
	$percentage = number_format($percentage, 2, '.', '')."%";
	
	return $percentage;
}

function getFreeToPaidPercTotal(){
	$sql = "SELECT (COUNT(DISTINCT(user_id)))+161 AS spending_users 
		FROM mytcg_transactionlog 
		WHERE description LIKE '%paypal%' 
		OR description LIKE '%sms%' 
		OR description LIKE '%Credit Card%';";
	$result1 = myqu($sql);
	
	$sql = "SELECT COUNT(*) FROM mytcg_user; ";
	$result2 = myqu($sql);
	
	$percentage = (($result1[0][0] / $result2[0][0])*100);
	$percentage = number_format($percentage, 2, '.', '')."%";
	
	return $percentage;
}

function getTotalPaypalPurchasesWorth(){
	$sql = "SELECT SUM(payment_amount) FROM mytcg_paypal WHERE (status = 'SUCCESFULL')";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalPaypalPurchases(){
	$sql = "SELECT COUNT(transaction_id) FROM  mytcg_transactionlog WHERE transactiontype_id=3 AND transactionstatus_id=1";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalSMSForCredits(){
	// $sql = "SELECT COUNT(transaction_id) FROM  mytcg_transactionlog WHERE transactiontype_id=2";
	$sql = "SELECT COUNT(*)+161 FROM mytcg_transactionlog WHERE description LIKE '%sms%' AND description LIKE '%purchase%'";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalActiveFullSystemUsers(){
	$sql = "SELECT COUNT(user_id) FROM mytcg_user WHERE mobile_date_last_visit IS NOT NULL AND date_last_visit IS NOT NULL AND (is_active = 1)";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalActiveMobileUsers(){
	
	//SQL query written by nick
	// $sql = "SELECT COUNT(user_id) 
				// FROM mytcg_user 
				// WHERE mobile_date_last_visit IS NOT NULL 
				// AND (is_active = 1)";
				
	$sql = "SELECT COUNT(*) active_mobile_users
		FROM (SELECT u.user_id, u.credits, u.gameswon, u.mobile_date_last_visit, COUNT(tl.transaction_id) transactions
		FROM mytcg_user u
		INNER JOIN mytcg_transactionlog tl
		ON tl.user_id = u.user_id
		WHERE u.ai = 0
		GROUP BY u.user_id) user_transactions 
		WHERE (credits <> 300 
		OR gameswon <> 0
		OR transactions > 1)
		AND mobile_date_last_visit IS NOT NUlL";
	
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalActiveWebUsers(){
	
	//SQL query written by nick
	// $sql = "SELECT COUNT(user_id) 
				// FROM mytcg_user 
				// WHERE date_last_visit IS NOT NULL 
				// AND (is_active = 1)";
				
	$sql = "SELECT COUNT(*) active_web_users
				FROM (SELECT u.user_id, u.credits, u.gameswon, u.date_last_visit, COUNT(tl.transaction_id) transactions
				FROM mytcg_user u
				INNER JOIN mytcg_transactionlog tl
				ON tl.user_id = u.user_id
				WHERE u.ai = 0
				GROUP BY u.user_id) user_transactions 
				WHERE (credits <> 300 
				OR gameswon <> 0
				OR transactions > 1)
				AND date_last_visit IS NOT NULL;";
				
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalActiveUsers(){
	//$sql = "SELECT COUNT(user_id) FROM mytcg_user WHERE (is_active = 1)";
	
	$sql = "SELECT COUNT(*) active_users
				FROM (SELECT u.user_id, u.credits, u.gameswon, COUNT(tl.transaction_id) transactions
				FROM mytcg_user u
				INNER JOIN mytcg_transactionlog tl
				ON tl.user_id = u.user_id
				WHERE u.ai = 0
				GROUP BY u.user_id) user_transactions 
				WHERE credits <> 300 
				OR gameswon <> 0
				OR transactions > 1";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalFullSystemUsers(){
	$sql = "SELECT COUNT(user_id) FROM mytcg_user WHERE mobile_date_last_visit IS NOT NULL AND date_last_visit IS NOT NULL";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalMobileUsers(){
	$sql = "SELECT COUNT(user_id) FROM mytcg_user WHERE mobile_date_last_visit IS NOT NULL";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalWebUsers(){
	$sql = "SELECT COUNT(user_id) FROM mytcg_user WHERE date_last_visit IS NOT NULL";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalUsers(){
	$sql = "SELECT COUNT(user_id) FROM mytcg_user";
	$result = myqu($sql);
	return $result[0][0];
}

// execute mysql query and log, return in associative array 
function myqu($sQuery){   
  global $db;
  $aOutput=array();
  $pattern = '/INSERT/i';
  
  $aLink=mysqli_connect($db["host"],$db["username"],$db["password"],$db["database"]);
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
    echo("Err:".mysqli_errno($aLink)." - ".mysqli_error($aLink)." - ".$sQuery."\r\n");
    @mysqli_free_result($aResult);
    mysqli_close($aLink);
  }
}

?>