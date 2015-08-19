<?php
/**
 * 
 * @author   lixin <lixin65535@126.com>
 * @version  
 */
class IndexAction extends Action 
{
	
	public function index()
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
