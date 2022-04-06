CC=gcc
CFLAGS=-Wall -lm

all : exe

exe : main.o Partie1/part1.o Partie2/part2.o Partie3/part3.o Partie4/part4.o
	$(CC) -o exe main.o Partie1/part1.o Partie2/part2.o Partie3/part3.o Partie4/part4.o $(CFLAGS)

main.o : main.c
	$(CC) -o main.o -c main.c $(CFLAGS)

part1.o : Partie1/part1.c
	$(CC) -o Partie1/part1.o -c Partie1/part1.c $(CFLAGS)

part2.o : Partie2/part2.c
	$(CC) -o Partie2/part2.o -c Partie2/part2.c $(CFLAGS)

part3.o : Partie3/part3.c
	$(CC) -o Partie3/part3.o -c Partie3/part3.c $(CFLAGS)

part4.o : Partie4/part4.c
	$(CC) -o Partie4/part4.o -c Partie4/part4.c $(CFLAGS)

clean :
	rm -rf Partie1/*.o
	rm -rf Partie2/*.o
	rm -rf Partie3/*.o
	rm -rf Partie4/*.o
	rm -rf *.o
	rm exe