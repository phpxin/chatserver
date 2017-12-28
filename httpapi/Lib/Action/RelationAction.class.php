<?php
/**
 * 关系类,包括邀请,处理邀请等操作,删好友
 * Created by PhpStorm.
 * User: Administrator
 * Date: 2016/9/18
 * Time: 18:33
 */


class RelationAction extends Action{

    public function inviteList(){

        //$input = ApiTools::getRequestParams() ;
        $where = [
            'to_uid='.$this->uid ,
            'status='.InviteModel::STATUS_DOING ,
        ] ;
        $inviteList = M('invite')->where($where)->select() ;

        if (empty($inviteList)) {
            ApiTools::error(ApiTools::CODE_DATA_EMPTY, '暂无申请') ;
        }

        $_uids = array_column($inviteList, 'uid') ;
        $_users = M('user')->where("id in('".implode("','", $_uids)."')")->select() ;

        $users = [] ;
        foreach ($_users as $_u) {
            $users[$_u['id']] = $_u ;
        }

        $data = [] ;
        foreach ($inviteList as $item) {

            if (!isset($users[$item['uid']])) {
                continue ;
            }

            $_user = $users[$item['uid']] ;

            $data[] = [
                'id' => $item['id'] ,
                'uid' => $item['uid'] ,
                'intro' => $item['intro'] ,
                'name' => $_user['name'] ,
                'avatar' => getAvatarFullPath($_user['avatar']) ,
                'userIntro' => $_user['intro'] ,
                'gender' => $_user['gender']
            ] ;
        }

        ApiTools::success(['list'=>$data]) ;
    }

    public function dealInvite() {

        $input = ApiTools::getRequestParams();
        $id = $input['id'] ;
        $agree = $input['agree'] ; // 1,0 同意/拒绝

        $inviteInfo = M('invite')->where("id={$id}")->find();

        if (empty($inviteInfo)){
            ApiTools::error(ApiTools::CODE_MSG, '没有这条申请') ;
        }
        
        if ($inviteInfo['status'] != InviteModel::STATUS_DOING) {
            ApiTools::error(ApiTools::CODE_MSG, '这条申请已经处理过了') ;
        }

        if ($inviteInfo['to_uid']!=$this->uid) {
            ApiTools::error(ApiTools::CODE_MSG, 'ERR') ; //可能是攻击
        }
        
        $now = time();
        
        $relationModel = M('relation') ;

        $relationModel->execute('begin transaction') ;
        
        try{
            
            if ($agree){
                $save = [
                    'uid' => $inviteInfo['uid'] ,
                    'fid' => $inviteInfo['to_uid'] ,
                    'addtime' => $now
                ] ;
                $flag = $relationModel->add($save) ;
                if ($flag === false) {
                    throw new Exception('添加失败') ;
                }

                $save = [
                    'uid' => $inviteInfo['to_uid'] ,
                    'fid' => $inviteInfo['uid'] ,
                    'addtime' => $now
                ] ;
                $flag = $relationModel->add($save) ;
                if ($flag === false) {
                    throw new Exception('添加失败') ;
                }
                
                $status = InviteModel::STATUS_AGREE ;
            }else{
                $status = InviteModel::STATUS_REFUSE ;
            }
            
            $flag = M('invite')->where("id={$id}")->update(['status'=>$status]) ;
            if ($flag === false){
                throw new Exception('添加失败') ;
            }
            
            $relationModel->exec('commit') ;
            
        }catch (Exception $e) {

            $relationModel->execute("rollback") ;
            ApiTools::error(ApiTools::CODE_ERR_USER, $e->getMessage()) ;
            
        }
        
        ApiTools::success(['msg'=>'ok']) ;
    }

    /**
     * 邀请
     */
    public function invite(){
        
        $inputs = ApiTools::getRequestParams();

        $save['uid'] = $this->uid ;
        $save['to_uid'] = isset($inputs['fid']) ? $inputs['fid'] : '' ;
        $save['intro'] = isset($inputs['intro']) ? $inputs['intro'] : '' ;
        
        if (empty($save['to_uid'])) {
            ApiTools::error(ApiTools::CODE_MSG, '请选择目标用户') ;
        }
        
        if ($save['uid'] == $save['to_uid']) {
            ApiTools::error(ApiTools::CODE_MSG, '不能加自己为好友') ;
        }

        $isset = M('relation')->where("uid={$save['uid']} and fid={$save['to_uid']}")->find();

        if ($isset) {
            ApiTools::error(ApiTools::CODE_MSG, '好友关系已经建立') ;
        }
        
        M('invite')->add($save) ;


        ApiTools::success(['msg'=>'ok']) ;
    }



}