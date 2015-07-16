all: chatserver

CC = gcc
CFLAGS = -g -Wall -ansi -D_REENTRANT

GLIBDIR = /usr/local/glib/

SRCF = src/
OUTPUTF = output/
BINF = bin/

INCLUDE = -I${GLIBDIR}include/glib-2.0 -I${GLIBDIR}include/glib-2.0/glib -I${GLIBDIR}lib/glib-2.0/include -I./header
CLIB = -L${GLIBDIR}lib -lglib-2.0 -lpthread

chatserver: ${OUTPUTF}main.o ${OUTPUTF}config.o ${OUTPUTF}hashutil.o ${OUTPUTF}strutil.o ${OUTPUTF}logicutil.o ${OUTPUTF}msg.o
	${CC} ${CFLAGS} -o ${BINF}chatserver ${OUTPUTF}main.o ${OUTPUTF}config.o ${OUTPUTF}hashutil.o ${OUTPUTF}strutil.o ${OUTPUTF}logicutil.o ${OUTPUTF}msg.o ${CLIB}

#create all c to o
${OUTPUTF}%.o: ${SRCF}%.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OUTPUTF}*.o ${BINF}chatserver
	
