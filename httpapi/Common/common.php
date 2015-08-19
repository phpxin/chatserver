<?php 
/**
 * 公用程序
 * @author   lixin <lixin65535@126.com>
 * @date 2014-05-28 
 */

include 'config.php';
include 'func.inc.php';
include 'logFunc.inc.php';

//系统日志
if(LOG_ERROR){
	set_error_handler('logErrorToFile',E_ALL-E_NOTICE); //设置警告级处理函数
}
if(LOG_EXCEPTION){
	set_exception_handler('logExceptionToFile');
}

?>