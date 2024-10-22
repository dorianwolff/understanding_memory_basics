CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address

all: game

game: main.o game.o
	$(CC) $(CFLAGS) -o game main.o game.o

main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

clean:
	rm -f *.o game

