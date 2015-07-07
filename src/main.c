#include "apue.h"
#include "config.h"

int main(int argc, char *argv[]){
	
	if(argc<2){
		printf("usage: chatserver {config path} \n\n") ;
		return -1;
	}
	
	if( parseConfig(argv[1]) )
	{
		printf("parse config ok \n");
	}
	else
	{
		printf("parse config faled \n");
	}
	
	return 0;
}