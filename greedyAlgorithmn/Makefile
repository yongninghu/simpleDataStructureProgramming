# $Id: Makefile,v 1.4 2013-010-16 20:49:28-08 - - $

GCC       = gcc -g -O0 -Wall -std=gnu99

CSOURCE   = greedy04.c inputLib.c loadWgtGraph.c minPQ.c edgeList.c
CHEADER   = greedy04.h inputLib.h loadWgtGraph.h minPQ.h edgeList.h
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = greedy04

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	${GCC} -c $<

