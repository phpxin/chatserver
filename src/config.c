#include "apue.h"
#include "config.h"

BOOL chat_parse_config(const char *path)
{
	
	FILE *fp = fopen(path, "r");
	
	if(fp == NULL){
		printf("open file failed !!!\n");
		return FALSE;
	}
	
	char confItem[BUFSIZE];
	
	while(fgets(confItem, BUFSIZE, fp) != NULL){
		printf("%s \n", confItem);
	}
	
	if(ferror(fp)){
		printf("read config error !!!\n");
		return FALSE;
	}
	
	return TRUE;
}