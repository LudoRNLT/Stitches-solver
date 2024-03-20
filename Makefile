CC=gcc

all: Solver5x5 Solver7x7

Solver5x5: Stitches5x5.c
	$(CC) $(CFLAGS) $^ -o $@

Solver7x7: Stitches7x7.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f Solver5x5 Solver7x7
