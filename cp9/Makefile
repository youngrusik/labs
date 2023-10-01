CC ?= gcc 
CFLAGS ?= -g -Werror -std=c99
all:
	$(CC) $(CFLAGS) vector_key.c main.c -o main

main: main.o vector_key.o
	${CC} -o main main.o vector_key.o

main.o: main.c 
	${CC} ${CFLAGS} -c main.c

functions.o: vector_key.c vector_key.o
	${CC} ${CFLAGS} -c vector_key.c

clean: 
	rm -rf *.o main