CC=cc
CFLAGS=-Wall -Wpedantic -o
DEBUG=-g

all: logfind

logfind: main.o logfind.o
	$(CC) -DNDEBUG $(CFLAGS) logfind logfind.o main.o

main.o:
	$(CC) -c -DNDEBUG $(CFLAGS) main.o main.c

logfind.o:
	$(CC) -c -DNDEBUG $(CFLAGS) logfind.o logfind.c

debug: debug_logfind

debug_logfind: debug_main.o debug_logfind.o
	$(CC) $(DEBUG) $(CFLAGS) logfind logfind.o main.o

debug_main.o:
	$(CC) -c $(DEBUG) $(CFLAGS) main.o main.c

debug_logfind.o:
	$(CC) -c $(DEBUG) $(CFLAGS) logfind.o logfind.c
clean:
	rm -rf *.o *~ logfind

