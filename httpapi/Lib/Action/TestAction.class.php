<?php
/**
 * Created by PhpStorm.
 * 接口测试程序
 * User: lx
 * Date: 16/10/19
 * Time: 下午2:37
 */


class TestAction {
    
    // http://192.168.2.98:8080/index.php?module=test&action=userTest
    public function userTest(){

        $host = '192.168.2.98:8080' ;
        $input = [] ;
        $url = '' ;

        $type = 'inviteList' ;
        $input['authcode'] = '/ZBR8ooB8Qo9jjPCP86zpw==' ; //uid=1
        //$input['authcode'] = '9YFPZ7einYAYWsA4kdyYRQ==' ; //uid = 2

        switch ($type) {
            case 'login' :
                $input['account'] = 'lx1' ;
                $input['pwd'] = '123' ;
                $url ='http://'.$host.'/index.php?module=user&action=login' ;
                break;
            case 'register' :
                $input['account'] = 'lx1' ;
                $input['pwd'] = '123' ;
                $input['repwd'] = '123' ;
                $url ='http://'.$host.'/index.php?module=user&action=register' ;
                break;
            case 'search' :
                $input['keywords'] = 'x' ;
                $url = 'http://'.$host.'/index.php?module=user&action=search' ;
                break;
            case 'invite' :
                $input['fid'] = 2;
                $input['intro'] = '123444444' ;
                $url = 'http://'.$host.'/index.php?module=relation&action=invite' ;
                break;
            case 'dealInvite' :
                $input['id'] = 2 ;
                $input['agree'] = 1 ;
                $url = 'http://'.$host.'/index.php?module=relation&action=dealInvite' ;
                break;
            case 'getList':
                $url = 'http://'.$host.'/index.php?module=user&action=getList' ;
                break;
            case 'getInfo':
                $input['uid'] = 3 ;
                $url = 'http://'.$host.'/index.php?module=user&action=getInfo' ;
                break;
            case 'inviteList' :
                $url = 'http://'.$host.'/index.php?module=relation&action=inviteList' ;
                break;
            default:
                echo 'no action ' ;
                exit();
        }

        $req = curl_init($url) ;

        curl_setopt($req, CURLOPT_POST, 1) ;
        curl_setopt($req, CURLOPT_RETURNTRANSFER, 1) ;
        curl_setopt($req, CURLOPT_POST, 1);
        curl_setopt($req, CURLOPT_POSTFIELDS, $input);

        $reqRet = curl_exec($req) ;
        var_dump($reqRet);


        if (curl_errno($req)){
            echo curl_error($req) ;
            exit();
        }

        $data = json_decode($reqRet, true) ;

        if (empty($data)) {
            dump($reqRet) ;
        }else{
            dump($data) ;
        }

    }
    
}