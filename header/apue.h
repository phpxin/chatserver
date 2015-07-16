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

/**
 * 三个值表示的状态应该不同
 * FAILED 表示失败
 * SUCC	表示成功
 * ERR	表示有逻辑错误，应该给客户端返回信息
 */ 
typedef enum{FAILED=0, SUCC=1, ERR=-1}STATUS ;

typedef struct {
	int uid;
	int fd;
	char ipv4[32];
	unsigned short port;
} C_INFO; /* 客户端连接信息 */
