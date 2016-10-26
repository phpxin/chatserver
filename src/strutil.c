#include "apue.h"
#include "logicutil.h"
#include <strutil.h>

int free_string_array(char *str_array[], int len)
{
	
	for(int i=0; i<len; i++){
		myfree((void *)str_array[i]);
	}

	return 1;
}

int su_explode(char c, const char *in, char *out[], int *out_len)
{

	size_t in_len = strlen(in);

	size_t buf_block = 1;
	char *tmp = (char *)malloc(BUFSIZE);
	memset(tmp, '\0', BUFSIZE);
	size_t cursor = 0 ;
	int index = 0 ;

	for(int i=0; i<in_len; i++) {
		if (in[i] != c)
		{
			if (strlen(tmp) >= BUFSIZE*buf_block-2 )
			{
				
				buf_block++ ; /* increase block */
				tmp = (char *)realloc((void *)tmp, BUFSIZE*buf_block);
			}
			tmp[cursor] = in[i] ;
			tmp[cursor+1] = '\0' ;
			cursor++ ;
		}else{
			*(out+index) = tmp ;

			buf_block = 1 ; /* reset block */
			tmp = (char *)malloc(BUFSIZE);
			memset(tmp, '\0', BUFSIZE); /* reset tmp memory */
			cursor = 0 ;
			index++ ; /* auto increase index */
		}
	}

	*(out+index) = tmp ;
	*out_len = index + 1 ; /* length not include NULL flag */ 
	index++ ;
	*(out+index) = NULL ; /* end flag */

	return 1;

}

int su_cfpos(char *arr, char c)
{
	size_t len = strlen(arr);
	int index = -1;

	int i;
	for( i=0; i<len; i++)
	{
		if(arr[i] == c)
		{
			index = i;
		}
	}

	return index;
}

int su_trim(char *str, char *cs)
{
	su_rtrim(str, cs);
	su_ltrim(str, cs);

	return 1;
}

int su_rtrim(char *str, char *cs)
{
	size_t len = strlen(str);
	size_t end = len-1;

	char *_str = (char *)malloc(len);
	memset(_str, '\0', len);

	int i;
	int fpos = -1;

	for( i=end; i>=0; i-- )
	{
		fpos = su_cfpos(cs, str[i]);
		if(fpos>=0)
		{
			end--;
		}
		else
		{
			break;
		}
	}

	if(end < len-1)
	{
		strncpy(_str, str, end+1);
		memset(str, '\0', len);
		strcpy(str, _str);
	}

	free(_str);

	return 1;

}

int su_ltrim(char *str, char *cs)
{
    size_t len = strlen(str);
	size_t start = 0;

	char *_str = (char *)malloc(len);
	memset(_str, '\0', len);

    int i;
	int fpos = -1;

    for( i=0; i<len; i++ )
    {
		fpos = su_cfpos(cs, str[i]);
        if(fpos>=0)
		{
			start++;
		}
		else
		{
			break;
		}
        
    }

	if(start>0)
	{
		strcpy(_str, str+start);
		memset(str, '\0', len);
		strcpy(str, _str);
	}

	free(_str);

	return 1;
}
