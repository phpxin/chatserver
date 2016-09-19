<?php

class ApiLoginLogic{

    private static $authKey = '#3b*&#D129Ac)28s' ;
    private static $authIv = '#3b*&#D129Ac)28s' ;


    private static $flag = 'chat' ; // authcode标识符
    private static $timeLimit = 86400 ; // 登录失效时间，暂定一小时


    public static function login($account, $pwd){
        $info = M("user")->checkLogin($account, $pwd);

        if (!$info){
            return false;
        }

        $staff_id = $info['id'];

        $auth = self::getAuthCode($staff_id) ;

        return $auth ;
    }

    public static function authCode($code){


        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');
        mcrypt_generic_init($td, self::$authKey, self::$authIv);

        $code = base64_decode($code) ;
        $data = mdecrypt_generic($td, $code);

        $data = self::pkcs5_unpad($data) ;

        $data = explode('_', $data) ;

        if (count($data) < 3) {
            return false;
        }

        $authcode_flag = $data[0] ;

        if ($authcode_flag != self::$flag){
            return false;
        }


        $ret['uid'] = $data[1] ;
        $ret['lose_time'] = $data[2] ;


        return $ret ;
    }

    public static function getAuthCode($staff_id){

        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');
        mcrypt_generic_init($td, self::$authKey, self::$authIv);

        $lose_time = time() + self::$timeLimit;

        $_authstr = self::$flag.'_'.$staff_id.'_'.$lose_time ;


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