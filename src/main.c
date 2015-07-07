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
	
	gpointer val = g_hash_table_find(config, chat_config_search_call, "db.name");
	if(val != NULL){
		printf("%s \n", (char *)val) ;
	}else{
		printf("key not found \n");
	}
	
	/* 程序结束，释放所有堆 */

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, config);
	while (g_hash_table_iter_next (&iter, &key, &value))
  	{
		printf("%s = %s \n", (char *)key, (char *)value);
		free(key);
		free(value);
  	}

	free(config);


	void *test_a = malloc(BUFSIZE);
	free(test_a);
	
	return 0;
}
