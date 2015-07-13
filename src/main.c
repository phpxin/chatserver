#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"
#include "logicutil.h"
#include <errno.h>
#include "net.h"
#include <sys/epoll.h>

#define MAX_EFDS 1024	/* epoll 最大支持的描述符数量 */

extern int errno ;

GHashTable *config = NULL;
GHashTable *clients = NULL;

static void sig_func(int signum); /* 信号处理 */
static void free_all(); /* 释放堆内存 */

int ep_servf = -1;
struct epoll_event *event_ok = NULL;

int uid = 1; /* ______  模拟的uid */

int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		return -1;
	}

	(void)signal(SIGINT, sig_func) ;
	
	if( chat_parse_config(argv[1]) || config == NULL )
	{
		elog("parse config ok ");
	}
	else
	{
		elog("parse config faled ");
		return -1;
	}
	
	char *_ip = (char *)g_hash_table_find(config, chat_config_search_call, "server.ip");
	char *_port = (char *)g_hash_table_find(config, chat_config_search_call, "server.port");
	if(_ip == NULL || _port == NULL){
		elog("key not found ");
		return -1;
	}
	
	unsigned short port = atoi(_port);
	
    struct sockaddr_in serv_addr;
	int serv_sock_f = socket(AF_INET, SOCK_STREAM, 0);
	ioctl(serv_sock_f, FIONBIO, 1);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton( AF_INET, _ip, &serv_addr.sin_addr.s_addr);
	
	int flag = 0;

	flag = bind(serv_sock_f, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(flag == -1)
	{
		elog("bind address failed error num is %d", errno);
		perror("bind");
		return -1;
	}

	flag = listen(serv_sock_f, 5);
	if(flag == -1)
	{
		elog("listen failed error num is %d", errno);
		perror("bind");
		return -1;
	}

	
    struct sockaddr_in client_addr;
	socklen_t client_sock_l = 0;
	int client_sock_f = -1;

	elog("server is running .. wait connecter ...");

	ep_servf = epoll_create(5);

	struct epoll_event _event;
	_event.data.fd = serv_sock_f;
    _event.events = EPOLLIN ;

	flag = epoll_ctl(ep_servf, EPOLL_CTL_ADD, serv_sock_f, &_event);
    if (flag == -1) {
		elog("epoll_ctl failed error num is %d", errno);
        perror("epoll_ctl");
        return -1;
    }

	event_ok = (struct epoll_event *)calloc( 1, sizeof(struct epoll_event));
	C_INFO *client_info = NULL;
	clients = g_hash_table_new(g_direct_hash, g_int_equal);

	while(1)
	{
        int isok=epoll_wait(ep_servf, event_ok, 1, -1) ;
        if(isok>0 && event_ok[0].data.fd == serv_sock_f)
        {

			client_sock_f = accept(serv_sock_f, (struct sockaddr *)&client_addr, &client_sock_l);
			if(client_sock_f == -1)
			{
				elog("client socket broken");
				continue;
			}

			client_info = (C_INFO *)calloc(1, sizeof(C_INFO));

			client_info->uid = uid;
			uid++;
			client_info->fd = client_sock_f;
			inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_info->ipv4, 16);
			client_info->port = client_addr.sin_port;

			elog("%s:%u connect", client_info->ipv4, client_info->port);

			/* 添加到客户端连接表 */
			g_hash_table_insert(clients, &(client_info->uid), (gpointer)client_info) ;
		}
	}
	
	return 0;
}

/*
static void *thread_func(void *udata)
{
}

*/

static void sig_func(int signum)
{
	switch(signum)
	{
	case SIGINT:
		free_all();
		kill(getpid(), SIGKILL);
		break;
	default:
		elog("%d signal not found"); break;
	}
}

static void free_all()
{
	/* 程序结束，释放所有堆 */

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, config);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		myfree(key);
		myfree(value);
  	}

	myfree(config);

	g_hash_table_iter_init (&iter, clients);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		
		C_INFO *_info = (C_INFO *)value;
		
		elog("%d:%s:%u destory", *((int *)key), _info->ipv4, _info->port);
		
		
		close(_info->fd);
		
		myfree(value);
		
  	}
	
	myfree(clients);
	/* clients = NULL; */
	
	if(ep_servf != -1)
	{
		close(ep_servf);
	}

	myfree(event_ok);
	
	/* 程序结束，释放所有堆 END */	
}
