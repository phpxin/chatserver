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


void myfree(void *var)
{
	if(var!=NULL)
	{
		free(var);
		var = NULL;
	}
}
