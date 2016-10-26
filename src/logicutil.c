#include "apue.h"
#include "stdarg.h"
#include "config.h"
#include "logicutil.h"


void elog(int level, const char *format , ...)
{
	FILE *fp = stdout ;
	
	if(!MODE_DEBUG){
		char *filepath = chat_get_config("server.log.path") ;
		FILE *fp = fopen(filepath, "a+");
		if(fp==NULL){
			printf("write log falied, %s file falied\n", filepath);
			return ;
		}
		
		va_list argp;
		fprintf(fp, "dump:");
		va_start(argp, format);
		vfprintf(fp, format, argp);
		va_end(argp);
		fprintf(fp, "\n");
		fclose(fp);
	}else{
		va_list argp;
		fprintf(fp, "dump:");
		va_start(argp, format);
		vfprintf(fp, format, argp);
		va_end(argp);
		fprintf(fp, "\n");
	}
	
	
}

int bl_or_ll(){
     union{
          short value ;
          char union_bytes[sizeof(short)] ;
     } test;

     test.value = 0x0102;

     if( (test.union_bytes[0]==1) && (test.union_bytes[1]==2) ){

    	 return 1; /* big */
     }

     else if( (test.union_bytes[1]==1) && (test.union_bytes[0]==2) ){

    	 return 2; /* little */
     }

     return 0;
}

int int_to_net(int value){
	if(bl_or_ll()==1){
		return value;
	}

	return int_bl_endian(value);
}

int net_to_int(int value){
	if(bl_or_ll()==1){
		return value;
	}

	return int_bl_endian(value);
}

int int_bl_endian(int x)
{
    int tmp;
    tmp = (((x)&0xff)<<24) + (((x>>8)&0xff)<<16) + (((x>>16)&0xff)<<8) + (((x>>24)&0xff));
    return tmp;
}

void myfree(void *var)
{
	if(var!=NULL)
	{
		free(var);
		var = NULL;
	}
}
