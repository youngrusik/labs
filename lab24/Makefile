CC=gcc
LD=gcc
CCFLAGS=-Wall -Werror -Wextra -Wfatal-errors -Wpedantic -pedantic-errors -g
SOURCES=tree.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm *.o
