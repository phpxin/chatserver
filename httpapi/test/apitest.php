<?php

$do = 'login' ;

if ($do == 'getInfo'){

    $inputs['uid'] = '1' ;
    $url ='http://localhost:8080/index.php?module=user&action=getInfo' ;

}else if($do == 'login'){
    $inputs['account'] = 'lx' ;
    $inputs['pwd']= '123' ;
    $url ='http://localhost:8080/index.php?module=user&action=login' ;
}else{
    echo '未找到action' ;
}



$req = curl_init($url) ;

curl_setopt($req, CURLOPT_POST, 1) ;
curl_setopt($req, CURLOPT_RETURNTRANSFER, 1) ;
curl_setopt($req, CURLOPT_POST, 1);
curl_setopt($req, CURLOPT_POSTFIELDS, $inputs);

$reqRet = curl_exec($req) ;

echo $reqRet ;
