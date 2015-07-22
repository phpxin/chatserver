#include "apue.h"
#include "dba.h"
#include "action.h"

STATUS act_user_login(const void *pkg, size_t pkg_len)
{
	char account[200] = {'\0'} ;
	char pwd[200] = {'\0'} ;

	size_t shift = 0, cplen = 0;
	
	cplen = 200 ;
	memcpy(account, pkg+shift, cplen);

	shift += cplen ;
	cplen = 200 ;
	memcpy(pwd, pkg+shift, cplen);

	printf("account is %s, pwd is %s \n", account, pwd);

	return SUCC;
}

STATUS act_user_message(const void *pkg, size_t pkg_len)
{
	return SUCC;
}
