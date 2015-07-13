#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ioctl.h>

#ifndef NULL
	#define NULL (void *)0
#endif

#define BUFSIZE 1024

typedef enum{FAILED=0, SUCC=1, ERR=-1}STATUS ;

typedef struct {
	int uid;
	int fd;
	char ipv4[32];
	unsigned short port;
} C_INFO; /* 客户端连接信息 */
