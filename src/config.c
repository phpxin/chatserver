#include "apue.h"
#include <glib.h>
#include "config.h"

extern GHashTable *config;

gboolean chat_parse_config(const char *path)
{
	
	FILE *fp = fopen(path, "r");
	
	if(fp == NULL){
		printf("open file failed !!!\n");
		return FALSE;
	}
	config = g_hash_table_new (g_str_hash, g_str_equal);
	
	char conf_item[BUFSIZE];
	
	gchar **_val;
	
	while(fgets(conf_item, BUFSIZE, fp) != NULL){
		
		_val = g_strsplit(conf_item, "=", 2);
		
		/* printf("%s is %s \n", _val[0], _val[1]) ; */
		g_hash_table_insert(config, _val[0], _val[1]) ;
		
		/* g_strfreev (_val); */
	}
	
	_val = NULL;
	
	if(ferror(fp)){
		printf("read config error !!!\n");
		return FALSE;
	}
	
	return TRUE;
}
