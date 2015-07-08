#include "apue.h"
#include <glib.h>
#include "config.h"
#include "strutil.h"
#include "logicutil.h"

extern GHashTable *config;

gboolean chat_parse_config(const char *path)
{
	
	FILE *fp = fopen(path, "r");
	
	if(fp == NULL){
		elog("open file failed !!!");
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
		elog("read config error !!!");
		return FALSE;
	}
	
	return TRUE;
}
