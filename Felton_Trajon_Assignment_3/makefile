# Define required macros here
SHELL = /bin/sh
OBJS =  main.o BoundedBuffer.o BufferItem.o BufferItemCollection.o
CFLAGS = -Wall -g -std=c++11 
CC = g++
INCLUDEs =
LIBS = -pthread

assignment3:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} ${LIBS} -o $@ ${OBJS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<