#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"
#include "logicutil.h"
#include <errno.h>
#include "net.h"

#include "msg.h"
#include "dba.h"

#include <event.h>

#define MAX_CLIENT 512	/* 最大客户端连接数 */
#define MAX_EFDS 1024	/* epoll 最大支持的描述符数量 */

extern int errno ;

GHashTable *config = NULL;
GHashTable *clients = NULL;	/* 以连接标识符为key */

static void sig_func(int signum); /* 信号处理 */
static void free_all(); /* 释放堆内存 */


/*static void *thread_func(void *udata);  连接线程 */
/*pthread_t thread_c1;*/

struct event_base* client_ebase;

void onRead(int iCliFd, short iEvent, void *arg);
void onAccept(int serv, short iEvent, void *arg);

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
		elog(E_ERROR, "parse config faled ");
		exit( -1 );
	}

	/*
	flag = pthread_create(&thread_c1, NULL, thread_func, NULL);
	if(flag != 0)
	{
		elog(E_ERROR, "run child thread failed error num is %d", errno);
		exit( -1 );
	}
	*/

	char *_ip = chat_get_config("server.ip");
	char *_port = chat_get_config("server.port");
	if(_ip == NULL || _port == NULL){
		elog(E_ERROR, "key [server_ip/server_port] not found from config");
		exit( -1 );
	}

	unsigned short port = atoi(_port);

    struct sockaddr_in serv_addr;
	int serv_sock_f = socket(AF_INET, SOCK_STREAM, 0);


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton( AF_INET, _ip, &serv_addr.sin_addr.s_addr);

	/* SO_REUSEADDR 设置端口重用，否则中断程序重启会提示端口被使用，需要过一段时间才能使用 */
	flag = 1;
	int len=sizeof(int);
    if( setsockopt(serv_sock_f, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1)
    {

	   elog(E_ERROR, "setsockopt reuse addr failed error num is %d", errno);
       exit( -1 );
    }

	flag = bind(serv_sock_f, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(flag == -1)
	{
		elog(E_ERROR, "bind address failed error num is %d", errno);
		exit( -1 );
	}

	flag = listen(serv_sock_f, 5);
	if(flag == -1)
	{
		elog(E_ERROR, "listen failed error num is %d", errno);
		exit( -1 );
	}

	clients = g_hash_table_new(g_direct_hash, g_int_equal);


	flag = init_db();
	if(!flag)
	{
		elog(E_ERROR, "init database failed !");
		exit( -1 );
	}

	client_ebase = event_base_new();  /* 创建一个 event 的描述符 */

	/* 连接注册为新事件 (EV_PERSIST为事件触发后不默认删除) */
	struct event *pEvRead = (struct event *)malloc(sizeof(struct event));
	event_set(pEvRead, serv_sock_f, EV_READ|EV_PERSIST, onAccept, NULL);
	event_base_set(client_ebase, pEvRead);
	event_add(pEvRead, NULL);
	event_base_dispatch(client_ebase); /* 开始事件循环 */

	while(1)
	{

	}

	exit( EXIT_SUCCESS );
}

/*
static void *thread_func(void *udata)
{
	 扩展多线程注意：这里应该是每个线程管理不同的epoll，避免并发加锁造成拥塞，影响程序效率
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
*/

static void sig_func(int signum)
{
	switch(signum)
	{
	case SIGINT:
		free_all();
		kill((pid_t)getpid(), (int)SIGKILL);
		break;
	default:
		elog(E_WARNNING, "%d signal not found"); break;
	}
}

int remove_client(int key, int type)
{
	C_INFO *_cinfo = NULL;
	int flag = 0 , ret = 0;

	if(type == 1)
	{
		_cinfo = (C_INFO *)g_hash_table_find(clients, chat_cinfo_search_withuid, &key);
	}
	else
	{
		_cinfo = (C_INFO *)g_hash_table_find(clients, chat_cinfo_search_withfd, &key);
	}

	if(_cinfo == NULL)
	{
		elog(E_WARNNING, "client not found on main.c remove_client ");
		return 0;
	}


	flag = event_del(_cinfo->eventFp); /* 连接结束调用 */
	if(flag == -1)
	{
		elog(E_WARNNING, "event_del failed error num is %d", errno);
		perror("event_del");
		ret = 0;
	}
	else
	{
		close(_cinfo->fd);
		elog(E_MSG, "close fd ok !");
		ret = 1;
	}


	g_hash_table_remove(clients, &_cinfo->fd);

	myfree(_cinfo);

	return ret;
}

void onAccept(int serv_sock_f, short iEvent, void *arg)
{

	struct sockaddr_in client_addr;
	socklen_t client_sock_l = 0;
	int client_sock_f = -1;

	elog(E_MSG, "server is running .. wait connecter ...");
	ssize_t client_len = 0;
	client_len = g_hash_table_size(clients);
	if(client_len >= MAX_CLIENT)
	{
		elog(E_WARNNING, "client pool is full max client is %d , now %d", MAX_CLIENT, client_len);
		/* 应该通知客户端，服务器链接已满，需要排队 */

	}

	client_sock_f = accept(serv_sock_f, (struct sockaddr *)&client_addr, &client_sock_l);
	if(client_sock_f == -1)
	{
		elog(E_ERROR, "accept failed error num is %d", errno);

	}
	C_INFO *client_info = NULL;
	client_info = (C_INFO *)calloc(1, sizeof(C_INFO));

	client_info->uid = 0;
	client_info->fd = client_sock_f;
	inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_info->ipv4, 16);
	client_info->port = client_addr.sin_port;

	elog(E_MSG, "%s:%u connect", client_info->ipv4, client_info->port);


	/* 连接注册为新事件 (EV_PERSIST为事件触发后不默认删除) */
	struct event *pEvRead = (struct event *)malloc(sizeof(struct event));
	event_set(pEvRead, client_sock_f, EV_READ|EV_PERSIST, onRead, pEvRead);
	event_base_set(client_ebase, pEvRead);
	event_add(pEvRead, NULL);

	client_info->eventFp = pEvRead;

	/* 添加到客户端连接表 */
	g_hash_table_insert(clients, &(client_info->fd), (gpointer)client_info) ;
}



void onRead(int client, short iEvent, void *arg)
{

	msg(client);
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

		elog(E_MSG, "%d:%s:%u destory", *((int *)key), _info->ipv4, _info->port);

		close(_info->fd);

		myfree(value);

  	}

	/* myfree(clients); */


	close_db();

	elog(E_MSG, "free alloc done !");

	/* 程序结束，释放所有堆 END */
}
