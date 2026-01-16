CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c11

all: record_editor

record_editor: main.c
	$(CC) $(CFLAGS) -o record_editor main.c

clean:
	rm -f record_editor *.o
