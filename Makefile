CC=gcc
CFLAGS=-Wall -Wextra -O3 -g

comp: main.c
	$(CC) -o main $^ $(CFLAGS)
	./main