/* Chat Exception */


#define CE_NONE 		10000		/* none exception */
#define CE_PARAM 		10001		/* wrong param exception */
#define CE_BAD_DATA 	10002		/* data package bad */
#define CE_VERIFY 		10003		/* verify failed */

#define DBE_NOINSTANCE	-20000		/* mysql no instance mysql is null */
#define DBE_STATEMENT	-20001		/* statement syntax error */

void p_chat_exception(int _eno);
void set_chat_exception(int _eno);
