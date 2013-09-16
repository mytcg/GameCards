<?php

function getPhoneOSDistribution($os) {
	
	switch($os) {
		case "android_3": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%android_3%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result0 = myqu($sql);
			return $result0[0][0];
		case "android_4": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%android_4%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result1 = myqu($sql);
			return $result1[0][0];
		case "android_7": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%android_7%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result2 = myqu($sql);
			return $result2[0][0];
		case "blackberry": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%blackberry%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result3 = myqu($sql);
			return $result3[0][0];
		case "iphoneos": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%iphoneos%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result4 = myqu($sql);
			return $result4[0][0];
		case "JavaME": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%JavaME%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result5 = myqu($sql);
			return $result5[0][0];
		case "symbian_s60v3": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%s60v3%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result6 = myqu($sql);
			return $result6[0][0];
		case "symbian_s60v5": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%s60v5%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result7 = myqu($sql);
			return $result7[0][0];
		case "WndowsPhoneOS": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%Windows%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result8 = myqu($sql);
			return $result8[0][0];
		case "MTKNucleusOSJ2ME": 
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE os LIKE '%J2ME%' AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83))) GROUP BY os";
			$result9 = myqu($sql);
			return $result9[0][0];
		case "iOS":
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE (os LIKE '%iOS_%' OR os LIKE '%iphoneos%') AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83)))";
			$result10 = myqu($sql);
			return $result10[0][0];
		case "unknown":
			$sql = "SELECT count(distinct user_id) FROM mytcg_userphone WHERE (os LIKE '%temp%' OR os LIKE '%unknown%') 
			AND user_id IN (select distinct uc.user_id
			from mytcg_usercard uc
			where uc.card_id in (select c.card_id from mytcg_card c where c.category_id in (82,83)))
			AND user_id NOT IN (select distinct user_id from mytcg_userphone where os not like '%temp%' and os not like '%unknown%')";
			$result10 = myqu($sql);
			return $result10[0][0];
	}
}

function getTotalVoters() {
	$sql = "select count(distinct d.user_id)
		from mytcg_deckcard dc
		inner join mytcg_deck d
		on d.deck_id = dc.deck_id
		where d.category_id = 85;";
	$result = myqu($sql);
	return $result[0][0];
}

function getTotalUsers(){
	$sql = "select count(*)
		from mytcg_user where apps_id = 7;";
	$result = myqu($sql);
	return $result[0][0];
}

function getMobileUsers(){
	$sql = "select count(*)
		from mytcg_user where apps_id = 7 
		and mobile_date_last_visit is not null;";
	$result = myqu($sql);
	return $result[0][0];
}

function getWebUsers(){
	$sql = "select count(*)
		from mytcg_user where apps_id = 7 
		and web_date_last_visit is not null;";
	$result = myqu($sql);
	return $result[0][0];
}

function getFacebookUsers(){
	$sql = "select count(*)
		from mytcg_user where apps_id = 7 
		and facebook_date_last_visit is not null;";
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