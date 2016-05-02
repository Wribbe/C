CC := gcc
LIB := libportaudio.a
CFLAGS := -Wall -Wextra -pedantic -g -Wwrite-strings
LAFLAGS := -lrt -lm -lasound -ljack -pthread

main: main.c
	$(CC) main.c $(LIB) $(CFLAGS) $(LAFLAGS) -o main
