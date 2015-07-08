#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"

GHashTable *config = NULL;

int main(int argc, char *argv[]){
	
	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		return -1;
	}
	
	
	if( chat_parse_config(argv[1]) || config == NULL )
	{
		printf("parse config ok \n");
	}
	else
	{
		printf("parse config faled \n");
		return -1;
	}
	
	char *_ip = (char *)g_hash_table_find(config, chat_config_search_call, "server.ip");
	char *_port = (char *)g_hash_table_find(config, chat_config_search_call, "server.port");
	if(_ip == NULL || _port == NULL){
		printf("key not found \n");
		return -1;
	}
	
	unsigned short port = atoi(_port);
	
	printf("%s:%us \n", _ip, port);
	
	/* 程序结束，释放所有堆 */

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, config);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		/* printf("%s = %s \n", (char *)key, (char *)value); */
		free(key);
		free(value);
  	}

	free(config);


	void *test_a = malloc(BUFSIZE);
	free(test_a);
	
	/* 程序结束，释放所有堆 END */
	
	return 0;
}
