#include "apue.h"
#include "logicutil.h"
#include "chat_exception.h"

void p_chat_exception(int _eno)
{
	char *emsg = NULL;
	switch(_eno)
	{
		case CE_PARAM : emsg = "wrong param" ; break;
		case CE_BAD_DATA : emsg = "data package bad" ; break;
		case CE_VERIFY : emsg = "verify failed" ; break;
		
		case DBE_NOINSTANCE : emsg = "db instance not exist" ; break;
		
		default : emsg = "none exception" ; break;
	}

	/*printf("%s\n", emsg);*/
	elog(E_ERROR, "%s\n", emsg);
}

void set_chat_exception(int level, int _eno)
{
	/* do somethine , example send sms to admin */
	elog(level, "catch exception , number is %d", _eno);
}
