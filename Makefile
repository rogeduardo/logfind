#Name of the project
EXEC=logfind

#Compiler and linker
CC=cc
#Flags for compiler
CFLAGS=-Wall -Wpedantic -o
#Flag for debug
DEBUG=-g
#.c files
C_SOURCE=$(wildcard *.c)
#.h files
H_SOURCE=$(wildcard *.h)

all: $(EXEC)

$(EXEC): main.o $(EXEC).o
	@ echo 'Building binary using gcc linker: $@'
	$(CC) -DNDEBUG $(CFLAGS) $@ $^
	@ echo 'Finished building binary: $@'
	@ echo ' '

main.o:
	@ echo 'Building target using gcc compiler: $<'
	$(CC) -c -DNDEBUG $(CFLAGS) main.o main.c
	@ echo ' '

%.o: %.c %.h
	@ echo 'Building target using gcc compiler: $<'
	$(CC) -c -DNDEBUG $(CFLAGS) $@ $<
	@ echo ' '

debug: debug_$(EXEC)

debug_$(EXEC): debug_main.o debug_$(EXEC).o
	@ echo 'Building binary, for debug, using gcc linker: $@'
	$(CC) $(DEBUG) $(CFLAGS) $(EXEC) main.o $(EXEC).o
	@ echo 'Finished building binary: $@'
	@ echo ' '

debug_main.o:
	@ echo 'Building target using gcc compiler: $<'
	$(CC) -c $(DEBUG) $(CFLAGS) main.o main.c
	@ echo ' '

debug_$(EXEC).o: $(EXEC).c $(EXEC).h
	@ echo 'Building target using gcc compiler: $<'
	$(CC) -c $(DEBUG) $(CFLAGS) $(EXEC).o $(EXEC).c
	@ echo ' '

clean:
	rm -rf *.o *~ $(EXEC)

