<?php

class ApiLoginLogic{

    private static $authKey = '0123456789abcdeF' ;
    private static $authIv = '1234567887654321' ;

    private static $timeLimit = 86400 ; // 登录失效时间，暂定一小时


    public static function login($account, $pwd){
        $info = M("user")->checkLogin($account, $pwd);

        if (!$info){
            return false;
        }

        $uid = $info['id'];

        $auth = self::getAuthCode($uid) ;

        return $auth ;
    }

    public static function authCode($code){


        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');
        mcrypt_generic_init($td, self::$authKey, self::$authIv);

        $code = base64_decode($code) ;
        $data = mdecrypt_generic($td, $code);

        $data = self::pkcs5_unpad($data) ;

        $data = explode('_', $data) ;

        if (count($data) < 2) {
            return false;
        }


        $ret['uid'] = $data[0] ;
        $ret['lose_time'] = $data[1] ;


        return $ret ;
    }

    public static function getAuthCode($uid){

        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');
        mcrypt_generic_init($td, self::$authKey, self::$authIv);

        $lose_time = time() + self::$timeLimit;

        $_authstr = $uid.'_'.$lose_time ;


        $blockSize = mcrypt_enc_get_block_size($td);
        $_authstr = self::pkcs5_pad($_authstr, $blockSize);
        $encrypted = mcrypt_generic($td, $_authstr);
        $mdata = base64_encode($encrypted);

        return $mdata ;

    }

    /**
     * PKCS5PADDING 标准，数据补齐
     * @param $text
     * @param $blocksize
     * @return string
     */
    public static function pkcs5_pad ($text, $blocksize)
    {
        $pad = $blocksize - (strlen($text) % $blocksize);
        return $text . str_repeat(chr($pad), $pad);
    }

    /**
     * PKCS5PADDING 标准，去掉数据补齐
     * @param $text
     * @return bool|string
     */
    public static function pkcs5_unpad($text)
    {
        $pad = ord($text{strlen($text)-1});
        if ($pad > strlen($text)) return false;
        if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) return false;
        return substr($text, 0, -1 * $pad);
    }

}