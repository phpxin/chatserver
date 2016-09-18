<?php



$inputs['uid'] = '1' ;
$url ='http://localhost:8080/index.php?module=user&action=getInfo' ;



$req = curl_init($url) ;

curl_setopt($req, CURLOPT_POST, 1) ;
curl_setopt($req, CURLOPT_RETURNTRANSFER, 1) ;
curl_setopt($req, CURLOPT_POST, 1);
curl_setopt($req, CURLOPT_POSTFIELDS, $inputs);

$reqRet = curl_exec($req) ;

echo $reqRet ;
