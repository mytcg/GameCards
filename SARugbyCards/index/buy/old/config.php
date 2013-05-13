<?php
$Conf=array();
/** GLOBAL SETTINGS HERE, for administrator to modify */
//Point to dev

$Conf["database"]["host"]="localhost";
$Conf["database"]["databasename"]="gamecard";
$Conf["database"]["username"]="root";
$Conf["database"]["password"]="";
$Conf["database"]["table_prefix"]="mytcg";

// $Conf["database"]["host"]="dedi94.flk1.host-h.net";
// $Conf["database"]["databasename"]="gamecard";
// $Conf["database"]["username"]="mytcg_root";
// $Conf["database"]["password"]="g4m3c4rd98";
// $Conf["database"]["table_prefix"]="mytcg";

$Conf["files"]["directory_root"]="";
$Conf["files"]["directory_var"]="var/";
$Conf["files"]["file_sql_log"]="sqlq.log";

$Conf["system"]["session_expire_in_minutes"]="20";
$Conf["system"]["ssl_is_on"]="0";
$Conf["system"]["rootdir"]="";

/* NO MORE CONFIGURABLE SETTINGS BELOW THIS LINE */		
$sHTTP = ($Conf["system"]["ssl_is_on"]=="1") ? "https://" : "http://"; 
$Conf["system"]["web_url"]=$sHTTP.$_SERVER["HTTP_HOST"];
$Conf["user"]["logged_in"]="0";
$Conf["user"]["ip"]=$_SERVER["REMOTE_ADDR"];
$Conf["user"]["user_agent"]=$_SERVER["HTTP_USER_AGENT"];

ini_set("session.gc_maxlifetime",intval($Conf["system"]["session_expire_in_minutes"])*60);

?>
