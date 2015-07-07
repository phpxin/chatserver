#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#ifndef NULL
	#define NULL (void *)0
#endif

#define BUFSIZE 1024

typedef enum{FALSE=0, TRUE=1}BOOL ;
typedef enum{FAILED=0, SUCC=1, ERR=-1}STATUS ;
