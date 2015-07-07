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