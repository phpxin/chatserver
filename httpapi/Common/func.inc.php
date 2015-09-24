<?php
/**
 * 公用函数库
 * @author   lixin <lixin65535@126.com>
 * @version  
 */

function dump($obj){
	echo '<pre>';
	var_dump($obj);
	echo '</pre>';
}

//从org文件夹载入类
function importORGClass($str){
	include 'Lib/ORG/'.ltrim($str,'/') ;
}

function getAvatarFullPath($filename)
{
	return 'http://'.$_SERVER["HTTP_HOST"].'/uploads/'.$filename;
}

/**
 * @param int $ret 1/2 成功/失败
 * @param array $data 消息数组，失败时data.msg 为错误信息/ecode为错误代码
 */
function jsonReturn($ret, $data){
	$arr = array(
		'ret'=>intval($ret), 
		'data'=>$data
	);

	return json_encode($arr);
}
	
/**
 * 自动加载类
 * @param string $s 类名
 * @return string 
 * @info 此函数为php自动调用
 */
function classLoader($className){

	if(strtolower(substr($className,-5))=='model'){
		include 'Lib/Model/'.$className.'.class.php';
	}else if(strtolower(substr($className,-6))=='action'){
		include 'Lib/Action/'.$className.'.class.php';
	}else if(strtolower(substr($className,-9))=='exception'){
		include 'Lib/Exception/'.$className.'.class.php';
	}else{
		//如果项目文件中没有，在核心包搜索
		if(file_exists('Lib/Core/'.$className.'.class.php')){
			include 'Lib/Core/'.$className.'.class.php';
		}else{
			//交给其他自动加载类函数 如 smarty
			//throw new NotFoundException("类 $className 未找到" );
		}
	}
}

/**
 * 字符串处理
 * @param string $s 字符串
 * @return string 
 */
function safeStr($s){
	return trim($s) ;
}

/**
 * 获取数据库连接实例
 * @param string $table 表名
 */
function M($table){
	static $dbMaster ;	//定义一个在函数执行后不会释放的静态变量， 存放数据库类实例化的容器
	$table=ucfirst(strtolower($table)).'Model';
	if(empty($dbMaster[$table])){
		$dbMaster[$table] = new $table();
		return $dbMaster[$table];
	}
	return $dbMaster[$table];
}

/**
 * 获取标准路径
 * Enter description here ...
 * @param string $module 模块名称
 * @param string $action 控制器名称
 * @param array $params get参数 以键值对的方式保存
 */
function U($module='index',$action='Index',$_params=array()){
	$module=strtolower($module);
	$action=ucfirst(strtolower($action));
	if(!empty($_params)){
		foreach($_params as $key=>$val){
			$paramStr.=$key.'/'.$val.'/';
		}
	}

	return '/index.php/'.$action.'/'.$module.'/'.trim($paramStr,'/');
}

/**
 * 处理http post 数据
 * Enter description here ...
 */
function createPostParams(){
	foreach($_POST as $key=>$value){
		$safeParam[$key] = safeStr($value) ;
	}
	
	return $safeParam;	
}

/**
 * 处理http get 数据
 * @param string $table 表名
 */
function createGetParams(){
	if(URL_MODE===1){
		if(!empty($_SERVER['PATH_INFO'])){
			
			$pathInfo = trim(trim($_SERVER['PATH_INFO']),'/');
			$paramArray = array_reverse(explode('/',$pathInfo));
			//获取控制器参数
			$safeParam['module'] = safeStr(array_pop($paramArray)) ;
			$safeParam['action'] = safeStr(array_pop($paramArray)) ;
			//生成GET数组其他参数
			while(!empty($paramArray)){
				$key = safeStr(array_pop($paramArray));
				$value = safeStr(array_pop($paramArray));
				$safeParam[$key] = $value ;
			}
		}
	}
	
	foreach($_GET as $key=>$value){
		$safeParam[$key] = safeStr($value) ;
	}
	
	return $safeParam;
}


?>
