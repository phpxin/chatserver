#include "apue.h"
#include <glib.h>
#include "config.h"
#include "strutil.h"
#include "logicutil.h"
#include "hashutil.h"

extern GHashTable *config;

char* chat_get_config(char *name)
{
	char *value = (char *)g_hash_table_find(config, chat_config_search_call, name);
	return value;
}

gboolean chat_parse_config(const char *path)
{
	
	FILE *fp = fopen(path, "r");
	
	if(fp == NULL){
		elog("open config file %s failed !!!", path);
		return FALSE;
	}
	config = g_hash_table_new (g_str_hash, g_str_equal);
	
	char conf_item[BUFSIZE];
	
	gchar **_val;
	
	while(fgets(conf_item, BUFSIZE, fp) != NULL){
		
		_val = g_strsplit(conf_item, "=", 2);

		su_trim(_val[0], "\r\n");
		su_trim(_val[1], "\r\n");
		
		g_hash_table_insert(config, _val[0], _val[1]) ;
		
	}
	
	_val = NULL;
	
	if(ferror(fp)){
		elog("read config %s error !!!", path);
		return FALSE;
	}
	
	return TRUE;
}
