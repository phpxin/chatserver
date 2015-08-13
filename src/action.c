#include "apue.h"
#include "dba.h"
#include "action.h"
#include "protocol.h"
#include "strutil.h"
#include "logicutil.h"
#include "hashutil.h"
#include "msg.h"

extern GHashTable *clients;

void update_user_link(int fd, int uid)
{
	C_INFO *_cinfo = (C_INFO *)g_hash_table_find(clients, chat_config_search_call, &fd);
	_cinfo->uid = uid;
}

RET act_user_login(const void *pkg, size_t pkg_len)
{
	char account[200] = {'\0'} ;
	char pwd[200] = {'\0'} ;

	RET ret={SUCC, "", 0, NULL};

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

	struct user _u;

	if(flag<0){
		elog(E_NOTICE, "user login failed return %d", flag);
		ret.status = ERR;
		ret.tip = "user account not exist";
	}else{
		int i;
		
		size_t user_il = sizeof(struct user);
		for( i=0; i<ucount; i++){
			memcpy(&_u, users+i, user_il);
		}

		if(strncmp(_u.pwd, pwd, strlen(_u.pwd)) != 0)
		{
			ret.status = ERR;
			ret.tip = "password wrong";
		}
	}

	myfree(users);

	/* response */
	if(ret.status == SUCC){
		ret.udata_l = sizeof(struct user);
		ret.udata = malloc(ret.udata_l) ;
		_u.id = int_to_net(_u.id);
		memcpy(ret.udata, &_u, ret.udata_l);
	}

	return ret;
}

RET act_user_message(const void *pkg, size_t pkg_len)
{
	RET ret = {SUCC, "", 0, NULL};

	int uid = 0;
	int fid = 0;
	int len = 0;

	size_t _k = sizeof(int) ;

	memcpy(&uid, pkg, _k);
	memcpy(&fid, pkg+_k, _k);
	memcpy(&len, pkg+_k*2, _k);

	uid = net_to_int(uid);
	fid = net_to_int(fid);
	len = net_to_int(len);

	if(len > TEXT_SIZE_1-1){
		ret.status = FAILED;
		ret.tip = "message content to long, big then max";
        return ret;
	}

	struct message msg ;
	msg.uid = uid;
	msg.fid = fid;
	
	memset(msg.content, '\0', TEXT_SIZE_1);
	memcpy(msg.content, pkg+_k*3, len);
	
	insert_message(&msg);

	/* retransmission */

	C_INFO *_cinfo = NULL;
	_cinfo = (C_INFO *)g_hash_table_find(clients, chat_cinfo_search_withuid, &msg.fid);

	if(_cinfo == NULL){
		elog(E_MSG, "对方未登录 %d", msg.fid);
	}else{
		void *wpkg = malloc(pkg_len);
		memcpy(wpkg, pkg, pkg_len);
		
		msg_write(_cinfo->fd, PTO_MSG, &wpkg, pkg_len);
		myfree(wpkg);
	}

	return ret;
}
