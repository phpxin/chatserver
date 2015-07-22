/* Chat Exception */


#define CE_NONE 0		/* none exception */
#define CE_PARAM 1		/* wrong param exception */
#define CE_BAD_DATA 2	/* data package bad */
#define CE_VERIFY 3		/* verify failed */

void p_chat_exception(const char *msg);
