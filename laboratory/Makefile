CC := gcc
LIB := libportaudio.a
CFLAGS := -Wall -Wextra -pedantic -g -Wwrite-strings
LAFLAGS := -lrt -lm -lasound -ljack -pthread

D_EXEC:= executables

PHONY: all

all: main patest_sync paex_saw


main: main.c
	$(CC) main.c $(LIB) $(CFLAGS) $(LAFLAGS) -o $(D_EXEC)/main

patest_sync: patest_sync.c
	$(CC) patest_sync.c $(LIB) $(CFLAGS) $(LAFLAGS) -o $(D_EXEC)/patest_sync

paex_saw: paex_saw.c
	$(CC) paex_saw.c $(LIB) $(CFLAGS) $(LAFLAGS) -o $(D_EXEC)/paex_saw
