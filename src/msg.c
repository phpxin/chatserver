#include "apue.h"
#include "net.h"
#include "msg.h"
#include "logicutil.h"
#include <errno.h>

extern int errno ;

int msg(int fd)
{
	void *buf = NULL;
	size_t buf_len = 0;
	msg_read(fd, &buf, &buf_len);

	if(buf_len<=0)
	{
		return 0;
	}

	printf("recv msg %ld : %s \n", buf_len, (char *)buf);

	char *_testmsg = "hello world !\n";
	msg_write(fd, (void *)_testmsg, strlen(_testmsg));

	myfree(buf);
	return 1;
}

int msg_read(int fd, void **pkg, size_t *pkg_len)
{
	int _len = 1024;
	*pkg = malloc(_len);
	memset(*pkg, '\0', _len);

	int recv_len = recv(fd, *pkg, _len, 0);

	if(recv_len == 0)
	{
		remove_client(fd, 0);
		elog("client %d was closed", fd);
		return 0;
	}

	if(recv_len < 0)
	{
		remove_client(fd, 0);
		elog("recv failed error num is %d", errno);
		return 0;
	}

	*pkg_len = recv_len;
	return 1;
}

int msg_write(int fd, void *pkg, size_t pkg_len)
{
	send(fd, pkg, pkg_len, 0);

	return 1;
}
