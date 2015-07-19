/*
 * dba.h
 *
 *  Created on: 2015年1月3日
 *      Author: lixin
 */

#ifndef DBA_H_
#define DBA_H_

#include </usr/local/mysql/include/mysql.h>


typedef enum {
    false = 0,
    true = 1
} bool;

struct user{
	int id;
	char account[200];
	char pwd[200];
	char name[200];
};

struct message{
	int id;
	int uid;
	int fid;
	char content[65535];
	short type;
};

bool init_db();
bool addUser(struct user *u);
int getUserForLogin(const char *account, const char *pwd, struct user *_u);
int getUserById(int id, struct user *_u);
int insertMessage(struct message *msg);

MYSQL *mysql;

#endif /* DBA_H_ */
