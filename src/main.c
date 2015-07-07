#include "apue.h"
#include <glib.h>
#include "config.h"
#include "hashutil.h"



int main(int argc, char *argv[]){
	
	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		return -1;
	}
	
	
	if( chat_parse_config(argv[1]) )
	{
		printf("parse config ok \n");
	}
	else
	{
		printf("parse config faled \n");
	}
	
	GHashTable *config = g_hash_table_new (g_str_hash, g_str_equal);
	g_hash_table_insert(config, "db.server", "127.0.0.1") ;
	g_hash_table_insert(config, "db.name", "chat") ;
	
	gpointer val = g_hash_table_find(config, chat_config_search_call, "db.name");
	if(val != NULL){
		printf("%s \n", (char *)val) ;
	}
	
	
	
	/* 程序结束，释放所有堆 */
	free(config);
	
	return 0;
}