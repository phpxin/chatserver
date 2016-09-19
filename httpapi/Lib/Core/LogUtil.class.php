<?php
/**
 * Created by PhpStorm.
 * User: Administrator
 * Date: 2016/9/18
 * Time: 17:06
 */


class LogUtil{

    const DEBUG = 'debug' ;     //详细的debug信息
    const INFO = 'info' ;       //感兴趣的事件。像用户登录，SQL日志
    const NOTICE = 'notice' ;   //正常但有重大意义的事件。
    const WARNING = 'warning' ; //发生异常，使用了已经过时的API。
    const ERROR = 'error' ;     //运行时发生了错误，错误需要记录下来并监视，但错误不需要立即处理。
    const CRITICAL = 'critical' ; //关键错误，像应用中的组件不可用。
    const ALERT = 'alert' ;     //需要立即采取措施的错误，像整个网站挂掉了，数据库不可用。这个时候触发器会通过SMS通知你

    
    private static $inst = null ;

    private static $defaultSubPath = 'chat' ;


    private function __construct(){
        
    }

    private function __clone(){

    }

    /**
     * 调用该函数，获取实例化
     * @return LogUtil|null
     */
    public static function inst(){

        /*
        if (!self::$inst || !(self::$inst instanceof self) ){
            self::$inst = new self();
        }

        return self::$inst ;
        */

        return new self();

    }


    /**
     * 写日志
     * @param $level
     *      ALERT 级别会给管理员发邮件，当前发邮件使用同步方法，应尽量减少这个类型的触发
     * @param string $_message
     * @param array $context
     * @param string $subPath
     * @return bool
     */
    public function write($level, $_message, array $context = array(), $subPath=''){

        if (empty($subPath)){
            $subPath = self::$defaultSubPath ;
        }

        $savePath = $this->getLogsPath($subPath) ;

        if (!$savePath)
            return false;

        $message = json_encode($_message, JSON_UNESCAPED_UNICODE) ;


        $content = date('Y-m-d H:i:s')."\t".$level."\t".$message."\t".json_encode($context, JSON_UNESCAPED_UNICODE)."\n" ;
        file_put_contents($savePath, $content, FILE_APPEND) ;


        return true;
    }

    /**
     * 给管理员发送短信，需要.env配置管理员邮箱
     * @param string $message 消息正文
     * @param array $context 消息上下文内容
     * @return bool
     */
    private function sendErrSms($message, array $context=array()){


        return false;
    }

    /**
     * Get the path to the logs file
     * @param string $subPath 子地址
     * @return string
     */
    private function getLogsPath($subPath)
    {
        $path = storage_path() . '/logs/'.$subPath.'/'.date('Y/m');

        if (!file_exists($path)) {
            if (!mkdir($path, 0777, true)){
                //Log::error('create log path failed ', ['path' => $path]);

                //不能写日志，重要错误
                $this->sendErrSms('create log path failed ', ['path' => $path]) ;

                return false;
            }
        }

        $path = $path.date('/d').'.log';

        return $path;
    }

}