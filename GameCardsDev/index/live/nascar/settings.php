<?php
//topcar category constant
include('../functions.php');
$topcar = "78";
$root = "../../";
$invite = 78;
$usercategories = " AND (cx.category_parent_id = 78 OR (cx.category_child_id = 78 AND cx.category_parent_id IS NULL)) ";
$productcategories = " AND c.category_id = 78 ";
$url = "www.mytcg.net";
$appkey = "Nascar";
$transactionlogtypes = "1,2,3";
$notificationtypes = "1,2,3";
?>