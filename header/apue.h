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

#define MODE_DEBUG 0

/**
 * 三个值表示的状态应该不同
 * FAILED 表示失败
 * SUCC	表示成功
 * ERR	表示有逻辑错误，应该给客户端返回信息
 */ 
typedef enum{FAILED=0, SUCC=1, ERR=-1}STATUS ;
typedef struct{
	STATUS status;
	char *tip;
	size_t udata_l;
	void *udata;
}RET;

typedef struct {
	int uid;
	int fd;
	char ipv4[32];
	unsigned short port;
	struct event *eventFp; /* event 描述符，用来在断开连接时删除事件 */
} C_INFO; /* 客户端连接信息 */

/**
 * 关闭客户端连接，并清除
 * @param int key 用户数据
 * @param int flag 查找标识：0/1 ：文件描述符/UID
 */
int remove_client(int key, int type);
