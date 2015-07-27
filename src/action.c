#include "apue.h"
#include "dba.h"
#include "action.h"
#include "protocol.h"
#include "strutil.h"
#include "logicutil.h"

RET act_user_login(const void *pkg, size_t pkg_len)
{
	char account[200] = {'\0'} ;
	char pwd[200] = {'\0'} ;

	RET ret={SUCC, ""};

	size_t shift = 0, cplen = 0;
	
	cplen = 200 ;
	memcpy(account, pkg+shift, cplen);

	shift += cplen ;
	cplen = 200 ;
	memcpy(pwd, pkg+shift, cplen);

	printf("account is %s, pwd is %s \n", account, pwd);

	su_trim(account, "\r\n ");
	su_trim(pwd, "\r\n ");

	char *format = "where account='%s'\0";
	size_t wlen = strlen(format) + strlen(account) + 2;

	char where[wlen];
	sprintf(where, format, account) ;

	struct user *users = NULL;
	size_t ucount = 0;
	
	int flag = get_users(where, &users, &ucount);

	if(flag<0){
		elog("user login failed return %d", flag);
		ret.status = ERR;
		ret.tip = "user account not exist";
	}else{
		int i;
		struct user _u;
		size_t user_il = sizeof(struct user);
		for( i=0; i<ucount; i++){
			memcpy(&_u, users+i, user_il);
			printf("account is %s, pwd is %s, name is %s \n", _u.account, _u.pwd, _u.name);
		}
	}

	myfree(users);

	return ret;
}

STATUS act_user_message(const void *pkg, size_t pkg_len)
{
	return SUCC;
}
