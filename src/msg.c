#include "apue.h"
#include "net.h"
#include "msg.h"
#include "logicutil.h"
#include <errno.h>
#include "action.h"
#include "protocol.h"
#include "dba.h"

extern int errno ;

int msg(int fd)
{
	void *buf = NULL; /* data package */
	size_t buf_len = 0;
	msg_read(fd, &buf, &buf_len);

	if(buf_len<=0)
	{
		return 0;
	}

	unsigned short protocol;
	size_t protocol_l = sizeof(unsigned short);
	memcpy(&protocol, buf, protocol_l);
	protocol = ntohs(protocol);

	RET sm_ret = protocol_stat_machine(protocol, buf+protocol_l, buf_len-protocol_l);

	int flag = 0;

	if(sm_ret.status == SUCC){
		flag = 1;

		/* if login protocol , update user link hash table */
		if(protocol == PTO_LOGIN)
		{
			int _login_uid = net_to_int( ( (struct user *)(sm_ret.udata) )->id );
			update_user_link(fd, _login_uid);
		}
	}
    
	response(fd, flag, protocol + PTO_MASK, sm_ret.udata, sm_ret.udata_l);

	myfree(buf);
	myfree(sm_ret.udata);

	return 1;
}

RET protocol_stat_machine(unsigned short protocol, const void *pkg, size_t pkg_len)
{
	RET ret = {SUCC, "", 0, NULL} ;
	switch(protocol)
	{
		case PTO_LOGIN:
			ret = act_user_login(pkg, pkg_len);
			break;
		case PTO_MSG:
			ret = act_user_message(pkg, pkg_len);
			break;
		default:
			ret.status = FAILED ;
			ret.tip = "unknow protocol" ;
			break;
	}

	return ret;
}

void response(int fd, int flag, unsigned short protocol, const void *udata, size_t udata_len)
{
	size_t flag_l = sizeof(int) ;

	int pkg_size = flag_l + udata_len ;

	void *pkg = malloc(pkg_size);
	memset(pkg, '\0', pkg_size);
	
	int _flag = int_to_net(flag);
	memcpy(pkg, &_flag, flag_l);

	memcpy(pkg+flag_l, udata, udata_len);

	msg_write(fd, protocol, &pkg, pkg_size);

	myfree(pkg);
}

int msg_read(int fd, void **pkg, size_t *pkg_len)
{
	
	int readlen = 0 , _rlen = 0;
	int _lenl = BUFSIZE;
	*pkg = malloc(_lenl);
	memset(*pkg, '\0', _lenl);

	int runflag = 1 , package_len = 0;
	int pkg_ll = sizeof(int); /* size of package_len */

	while( runflag ){

		_rlen = recv(fd, *pkg+readlen, _lenl, 0);
		readlen += _rlen;

		if(!package_len){
			memcpy(&package_len, *pkg, pkg_ll);
			package_len = net_to_int(package_len);
			/*printf("package len is %d \n", package_len);*/
			elog(E_MSG, "package len is %d \n", package_len);
		}

		if(_rlen >= _lenl && readlen < package_len){
			*pkg = realloc(*pkg, readlen+_lenl);
			memset(*pkg+readlen, '\0', _lenl);
			runflag = 1;
		}else{
			runflag = 0;
		}
	}

	if(readlen == 0)
	{
		remove_client(fd, 0);
		elog(E_MSG, "client %d was closed", fd);
		return 0;
	}

	if(readlen < 0)
	{
		remove_client(fd, 0);
		elog(E_MSG, "recv failed error num is %d", errno);
		return 0;
	}

    /* 去掉包长度字段 */
	*pkg_len = readlen - pkg_ll;

    void *tempmem = calloc(*pkg_len, sizeof(void));
    memcpy(tempmem, *pkg+pkg_ll, *pkg_len);

    memset(*pkg, '\0', readlen);
	memcpy(*pkg, tempmem, *pkg_len);
	
    myfree(tempmem);

	return 1;
}

int msg_write(int fd, unsigned short protocol, void **pkg, size_t pkg_len)
{
	size_t pkg_ll = sizeof(int) ;
	size_t pto_l = sizeof(unsigned short) ;

	void *tempmem = calloc(pkg_len, sizeof(void));
	memcpy(tempmem, *pkg, pkg_len);

	size_t pkg_size = pkg_ll+pto_l+pkg_len;

	*pkg = realloc(*pkg, pkg_size);
	memset(*pkg, '\0', pkg_len);

	

	int _pkg_size = int_to_net(pkg_size);
	memcpy(*pkg, &_pkg_size, pkg_ll);

	unsigned short _protocol = htons(protocol) ;
	memcpy(*pkg+pkg_ll, &_protocol, pto_l);
	
	memcpy(*pkg+pkg_ll+pto_l, tempmem, pkg_len);
	myfree(tempmem);

	send(fd, *pkg, pkg_size, 0);

	return 1;
}
