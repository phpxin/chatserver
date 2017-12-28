<?php
/**
 * 
 * @author   lixin <lixin65535@126.com>
 * @version  
 */
class UserAction extends Action 
{

	/**
	 * 注册接口
	 */
	public function register()
	{
		$inputs = ApiTools::getRequestParams() ;
		
		$account = $inputs['account'] ;
		$pwd = $inputs['pwd'] ;
		$repwd = $inputs['repwd'] ;


		if ($pwd != $repwd) {
			ApiTools::error(ApiTools::CODE_ERR_USER, "两次密码输入不一致") ;
		}

		$table = M('user') ;
		$table->execute("begin transaction") ;

		try{
			$info = $table->query("select * from user where account='{$account}' for update") ;
			if ($info) {
				throw new Exception('用户已经注册') ;
			}

			$save['account'] = $account ;
			$save['pwd'] = $pwd ;
			$save['name'] = $account ;
			$save['avatar'] = 'default.jpg' ;

			$table->add($save) ;

			$table->execute("commit") ;

		}catch(Exception $e){
			$table->execute("rollback") ;
			ApiTools::error(ApiTools::CODE_ERR_USER, $e->getMessage()) ;
		}

		ApiTools::success(['msg'=>'ok']) ;
	}

	/**
	 * 登录
	 */
	public function login(){
		

		$inputs = ApiTools::getRequestParams() ;
		LogUtil::inst()->write(LogUtil::DEBUG, 'debug info', ['inputs'=>$inputs]) ;


		$account = $inputs['account'] ;
		$pwd = $inputs['pwd'] ;


		$authcode = ApiLoginLogic::login($account, $pwd) ;

		if (!$authcode){
			ApiTools::error(ApiTools::CODE_ERR_USER, "用户名或密码错误") ;
		}

		ApiTools::success(['authcode'=>$authcode]) ;
	}

    /**
     * 搜索用户
     */
    public function search(){
        $uid = $this->uid ;

        //var_dump($this->userInfo) ;

        $inputs = ApiTools::getRequestParams() ;
        $keywords = $inputs['keywords'] ;

        $list = M('user')->field('id,name,avatar')->where("name like '%$keywords%'")->select();



        if( !empty($list) )
        {
            foreach($list as &$v){
                $v['avatar'] = getAvatarFullPath($v['avatar']) ;
            }
            $data = array( 'userlist' => $list );

            ApiTools::success($data) ;

        }else{

            ApiTools::error(ApiTools::CODE_ERR_NOT_FOUND, "找不到相关用户") ;

        }
    }

	/**
	 * 查询好友列表
	 */
	public function getlist()
	{

		$uid = $this->uid;
		LogUtil::inst()->write(LogUtil::DEBUG, 'debug info', ['uid'=>$uid]) ;

		$relation = M('relation')->where('uid='.$uid)->select() ;
		

        if (empty($relation)) {
            ApiTools::error(ApiTools::CODE_ERR_NOT_FOUND, "暂无好友") ;
        }

        $rids = array_column($relation, 'fid') ;
		
		$list = M('user')->field('id,name,avatar')->where("id in ('".implode("','", $rids)."')")->select();

	
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

		//
		//
        $uid = intval( $inputs['uid'] );
        //$uid = $this->uid ;
        
        if (!$uid) {
            ApiTools::error(ApiTools::CODE_ERR_NOT_FOUND, "找不到该用户") ;
        }

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
