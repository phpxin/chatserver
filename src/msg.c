#include "apue.h"
#include "net.h"
#include "msg.h"
#include "logicutil.h"
#include <errno.h>
#include "action.h"
#include "protocol.h"

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

	protocol_stat_machine(protocol, buf+protocol_l, buf_len-protocol_l);

	char *_testmsg = "hello world !\n";
	msg_write(fd, (void *)_testmsg, strlen(_testmsg));

	myfree(buf);
	return 1;
}

void protocol_stat_machine(unsigned short protocol, const void *pkg, size_t pkg_len)
{
	STATUS status;
	switch(protocol)
	{
		case PTO_LOGIN:
			status = act_user_login(pkg, pkg_len);
			break;
		case PTO_MSG:
			status = act_user_message(pkg, pkg_len);
			break;
		default:
			break;
	}
}

int msg_read(int fd, void **pkg, size_t *pkg_len)
{
	
	int readlen = 0 , _rlen = 0;
	int _lenl = BUFSIZE;
	_lenl = 10;
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
			printf("package len is %d \n", package_len);
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
		elog("client %d was closed", fd);
		return 0;
	}

	if(readlen < 0)
	{
		remove_client(fd, 0);
		elog("recv failed error num is %d", errno);
		return 0;
	}

	*pkg_len = readlen - pkg_ll;
	memcpy(*pkg, *pkg+pkg_ll, *pkg_len);
	
	return 1;
}

int msg_write(int fd, void *pkg, size_t pkg_len)
{
	send(fd, pkg, pkg_len, 0);

	return 1;
}
