# Define required macros here
SHELL = /bin/sh
OBJS =  main.o FCFS.o RR.o Processor.o PCB.o
CFLAG = -Wall -g
CC = g++
INCLUDE =
#LIBS = -lm

assignment2:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<