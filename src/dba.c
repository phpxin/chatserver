/*
 * dba.c
 *
 *  Created on: 2015年1月3日
 *      Author: lixin
 */

#include "apue.h"
#include "config.h"
#include "logicutil.h"
#include <mysql.h>
#include "dba.h"
#include "dbautil.h"
#include "chat_exception.h"

MYSQL *mysql = NULL;

static int instance_exist();
static int getAvatarFull(char *avatar);

int close_db()
{
	if(mysql != NULL)
	{
		mysql_close(mysql);
	}

	return 1;
}

int init_db()
{
    mysql = mysql_init(NULL);	/* mysql为动态非配内存，需在程序结束时(及异常中断)释放 */

    char *db_host = chat_get_config("db.server");
	char *db_user = chat_get_config("db.user");
	char *db_pwd  = chat_get_config("db.pwd");
	char *db_name = chat_get_config("db.name");

	elog(E_MSG, "正在连接到数据库: %s:%s:%s:%s ...", db_host, db_user, db_pwd, db_name);

    mysql_real_connect(mysql, db_host, db_user, db_pwd, db_name, 0, NULL, 0);
    unsigned my_errno = mysql_errno(mysql);
    if (my_errno>0) {
        elog(E_ERROR, "mysql 连接错误，错误代码%d", my_errno);
        return 0;
    }
	elog(E_MSG, "mysql 连接成功");

    mysql_set_character_set(mysql, "utf8");
    sql_query(mysql, "set names utf8");

    return 1;
}

static int instance_exist()
{
	if(mysql == NULL)
	{
		elog(E_ERROR, "mysql: instance not exist");
		return 0;
	}
	return 1;
}
static int getAvatarFull(char *avatar){
	char *_ip = chat_get_config("server.ip");
	char *_webport = "8080";
	char _avatar[200];
	const char *format = "http://%s:%s/uploads/%s" ;

	/* size_t fbufsize = strlen(_ip) + strlen(_webport) + _strlen(format) + strlen(avatar) + 20; */
	memcpy(_avatar, avatar, strlen(avatar)+1);
	sprintf(avatar, format, _ip, _webport, _avatar);

	return 1;
}

int insert_message(struct message *msg)
{
	if(!instance_exist()){
		return DBE_NOINSTANCE;
	}

	size_t _t = sizeof(struct message)+200;
	char sql[_t];

	sprintf(sql, "insert into message(uid, fid, content, type) values(%d, %d, '%s', %d)",
			msg->uid,
			msg->fid,
			msg->content,
			msg->type
			);

	int flag = sql_query(mysql, sql);

	if(flag>0){
		return DBE_STATEMENT;
	}

	my_ulonglong count = mysql_affected_rows(mysql);

	return count;

}

int get_user(int id, struct user *_u)
{
	if(!instance_exist()){
		return DBE_NOINSTANCE;
	}

	size_t _t = 200;
	char sql[_t];

	sprintf(sql, "select * from user where id=%d", id);

	int flag = sql_query(mysql, sql);

	if(flag>0){
		return DBE_STATEMENT;
	}

	MYSQL_RES *result = sql_store_result(mysql);

	if(result == NULL){
		return DBE_RESULT;
	}

	my_ulonglong count = mysql_num_rows(result);

	if(count==0){
		return 0;
	}

	MYSQL_ROW row = mysql_fetch_row(result);

	_u->id = atoi(row[0]);
	strncpy(_u->account, row[1], sizeof(_u->account));
	strncpy(_u->pwd, row[2], sizeof(_u->pwd));
	strncpy(_u->name, row[3], sizeof(_u->name));
	strncpy(_u->avatar, row[4], sizeof(_u->avatar));
	getAvatarFull( _u->avatar );


	return 1;
}

int get_users(const char *where, struct user **users, size_t *ucount){
	if(!instance_exist()){
		return DBE_NOINSTANCE;
	}

	size_t _t = 200 + strlen(where);

	char sql[_t];

	sprintf(sql, "select * from user %s", where);

	int flag = sql_query(mysql, sql);

	if(flag>0){
		return DBE_STATEMENT;
	}

	MYSQL_RES *result = sql_store_result(mysql);

	if(result == NULL){
		return DBE_RESULT;
	}

    my_ulonglong item_count = mysql_num_rows(result);

    if(item_count<=0){
		*ucount = 0;
    	return 1;
    }
	
	size_t user_il = sizeof(struct user);
	*users = (struct user *)malloc(item_count*user_il);
	
	struct user _u;
    MYSQL_ROW row;
	int i=0;

	while((row = mysql_fetch_row(result))){

		memset(&_u, '\0', user_il);
	
		_u.id = atoi(row[0]);
		strncpy(_u.account, row[1], sizeof(_u.account)-1);
		strncpy(_u.pwd, row[2], sizeof(_u.pwd)-1);
		strncpy(_u.name, row[3], sizeof(_u.name)-1);
		strncpy(_u.avatar, row[4], sizeof(_u.avatar)-1);
		getAvatarFull( _u.avatar );	
		
		memcpy(*users+i, &_u, user_il);

		i++;
	}
	*ucount = i;

	return 1;
}
