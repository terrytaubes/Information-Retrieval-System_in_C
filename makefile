CC = gcc
CFLAGS = -lm

main: main.o engine.o
	$(CC) -o main main.o engine.o -lm