CC = gcc
CFLAGS = -Wall

all: kp06 program

kp06: main.o
	$(CC) $(CFLAGS) main.o -o kp06

program: to_bin.o
	$(CC) $(CFLAGS) to_bin.o -o program
 
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

to_bin.o: to_bin.c
	$(CC) $(CFLAGS) -c to_bin.c

clean:
	rm -f *.o kp06 program
