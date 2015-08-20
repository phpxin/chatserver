http 接口

1 配置 Common/config.php, 配置 chatdb域名

2 DB_CACHE_FOLDER,LOG_ERROR_PATH,LOG_EXCEPTION_PATH : /var/log/cchat  需要读写权限
	mkdir -p /var/log/cchat/db_cache
	chmod -R 777 /var/log/cchat
