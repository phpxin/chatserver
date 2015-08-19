<?php
/**
 * 日志处理函数库
 * @author lixin
 * @date 2014-5-22
 * @info 注册到系统错误处理机制中的必须函数
 */

/**
 * 获取错误级别string
 * Enter description here ...
 */
function getErrorName($eCode){
	
	$arr[1]= 'E_ERROR';
	$arr[2]= 'E_WARNING';
	$arr[4]= 'E_PARSE';
	$arr[8]= 'E_NOTICE';
	$arr[16]= 'E_CORE_ERROR';
	$arr[32]= 'E_CORE_WARNING';
	$arr[64]= 'E_COMPILE_ERROR';
	$arr[128]= 'E_COMPILE_WARNING';
	$arr[256]= 'E_USER_ERROR';
	$arr[512]= 'E_USER_WARNING';
	$arr[1024]= 'E_USER_NOTICE';
	$arr[2048]= 'E_STRICT';
	$arr[4096]= 'E_RECOVERABLE_ERROR';
	$arr[8191]= 'E_ALL';
	
	return $eCode.' '.$arr[$eCode];
}


/**
 * 将错误记录到日志
 * Enter description here ...
 */
function logErrorToFile($errno, $errstr, $errfile, $errline){
	$errStr=date('Y-m-d H:i:s')." | 文件 $errfile | 第 $errline 行 | 错误级别 ".getErrorName($errno)." | 错误信息 $errstr \n";
	
	$path=defined(LOG_ERROR_PATH)?LOG_ERROR_PATH:'./log';
	if(!is_dir($path)){
		mkdir($path, 0777, true);
	}
	$fileName=rtrim($path,'/').'/'.date('Ym').'.err';
	
	touch($fileName); //更新文件访问时间，如果不存在则创建
	
	$fp=fopen($fileName,'a');
	fwrite($fp,$errStr); //写入文件
	fclose($fp);
	
	//应该判断错误级别，跳转到错误页面
	header("location:".U('error'));
}

/**
 * 将异常记录到日志
 * Enter description here ...
 */
function logExceptionToFile($eObj){
	
	$logMessage=date('Y-m-d H:i:s').' : '.get_class($eObj).' : '.$eObj->getCode().' : '.$eObj->getMessage()." \n";
	
	$path=defined(LOG_ERROR_PATH)?LOG_ERROR_PATH:'./log';
	if(!is_dir($path)){
		mkdir($path, 0777, true);
	}
	$fileName=rtrim($path,'/').'/'.date('Ym').'.except';
	
	touch($fileName); //更新文件访问时间，如果不存在则创建
	
	$fp=fopen($fileName,'a');
	fwrite($fp,$logMessage); //写入文件
	fclose($fp);
	
	//使用异常句柄，程序将会停止执行，跳转到错误页面，如果需要继续执行，请使用try catch结构
	header("location:".U('error'));
	
}
