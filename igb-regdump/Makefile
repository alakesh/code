CC=gcc
CFLAGS=-g

%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS)
regdump: regdump.o
	$(CC) -o regdump regdump.o -I.
clean:
	rm -rf *.o
	rm regdump

