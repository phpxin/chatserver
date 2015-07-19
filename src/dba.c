/*
 * dba.c
 *
 *  Created on: 2015年1月3日
 *      Author: lixin
 */

#include "main.h"
#include "dba.h"
#include "clientmanger.h"

extern GHashTable *ght_conf;

/*
 连接数据库
 @return bool 是否成功
 */
bool init_db()
{
    mysql = mysql_init(NULL);

    gpointer _info = NULL;

    _info = g_hash_table_find(ght_conf,my_hash_tab_find_conf,"db_host");
    char *db_host = (char *)_info;
    _info = g_hash_table_find(ght_conf,my_hash_tab_find_conf,"db_user");
	char *db_user = (char *)_info;
	_info = g_hash_table_find(ght_conf,my_hash_tab_find_conf,"db_pwd");
	char *db_pwd = (char *)_info;
	_info = g_hash_table_find(ght_conf,my_hash_tab_find_conf,"db_name");
	char *db_name = (char *)_info;

	printf("正在连接到数据库: %s:%s:%s:%s ...\n", db_host, db_user, db_pwd, db_name);

    //mysql_real_connect(mysql, HOST, USER, PWD, DB_NAME, 0, NULL, 0); //初始化连接
    mysql_real_connect(mysql, db_host, db_user, db_pwd, db_name, 0, NULL, 0); //初始化连接
    unsigned my_errno = mysql_errno(mysql); //获取错误码
    if (my_errno>0) {
        printf("mysql 连接错误，错误代码%d\n\n", my_errno);
        return false;
    }

    mysql_set_character_set(mysql, "utf8"); //设置字符集
    mysql_query(mysql, "set names utf8");

    printf("mysql 连接成功\n\n");

    return true;
}

/**
 * 插入聊天数据
 * @param struct message *msg 消息正文结构体
 * @return -3 数据库错误；0 插入0条；返回插入条数
 */
int insertMessage(struct message *msg)
{
	if(mysql == NULL){
		return -3;
	}

	size_t _t = sizeof(struct message)+200;
	char sql[_t];

	sprintf(sql, "insert into message(uid, fid, content, type) values(%d, %d, '%s', %d)",
			msg->uid,
			msg->fid,
			msg->content,
			msg->type
			);

	int flag = mysql_query(mysql, sql);

	if(flag>0){
		return -3;
	}

	my_ulonglong count = mysql_affected_rows(mysql);

	return count;

}

/**
 * 通过id获取用户信息
 *
 * @param int id 用户id
 * @param struct user *_u 返回值指针
 *
 * @return -3 数据库错误 -2 未找到 0 成功
 *
 */
int getUserById(int id, struct user *_u)
{
	if(mysql == NULL){
		return -3;
	}

	size_t _t = 200;
	char sql[_t];

	sprintf(sql, "select * from user where id=%d", id);

	int flag = mysql_query(mysql, sql);

	if(flag>0){
		return -3;
	}

	MYSQL_RES *result = mysql_store_result(mysql);

	if(result == NULL){
		return -3;
	}

	my_ulonglong count = mysql_num_rows(result);

	if(count==0){
		return 0;
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	_u->id = atoi(row[0]); //mysql返回数据同为 "const char *" 型
	strncpy(_u->account, row[1], sizeof(_u->account));
	strncpy(_u->pwd, row[2], sizeof(_u->pwd));
	strncpy(_u->name, row[3], sizeof(_u->name));

	return 1;
}

/**
 * 登录用户获取信息
 * @param const char *account 用户账号
 * @param const char *pwd 用户密码
 * @param struct user *_u 返回值指针
 *
 * @return struct user 用户结构
 * -1 登录账号错误
 * -2 密码错误
 * -3 数据库错误
 * 0 成功（将返回指针设为查到的用户）
 * 获取登录用户信息
 */
int getUserForLogin(const char *account, const char *pwd, struct user *_u){
	if(mysql == NULL){
		return -3;
	}

	size_t _t = 200;

	char sql[_t];

	sprintf(sql, "select * from user where account='%s'", account);

	int flag = mysql_query(mysql, sql);

	if(flag>0){
		return -3;
	}

	MYSQL_RES *result =  mysql_store_result(mysql); //获取结果集，每次一行获取

    if (result == NULL) {
        return -3;
    }

    my_ulonglong item_count = mysql_num_rows(result); //获取查寻行数

    if(item_count<=0){
    	return -1;
    }

    MYSQL_ROW row; //数据行

	row = mysql_fetch_row(result);

	_u->id = atoi(row[0]); //mysql返回数据同为 "const char *" 型
	strncpy(_u->account, row[1], sizeof(_u->account));
	strncpy(_u->pwd, row[2], sizeof(_u->pwd));
	strncpy(_u->name, row[3], sizeof(_u->name));

	if(strcmp(_u->pwd,pwd)!=0){
		return -2;
	}

	return 0;

}

bool addUser(struct user *u)
{

	if(mysql == NULL){
		return false;
	}

	size_t _t = sizeof(struct user)+200;

	char sql[_t];

	sprintf(sql, "insert into user(account,pwd,name) values('%s','%s','%s')", u->account, u->pwd, u->name) ;

	printf("sql is %s \n", sql);

	int flag = mysql_query(mysql, sql);

	if(flag>0){
		return false;
	}

	my_ulonglong _c = mysql_affected_rows(mysql);

	if(_c<=0){
		return false;
	}

	return true;
}

