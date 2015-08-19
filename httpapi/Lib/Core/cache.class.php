<?php
/**
 * 缓存类
 * Enter description here ...
 * @author lixin
 * @date 2014-05-28
 */
class cache{
	
	private static $_instance; //实例
	private $db;
	
	public function __clone(){
		trigger_error("禁止复制缓存对象",E_USER_ERROR);
	}
	
	private function __construct(){
		$this->db=new Memcache();
		$this->db->addServer(CACHE_HOST,CACHE_PORT);
		
	}
	
	/**
	 * 获取实例化对象
	 * Enter description here ...
	 * @return object $_instance
	 */
	public static function getInstance(){
		if(empty(CACHE_HOST)){
			return false;
		}
		
		if(!(self::$_instance instanceof self)){
			self::$_instance=new self();
		}
		
		return self::$_instance;
	}
	
	/**
	 * 删除一个条目
	 * Enter description here ...
	 * @param string $key 条目名
	 * @param bool 是否成功
	 */
	public function del($key){
		return $this->db->delete($key);
	}
	
	/**
	 * 获取条目
	 * Enter description here ...
	 * @param string $key 条目名
	 * @return mixed 对应值，失败返回false
	 */
	public function get($key){
		
		$d=$this->db->get($key);
		
		if(empty($d))	return null;
		return unserialize($d);
	}
	
	/**
	 * 添加条目
	 * Enter description here ...
	 * @param string $key 条目名
	 * @param mixed $value 对应值
	 * @param long $time 有效时间（秒）
	 * @return mixed 成功返回插入的数据，失败返回null
	 */
	public function set($key,$value,$time){
		$d=serialize($value);
		
		if($this->db->add($key,$d,false,$time)){
			return $value;
		}
		
		return null;
	}
	
}
