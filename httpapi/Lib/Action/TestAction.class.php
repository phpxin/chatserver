<?php
/**
 * Created by PhpStorm.
 * 接口测试程序
 * User: lx
 * Date: 16/10/19
 * Time: 下午2:37
 */


class TestAction extends Action {
    
    
    public function loginTest(){

        $host = '192.168.2.108:8080' ;
        $input['account'] = 'lx' ;
        $input['pwd'] = '123' ;
        $url ='http://'.$host.'/index.php?module=user&action=login' ;

        $req = curl_init($url) ;

        curl_setopt($req, CURLOPT_POST, 1) ;
        curl_setopt($req, CURLOPT_RETURNTRANSFER, 1) ;
        curl_setopt($req, CURLOPT_POST, 1);
        curl_setopt($req, CURLOPT_POSTFIELDS, $input);

        $reqRet = curl_exec($req) ;

        if (curl_errno($req)){
            echo curl_error($req) ;
            exit();
        }

        $data = json_decode($reqRet, true) ;

        dump($data) ;
    }
    
}