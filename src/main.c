#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"
#include "logicutil.h"
#include <errno.h>
#include "net.h"
#include <sys/epoll.h>
#include "msg.h"
#include "dba.h"

#define MAX_CLIENT 512	/* 最大客户端连接数 */
#define MAX_EFDS 1024	/* epoll 最大支持的描述符数量 */

extern int errno ;

GHashTable *config = NULL;
GHashTable *clients = NULL;

static void sig_func(int signum); /* 信号处理 */
static void free_all(); /* 释放堆内存 */
static void *thread_func(void *udata); /* 连接线程 */

int ep_servf = -1 , ep_clients_t1= -1 ;
struct epoll_event *event_ok = NULL;
struct epoll_event *ce_ok = NULL;

int uid = 1; /* ______  模拟的uid */

pthread_t thread_c1;

int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		exit( -1 );
	}

	int flag = 0;

	(void)signal(SIGINT, sig_func) ;
	
	chat_parse_config(argv[1]);

	if( config == NULL )
	{
		elog("parse config faled ");
		exit( -1 );
	}

	flag = pthread_create(&thread_c1, NULL, thread_func, NULL);
	if(flag != 0)
	{
		elog("run child thread failed error num is %d", errno);
		exit( -1 );
	}
	
	char *_ip = chat_get_config("server.ip");
	char *_port = chat_get_config("server.port");
	if(_ip == NULL || _port == NULL){
		elog("key [server_ip/server_port] not found from config");
		exit( -1 );
	}
	
	unsigned short port = atoi(_port);
	
    struct sockaddr_in serv_addr;
	int serv_sock_f = socket(AF_INET, SOCK_STREAM, 0);
	/* ioctl(serv_sock_f, FIONBIO, 1); */

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton( AF_INET, _ip, &serv_addr.sin_addr.s_addr);
	
	flag = bind(serv_sock_f, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(flag == -1)
	{
		elog("bind address failed error num is %d", errno);
		exit( -1 );
	}

	flag = listen(serv_sock_f, 5);
	if(flag == -1)
	{
		elog("listen failed error num is %d", errno);
		exit( -1 );
	}

	
    struct sockaddr_in client_addr;
	socklen_t client_sock_l = 0;
	int client_sock_f = -1;

	elog("server is running .. wait connecter ...");

	ep_servf = epoll_create(5);
	ep_clients_t1 = epoll_create(5);

	struct epoll_event _event;
	_event.data.fd = serv_sock_f;
    _event.events = EPOLLIN ;

	flag = epoll_ctl(ep_servf, EPOLL_CTL_ADD, serv_sock_f, &_event);
    if (flag == -1) {
		elog("epoll_ctl failed error num is %d", errno);
        exit( -1 );
    }

	event_ok = (struct epoll_event *)calloc( 1, sizeof(struct epoll_event));
	C_INFO *client_info = NULL;
	clients = g_hash_table_new(g_direct_hash, g_int_equal);
	size_t client_len = 0;

	flag = init_db();
	if(!flag)
	{
		elog("init database failed !");
		exit( -1 );
	}

	while(1)
	{
        int isok=epoll_wait(ep_servf, event_ok, 1, -1) ;
        if(isok>0 && event_ok[0].data.fd == serv_sock_f)
        {
			client_len = g_hash_table_size(clients);
			if(client_len >= MAX_CLIENT)
			{
				elog("client pool is full max client is %d , now %d", MAX_CLIENT, client_len);
				/* 应该通知客户端，服务器链接已满，需要排队 */
				continue;
			}

			client_sock_f = accept(serv_sock_f, (struct sockaddr *)&client_addr, &client_sock_l);
			if(client_sock_f == -1)
			{
				elog("accept failed error num is %d", errno);
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

			/* 添加到客户端epoll */
			_event.data.fd = client_sock_f;
    		_event.events = EPOLLIN | EPOLLOUT ;
			epoll_ctl(ep_clients_t1, EPOLL_CTL_ADD, client_sock_f, &_event);
		}
	}
	
	exit( EXIT_SUCCESS );
}

static void *thread_func(void *udata)
{

	ce_ok = (struct epoll_event *)calloc( MAX_EFDS, sizeof(struct epoll_event));
	int i = 0;
	while(1)
	{
		int isok = epoll_wait(ep_clients_t1, ce_ok, 1, -1) ;
		for( i=0; i<isok; i++)
		{
			if(ce_ok[i].events & EPOLLIN)
			{
				msg(ce_ok[i].data.fd);
			}
		}
	}


	pthread_exit( NULL );
}


static void sig_func(int signum)
{
	switch(signum)
	{
	case SIGINT:
		free_all();
		kill((pid_t)getpid(), (int)SIGKILL);
		break;
	default:
		elog("%d signal not found"); break;
	}
}

int remove_client(int key, int type)
{
	C_INFO *_cinfo = NULL;
	int flag = 0 , ret = 0;

	if(type == 1)
	{
		_cinfo = (C_INFO *)g_hash_table_find(clients, chat_cinfo_search_call, &key);
	}
	else
	{
		_cinfo = (C_INFO *)g_hash_table_find(clients, chat_cinfo_search_withfd, &key);
	}

	if(_cinfo == NULL)
	{
		elog("client not found on main.c remove_client ");
		return 0;
	}


	flag = epoll_ctl(ep_clients_t1, EPOLL_CTL_DEL, _cinfo->fd, NULL);

	if(flag == -1)
	{
		elog("epoll_ctl failed error num is %d", errno);
		perror("epoll_ctl");
		ret = 0;
	}
	else
	{
		close(_cinfo->fd);
		elog("close fd ok !");
		ret = 1;
	}

	g_hash_table_remove(clients, &_cinfo->uid);

	myfree(_cinfo);

	return ret;
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

	/* myfree(config); */

	
	g_hash_table_iter_init (&iter, clients);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		
		C_INFO *_info = (C_INFO *)value;
		
		elog("%d:%s:%u destory", *((int *)key), _info->ipv4, _info->port);
		
		close(_info->fd);
		
		myfree(value);
		
  	}

	/* myfree(clients); */
	
	if(ep_servf != -1)
	{
		close(ep_servf);
	}

	if(ep_clients_t1 != -1)
	{
		close(ep_clients_t1);
	}

	myfree(event_ok);
	myfree(ce_ok);
	
	close_db();

	elog("free alloc done !");
	
	/* 程序结束，释放所有堆 END */	
}
