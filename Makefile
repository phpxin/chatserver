all: chatserver

CC = gcc
CFLAGS = -g -Wall -ansi

SRCF = src/
OUTPUTF = output/
BINF = bin/

chatserver: ${OUTPUTF}main.o
	${CC} ${CFLAGS} -o ${BINF}chatserver ${OUTPUTF}main.o

#create all c to o
${OUTPUTF}%.o: ${SRCF}%.c
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm -rf ${OUTPUTF}main.o ${BINF}chatserver
	
