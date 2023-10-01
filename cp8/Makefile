CC = gcc
CFLAGS ?= -g -Wall -Wextra -std=c99 -w -lm

all:
	$(CC) $(CFLAGS)  list.c main.c machine_word.c -o main

main: main.o  list.o machine_word.o
	${CC} -o main main.o  list.o 

main.o: main.c 
	${CC} ${CFLAGS} -c main.c

functions.o: list.c list.o machine_word.c machine_word.o
	${CC} ${CFLAGS} -c  list.c machine_word.c

clean: 
	rm -rf *.o main