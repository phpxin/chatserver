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
#include "dbautil.h"

int sql_query(MYSQL *_mysql, const char *stmt_str)
{
	int _errno = mysql_query(_mysql, stmt_str);
	if(_errno>0)
	{
		elog(E_ERROR, "mysql: statement syntax error: errno %d", _errno);
	}
	return _errno;
}

MYSQL_RES *sql_store_result(MYSQL *_mysql)
{
	MYSQL_RES *result = mysql_store_result(_mysql);

	if(result == NULL){
		unsigned int _errno = mysql_errno(_mysql);
		elog(E_ERROR, "mysql: store result error: errno %d", _errno);
	}

	return result;
}
