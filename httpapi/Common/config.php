<?php
/**
 * 配置文件
 * @info 系统配置
 * @date 2014-05-28
 */

error_reporting(E_ALL-E_NOTICE) ;
date_default_timezone_set('Asia/ChongQing') ;
header('content-type:text/html; charset=utf-8');

//其他配置
define('DS', DIRECTORY_SEPARATOR);//系统分隔符
define('URL_MODE',1); //url模式 0 代表普通 1代表pathinfo

//缓存配置（如果不使用缓存不需要配置）
define('CACHE_HOST','');
define('CACHE_PORT',0);

define('STORAGE', dirname(__FILE__).'/../storage');

//数据库配置
define('DB_HOST','chatdb') ;
define('DB_USER','dev') ;
define('DB_PWD','JINEG928391^#^!@()') ;
define('DB_NAME','cchat') ;
define('DB_CHARSET','utf8') ;
define('DB_PREFIX','') ;
define('DB_CACHE_FOLDER',STORAGE.'/db_cache') ;

//错误日志
define('LOG_ERROR',FALSE);
define('LOG_ERROR_PATH',STORAGE.'/log/errors');

//异常日志
define('LOG_EXCEPTION', false);
define('LOG_EXCEPTION_PATH',STORAGE.'/log/exceptions');

//环境变量 (最后不要加分隔符)
define('__ROOT__','');

?>
