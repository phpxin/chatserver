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

MYSQL *mysql = NULL;

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

	elog("正在连接到数据库: %s:%s:%s:%s ...", db_host, db_user, db_pwd, db_name);

    mysql_real_connect(mysql, db_host, db_user, db_pwd, db_name, 0, NULL, 0);
    unsigned my_errno = mysql_errno(mysql);
    if (my_errno>0) {
        elog("mysql 连接错误，错误代码%d", my_errno);
        return 0;
    }
	elog("mysql 连接成功");

    mysql_set_character_set(mysql, "utf8");
    mysql_query(mysql, "set names utf8");

    return 1;
}

int insert_message(struct message *msg)
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

int get_user_by_id(int id, struct user *_u)
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

	_u->id = atoi(row[0]);
	strncpy(_u->account, row[1], sizeof(_u->account));
	strncpy(_u->pwd, row[2], sizeof(_u->pwd));
	strncpy(_u->name, row[3], sizeof(_u->name));

	return 1;
}

int get_user_for_login(const char *account, const char *pwd, struct user *_u){
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

	MYSQL_RES *result =  mysql_store_result(mysql);

    if (result == NULL) {
        return -3;
    }

    my_ulonglong item_count = mysql_num_rows(result);

    if(item_count<=0){
    	return -1;
    }

    MYSQL_ROW row;

	row = mysql_fetch_row(result);

	_u->id = atoi(row[0]);
	strncpy(_u->account, row[1], sizeof(_u->account));
	strncpy(_u->pwd, row[2], sizeof(_u->pwd));
	strncpy(_u->name, row[3], sizeof(_u->name));

	if(strcmp(_u->pwd,pwd)!=0){
		return -2;
	}

	return 0;

}

int add_user(struct user *u)
{

	if(mysql == NULL){
		return 0;
	}

	size_t _t = sizeof(struct user)+200;

	char sql[_t];

	sprintf(sql, "insert into user(account,pwd,name) values('%s','%s','%s')", u->account, u->pwd, u->name) ;

	printf("sql is %s \n", sql);

	int flag = mysql_query(mysql, sql);

	if(flag>0){
		return 0;
	}

	my_ulonglong _c = mysql_affected_rows(mysql);

	if(_c<=0){
		return 0;
	}

	return 1;
}

