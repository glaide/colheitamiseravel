CC=gcc
CFLAGS=-g -Wall -lm
OBJ = personagem.o jogo.o main.o 

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) 

all: main

main: $(OBJ)

	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY:	clean

clean: 

	rm *.o 

purge: 

	rm main
	