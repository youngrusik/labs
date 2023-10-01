CC ?= gcc 
CFLAGS ?= -g -Werror -std=c99
all:
	$(CC) $(CFLAGS) stack.c main.c -o main

main: main.o stack.o
	${CC} -o main main.o stack.o

main.o: main.c 
	${CC} ${CFLAGS} -c main.c

functions.o: stack.c stack.o
	${CC} ${CFLAGS} -c stack.c

clean: 
	rm -rf *.o main