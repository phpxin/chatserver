<?php
/**
 * 
 * @author   lixin <lixin65535@126.com>
 * @version  
 */
class UserAction extends Action 
{

	public function login(){

		$inputs = ApiTools::getRequestParams() ;

		$account = $inputs['account'] ;
		$pwd = $inputs['pwd'] ;


		$authcode = ApiLoginLogic::login($account, $pwd) ;

		if (!$authcode){
			ApiTools::error(ApiTools::CODE_ERR_USER, "用户名或密码错误") ;
		}

		ApiTools::success(['authcode'=>$authcode]) ;
	}

	/**
	 * 查询好友列表
	 */
	public function getlist()
	{
		$inputs = ApiTools::getRequestParams();

		$uid = intval( $inputs['uid'] );
		LogUtil::inst()->write(LogUtil::DEBUG, 'debug info', ['uid'=>$uid]) ;
		
		$list = M('user')->field('id,name,avatar')->where("id!=".$uid)->select();

	
		if( !empty($list) )
		{
			foreach($list as &$v){
				$v['avatar'] = getAvatarFullPath($v['avatar']) ;
			}
			$data = array( 'userlist' => $list );
			
			ApiTools::success($data) ;
			
		}else{
			
			ApiTools::error(ApiTools::CODE_ERR_NOT_FOUND, "暂无好友") ;
			
		}

	}

	/**
	 * 查询用户详细信息
	 *
	 */
	public function getInfo()
	{
		$inputs = ApiTools::getRequestParams();

		$uid = intval( $inputs['uid'] );

		$info = M('user')->where('id='.$uid)->field('id,name,avatar')->find();

		if(!empty($info)){
			$info['avatar'] = getAvatarFullPath($info['avatar']) ;
			$data = array('info' => $info );

			ApiTools::success($data) ;
		}else{
			ApiTools::error(ApiTools::CODE_ERR_NOT_FOUND, "找不到该用户") ;
		}

	}

	
}

?>
