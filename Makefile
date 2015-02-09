CC=gcc
LEX=flex

all: parselist

parselist: parselist.o scan.o
	$(CC) -o $@ $^ -ll

parselist.o: parselist.c
	$(CC) -o $@ -c $^

scan.o: scan.c
	$(CC) -o $@ -c $^

scan.c: scan.l
	$(LEX) $(LFLAGS) -o $@ $^

clean:
	$(RM) *.o scan.c
