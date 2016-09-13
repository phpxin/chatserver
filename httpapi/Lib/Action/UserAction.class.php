<?php
/**
 * 
 * @author   lixin <lixin65535@126.com>
 * @version  
 */
class UserAction extends Action 
{
	
	public function getlist()
	{
		$uid = intval( $GLOBALS['safePostParam']['uid'] );
		file_put_contents('/tmp/php.debug', var_export($uid, true));
		
		$list = M('user')->field('id,name,avatar')->where("id!=".$uid)->select();
		$data = array(
			'msg' => '暂无好友' ,
			'ecode' => 5001
		);
	
		if( !empty($list) )
		{
			foreach($list as &$v){
				$v['avatar'] = getAvatarFullPath($v['avatar']) ;
			}
			$data = array( 'userlist' => $list );
		}

		
		echo jsonReturn(1, $data);
	}

	public function getInfo()
	{
		$data = array(
			'msg' => '未找到用户' ,
			'ecode' => 5001
		);

		$uid = intval( $GLOBALS['safeGetParam']['uid'] );
		//file_put_contents('/tmp/php.debug', var_export($uid, true) );
		$info = M('user')->where('id='.$uid)->field('id,name,avatar')->find();

		if(!empty($info)){
			$info['avatar'] = getAvatarFullPath($info['avatar']) ;
			$data = array('info' => $info );
		}
		echo jsonReturn(1, $data);
	}
	
}

?>
