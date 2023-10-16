LIBRARY = matrix.c
MAIN = main.c 
OUT = main.out

all:
	gcc -std=c17 $(MAIN) $(LIBRARY) -o $(OUT)

debug: 
	gcc -g $(MAIN) $(LIBRARY) -o $(OUT)

clean:
	rm -f *.out