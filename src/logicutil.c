#include "apue.h"
#include "stdarg.h"
#include "logicutil.h"

void elog(const char *format , ...)
{
	va_list argp;
	fprintf(stdout, "dump:");
	va_start(argp, format);
	vfprintf(stdout, format, argp);
	va_end(argp);
	fprintf(stdout, "\n");
}

int blOrll(){
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
	if(blOrll()==1){
		return value;
	}

	return big_litle_endian(value);
}

int net_to_int(int value){
	if(blOrll()==1){
		return value;
	}

	return big_litle_endian(value);
}

int big_litle_endian(int x)
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
