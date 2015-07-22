#include "apue.h"
#include "chat_exception.h"

int exception_no = 0;

void p_chat_exception(const char *msg)
{
	char *emsg = NULL;
	switch(exception_no)
	{
		case CE_PARAM : emsg = "wrong param" ; break;
		case CE_BAD_DATA : emsg = "data package bad" ; break;
		case CE_VERIFY : emsg = "verify failed" ; break;
		default : emsg = "none exception" ; break;
	}

	printf("%s : %s \n", msg, emsg);
}

