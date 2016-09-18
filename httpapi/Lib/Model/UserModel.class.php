<?php
/**
 * 数据库基类，提供公用项目数据库层代码
 * Enter description here ...
 * @author lixin
 *
 */
class UserModel extends BaseModel 
{



    public function checkLogin($account, $pwd){

        $info = $this->where("account='{$account}'")->find();


        if (!$info){
            return false;
        }


        if ($info['pwd']!=$pwd){
            return false;
        }


        return $info ;
    }
	
}
