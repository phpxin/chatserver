<?php
/**
 * 主入口文件
 * @author   lixin <lixin65535@126.com>
 * @version  
 */
include 'Common/common.php';

//增加include文件夹
//set_include_path(get_include_path().';./Lib');

//创建get参数
$GLOBALS['safeGetParam'] = createGetParams(); //将安全get参数装载到全局数组
$GLOBALS['safePostParam'] = createPostParams(); //将安全get参数装载到全局数组

//注册自动加载类函数，__autoload与 smarty3 有冲突
spl_autoload_register('classLoader');

//加载控制器
$module = safeStr($GLOBALS['safeGetParam']['module']);
$action = safeStr($GLOBALS['safeGetParam']['action']);

$module = empty($module)?'IndexAction':ucfirst(strtolower($module)).'Action';
$action = empty($action)?'index':strtolower($action);

$__m = new $module();

$__m->$action();

?>