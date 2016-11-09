<?php
/**
 * 主控制器
 * 所有控制器都应继承这个控制器
 * @author   lixin <lixin65535@126.com>
 * @date 2014-05-28
 */
class Action{

	protected $template ;
    protected $uid = 0 ;
    protected $userInfo = null ;
	
	public function __construct(){


        $skipLogin = [
            'user:login' ,
            'user:register' ,
        ] ; // 可以跳过用户验证的程序

        $route = strtolower($GLOBALS['safeGetParam']['module'].':'.$GLOBALS['safeGetParam']['action']) ;

        if (!in_array($route, $skipLogin)) {

            $inputs = ApiTools::getRequestParams() ;


            if (!isset($inputs['authcode'])){
                ApiTools::error(ApiTools::CODE_ERR_LOGIN, "授权信息有误") ;
            }

            //用户验证等
            $inputs = ApiTools::getRequestParams();
            $authcode = $inputs['authcode'] ;

            $loginInfo = ApiLoginLogic::authCode($authcode) ;

            if (!$loginInfo) {
                ApiTools::error(ApiTools::CODE_ERR_LOGIN, "授权信息有误") ;
            }

            $now = time();
            $this->uid = $loginInfo['uid'] ;

            if ($now > $loginInfo['lose_time']) {
                ApiTools::error(ApiTools::CODE_ERR_INVALID, "授权已过期") ;
            }

            $this->userInfo = M('user')->where("id={$this->uid}")->find();

            if (empty($this->userInfo)) {
                ApiTools::error(ApiTools::CODE_ERR_LOGIN, "授权信息有误") ;
            }

        }

        
	}

}
 
 
?>
