#include "apue.h"
#include <glib.h>
#include "hashutil.h"

gboolean chat_config_search_call(gpointer key, gpointer value, gpointer user_data)
{
	char *_k = (char *)key;
	char *_u = (char *)user_data;
	
	if(strcmp(_k, _u) == 0){
		return TRUE;
	}
	
	return FALSE;
}

gboolean chat_cinfo_search_withfd(gpointer key, gpointer value, gpointer user_data)
{
	C_INFO *_v = (C_INFO *)value;
	int *_u = (int *)user_data;

	if(_v->fd == *_u){
		return TRUE;
	}

	return FALSE;
}

gboolean chat_cinfo_search_withuid(gpointer key, gpointer value, gpointer user_data)
{
	C_INFO *_v = (C_INFO *)value;
	int *_u = (int *)user_data;

	if(_v->uid == *_u){
		return TRUE;
	}

	return FALSE;
}

gboolean chat_cinfo_search_call(gpointer key, gpointer value, gpointer user_data)
{
	int *_k = (int *)key;
	int *_u = (int *)user_data;

	if(*_k == *_u){
		return TRUE;
	}

	return FALSE;
}
