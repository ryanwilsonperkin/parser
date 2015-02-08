CC=gcc
LEX=flex

scan.o: scan.c
	$(CC) -o $@ -c $^

scan.c: scan.l
	$(LEX) $(LFLAGS) -o $@ $^

clean:
	$(RM) *.o scan.c
