<?php
/**
 * pdo 连接 mysql 数据库
 * Enter description here ...
 * @author lixin
 * @date 2014-05-28
 */
class pdo_mysql extends PDO {
	
	private $dsn;
	private $host;
	private $user;
	private $pwd;
	private $db;
	private $charset;
	private $prefix;
	private $cacheFolder;
	
	private $tableName; //表明
	private $sql; //查询语句
	private $fields; //字段
	
	private $_field='';
	private $_where='';
	private $_order='';
	private $_limit='';
	
	
	public function __construct($tableName=''){
		$this->host=DB_HOST;
		$this->user=DB_USER;
		$this->pwd=DB_PWD;
		$this->db=DB_NAME;
		$this->prefix=DB_PREFIX;
		$this->tableName=$this->prefix.strtolower((empty($tableName)?substr(get_class($this), 0, -5):$tableName));
        $this->cacheFolder=rtrim(DB_CACHE_FOLDER,'/').'/';

        if( !file_exists($this->cacheFolder) && !(mkdir($this->cacheFolder, 0777, true)) ) {
            throw new Exception('无法创建数据库字段缓存文件夹，或没有权限');
        }
		
		//尝试连接数据库
		$this->dsn = 'mysql:host='.$this->host.';dbname='.$this->db;

		//echo $this->dsn, $this->user, $this->pwd;

		try{
			parent::__construct($this->dsn, $this->user, $this->pwd);

		}catch(PDOException $e){
			
			throw new SQLException('pdo 数据库连接失败');
			
		}
		
		$this->getFields();
	
	}
	
	//设置字段缓存
	private function writeFields(){
		$sql="desc ".$this->tableName;
		$result=$this->query($sql);

		foreach($result as $val){
			$field[]=$val['Field'];

			if($val['Key']=='PRI'){
				$field['_pk']=$val['Field'];
			}
			if($val['Extra']=='auto_increment'){
				$field['_auto']=$val['Field'];
			}

		}
		$string="<?php \n return ".var_export($field,true)."\n ?>";

		if(!file_put_contents($this->cacheFolder.$this->tableName.'.php',$string)){
			throw new SQLException('设置数据库字段失败,写入文件失败，请检查文件权限');
			return ;
		}

		return $field;
	}
	
	//获取字段缓存
	private function getFields(){
		$file=$this->cacheFolder.$this->tableName.'.php';
		if(file_exists($file)){
			$this->fields=include($file);
		}else{
			$this->fields=$this->writeFields();
		}
	}
	
	//执行查询 select
	public function query($sql){
		$this->sql=$sql;
		$statement=parent::query($sql);
		$result=$statement->fetchAll(PDO::FETCH_ASSOC);
		if(!empty($result))	return $result;
		else return false;
	}
	
	//执行更新 insert delete update
	public function execute($sql){
		$this->sql=$sql;
		$statement=parent::query($sql);
		if(!empty($statement) && $statement->rowCount()>0)
			return $statement->rowCount();
		else
			return false;
	}
	
	//增加
	public function add($data){
		$realFieldArray=array_intersect($this->fields, array_keys($data));//所有有效字段
		$field=implode(',', $realFieldArray);
		
		foreach($realFieldArray as $v){
			$values[]="'$data[$v]'";
		}

		$values=implode(',', $values);
		
		$sql="insert into $this->tableName($field) values($values)";
		
		return $this->execute($sql);
		
		
	}
	
	//limit条件
	public  function limit($limit){
		$this->_limit='limit '.$limit;
		return $this;
	}
	
	//排序条件
	public function order($order){
		if(is_array($order)){
			$order=implode(',', $order);
		}
		$this->_order='order by '.$order;
		return $this;
	}
	
	//设置查寻条件
	public function where($where){
		if(is_array($where)){
			$where=implode(" and ", $where);
		}
		$this->_where='where '.$where;
		return $this;
	}
	
	//设置字段
	public function field($field){
		if(!is_array($field)){
			$field=explode(',', $field);
		}
		$_field=array_intersect($this->fields, array_values($field));//所有有效字段
		$_field=array_unique($_field);
		$this->_field=implode(',', $_field);
		return $this;
	}
	
	//查寻多个
	function select(){
		if(empty($this->_field)){
			$f=array_unique(array_values($this->fields));
			$this->_field=implode(',', $f);
		}
		
		$sql="select $this->_field from $this->tableName $this->_where $this->_order $this->_limit";
		return $this->query($sql);
	}
	
	//查寻一个
	function find(){
		if(empty($this->_field)){
			$f=array_unique(array_values($this->fields));
			$this->_field=implode(',', $f);
		}
		
		$sql="select $this->_field from $this->tableName $this->_where $this->_order $this->_limit";
		$_d=$this->query($sql);
		if(is_array($_d)){
			return $_d[0];
		}
		return $_d;
	}
	
	//删除指定内容
	function del(){
		if(empty($this->_where)){
			trigger_error("企图在没有条件的情况下删除 $this->tableName 表下的内容",E_USER_WARNING);
			return false;
		}
		$sql="delete from $this->tableName $this->_where";
		return $this->execute($sql);
	}
	
	//更新内容
	function update($data){
		if(empty($this->_where)){
			trigger_error("企图在没有条件的情况下更新 $this->tableName 表下的内容",E_USER_WARNING);
			return false;
		}
		
		$f=array_intersect($this->fields, array_keys($data)); //合法数据
		foreach($f as $v){
			$sets[]="$v='$data[$v]'";
		}
		$sets=implode(',', $sets);
		
		$sql="update $this->tableName set $sets $this->_where $this->_order $this->_limit";
		return $this->execute($sql);
		
	}
	
	//最大值
	function max(){
		foreach(explode(',', $this->_field) as $v){
			$ms[]="MAX($v) as $v";
		}
		$ms=implode(',', $ms);
		
		$sql="select $ms from $this->tableName $this->_where";
		
		return $this->query($sql);
		
	}
	
	
	function count($where=''){
		foreach(explode(',', $this->_field) as $v){
			$ms[]="COUNT($v) as $v";
		}
		$ms=implode(',', $ms);
		
		$sql="select $ms from $this->tableName $this->_where";
		
		return $this->query($sql);
	}
	function min($field='',$where=''){
		foreach(explode(',', $this->_field) as $v){
			$ms[]="MIN($v) as $v";
		}
		$ms=implode(',', $ms);
		
		$sql="select $ms from $this->tableName $this->_where";
		
		return $this->query($sql);
	}
	function avg($field='',$where=''){
		foreach(explode(',', $this->_field) as $v){
			$ms[]="AVG($v) as $v";
		}
		$ms=implode(',', $ms);
		
		$sql="select $ms from $this->tableName $this->_where";
		
		return $this->query($sql);
	}
	function sum($field='',$where=''){
		foreach(explode(',', $this->_field) as $v){
			$ms[]="SUM($v) as $v";
		}
		$ms=implode(',', $ms);
		
		$sql="select $ms from $this->tableName $this->_where";
		
		return $this->query($sql);
	}
	function getLastInsertId(){
		return parent::lastInsertId();
	}
	function getLastSql(){
		return $this->sql;
	}
	
	public function __destruct(){
		
	}
}
