all: chatserver

CC = gcc
CFLAGS = -g -Wall -ansi

GLIBDIR = /usr/local/glib/

SRCF = src/
OUTPUTF = output/
BINF = bin/

INCLUDE = -I${GLIBDIR}include/glib-2.0 -I${GLIBDIR}include/glib-2.0/glib -I${GLIBDIR}lib/glib-2.0/include -I./header
CLIB = -L${GLIBDIR}lib -lglib-2.0

chatserver: ${OUTPUTF}main.o ${OUTPUTF}config.o ${OUTPUTF}hashutil.o
	${CC} ${CFLAGS} -o ${BINF}chatserver ${OUTPUTF}main.o ${OUTPUTF}config.o ${OUTPUTF}hashutil.o ${CLIB}

#create all c to o
${OUTPUTF}%.o: ${SRCF}%.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -rf ${OUTPUTF}*.o ${BINF}chatserver
	
