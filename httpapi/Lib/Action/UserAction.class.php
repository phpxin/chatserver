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
		
		$list = M('user')->select();

		if( empty($list) )
		{
			$list = array();
		}
	
		echo json_encode($list);
		
	}

}

?>
