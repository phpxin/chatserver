#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"
#include "logicutil.h"
#include <errno.h>
#include "net.h"

extern int errno ; /* 用于设置错误 */

GHashTable *config = NULL;

int main(int argc, char *argv[]){

	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		return -1;
	}
	
	
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

	while(1)
	{
		client_sock_f = accept(serv_sock_f, (struct sockaddr *)&client_addr, &client_sock_l);
		if(client_sock_f == -1)
		{
			elog("client socket broken");
			continue;
		}

		char _client_ip[BUFSIZE];
			
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, _client_ip, client_sock_l);

		elog("%s:%u connect", _client_ip, client_addr.sin_port);

		

		close(client_sock_f);
	}

	
	/* 程序结束，释放所有堆 */

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, config);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		free(key);
		free(value);
  	}

	free(config);


	void *test_a = malloc(BUFSIZE);
	free(test_a);
	
	/* 程序结束，释放所有堆 END */
	
	return 0;
}
