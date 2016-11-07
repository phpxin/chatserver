<?php

class ApiTools{

    const CODE_SUCCESS = 0 ;

    // 10 段：公用异常
    const CODE_MSG = 10000 ; //标准错误
    const CODE_ERR_LOGIN = 10001 ; //登录异常
    const CODE_ERR_LOST_AUTHCODE = 250;//缺少用户验证码
    const CODE_ERR_USER = 10002;//用户异常
    const CODE_ERR_INVALID = 10003;//登录失效

    // 20 段：逻辑异常
    const CODE_ERR_LACK_FIELD = 20001;//缺少关键字段
    const CODE_ERR_NOT_FOUND = 20002; //未找到结果


    // 30 段：服务器错误
    const CODE_ERR_SERVER = 20001 ; //请求第三方接口失败
    const CODE_ERR_DATA =  10007;//数据型错误

    private $iv ;    // 编码的加密用向量
    private $key ;   // 编码的加密用key
    private static $sss ;   // 手机端加密标志

    private static $inst = null ;

    //***********************  基础功能区

    /**
     * 单利模式
     * ApiTools constructor.
     */
    private function __construct()
    {

    }

    private function __clone(){

    }

    public static function inst(){
        if (!self::$inst || !(self::$inst instanceof self)){
            self::$inst = new self();
        }

        return self::$inst ;
    }

    public static function getRequestParams(){

        return $GLOBALS['safePostParam'] ;


        if (empty($request)){
            $request = request() ;
        }

        $mKey = $request->get('key');
        $mIv = $request->get('iv') ;
        $mData = $request->get('data') ;

        $sss = $request->get('sss') ;

        $data = self::inst()->decodeRequest($sss, $mKey, $mIv, $mData) ;
        return $data ;
    }

    /**
     * 成功
     * @param mixed $data 数据
     * @param bool $encode 是否加密
     * @return \Illuminate\Http\JsonResponse
     */
    public static function success($data, $encode=true){

        $ret = ['code' => self::CODE_SUCCESS, 'data'=>$data] ;

        return self::_toJson($ret, $encode) ;
    }

    /**
     * 失败
     * @param int $code 错误码
     * @param string $msg 错误信息
     * @param bool $encode 是否加密
     * @return \Illuminate\Http\JsonResponse
     */
    public static function error($code, $msg='', $encode=true){

        if(!is_string($msg)){
            $msg = json_encode($msg) ;
        }

        $ret = ['code' => $code, 'msg'=>$msg ];

        return self::_toJson($ret, $encode) ;
    }

    private static function _toJson($ret, $encode){

        echo json_encode($ret, JSON_UNESCAPED_UNICODE) ;
        exit();

        //return response()->json($ret);
        if (!$encode){
            return response()->json($ret);
        }

        $enData = self::inst()->encodeResponse($ret) ;
        $enRet = [
            'sss' => self::$sss ,
            'data' => $enData
        ] ;

        return response()->json($enRet);
    }


    //******************    加密功能区

    /**
     * 对标准加密返回协议数据加密
     * @param $data
     * @return string
     */
    public function encodeResponse($data){

        if (is_array($data)) {
            $data = json_encode($data) ;
        }

        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');

        $blockSize = mcrypt_enc_get_block_size($td);

        mcrypt_generic_init($td, $this->key, $this->iv);

        $encrypted = mcrypt_generic($td, $this->pkcs5_pad($data, $blockSize));
        $encrypted = base64_encode($encrypted);

        /* clean memory */
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);

        return $encrypted;
    }

    /**
     * 对标准请求协议数据执行解密
     * @param Request $request 必须包含 key、iv、data、sss 四个字段
     * @return bool|string
     */
    public function decodeRequest($sss, $mKey, $mIv, $mData){

        self::$sss = $sss ;

        $mKey = base64_decode($mKey) ;
        $mIv = base64_decode($mIv) ;
        $mData = base64_decode($mData) ;

        $privateKeyPath = storage_path('app/public/pks/pri_key.pem') ;


        $priKey = file_get_contents($privateKeyPath) ;

        $decrypted = '';
        openssl_private_decrypt($mKey, $decrypted, $priKey) ;
        $this->key = $decrypted;

        $decrypted = '';
        openssl_private_decrypt($mIv, $decrypted, $priKey) ;
        $this->iv = $decrypted;


        $td = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');


        /* 初始化解密模块 */
        mcrypt_generic_init($td, $this->key, $this->iv);

        /* 解密数据 */
        $data = mdecrypt_generic($td, $mData);

        $data = $this->pkcs5_unpad($data) ;

        /* 结束解密，执行清理工作，并且关闭模块 */
        mcrypt_generic_deinit($td);
        mcrypt_module_close($td);
        

        return json_decode($data, true);
    }

    /**
     * PKCS5PADDING 标准，数据补齐
     * @param $text
     * @param $blocksize
     * @return string
     */
    private function pkcs5_pad ($text, $blocksize)
    {
        $pad = $blocksize - (strlen($text) % $blocksize);
        return $text . str_repeat(chr($pad), $pad);
    }

    /**
     * PKCS5PADDING 标准，去掉数据补齐
     * @param $text
     * @return bool|string
     */
    private function pkcs5_unpad($text)
    {
        $pad = ord($text{strlen($text)-1});
        if ($pad > strlen($text)) return false;
        if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) return false;
        return substr($text, 0, -1 * $pad);
    }

}