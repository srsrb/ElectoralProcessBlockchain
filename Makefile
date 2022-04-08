CC=gcc
CFLAGS=-Wall -lm

PART1=./src/Partie1
PART2=./src/Partie2
PART3=./src/Partie3
PART4=./src/Partie4

BIN=./bin
OBJ=./obj

all : main1 main2 main3 main4

main1 : $(PART1)/main1.c $(OBJ)/part1.o
	$(CC) $^ -o $(BIN)/main1.bin $(CFLAGS)

main2 : $(PART2)/main2.c $(OBJ)/part1.o $(OBJ)/part2.o
	$(CC) $^ -o $(BIN)/main2.bin $(CFLAGS)

main3 : $(PART3)/main3.c $(OBJ)/part1.o $(OBJ)/part2.o $(OBJ)/part3.o
	$(CC) $^ -o $(BIN)/main3.bin $(CFLAGS)

main4 : $(PART4)/main4.c $(OBJ)/part1.o $(OBJ)/part2.o $(OBJ)/part3.o $(OBJ)/part4.o
	$(CC) $^ -o $(BIN)/main4.bin $(CFLAGS)

$(OBJ)/part1.o : $(PART1)/part1.c
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/part2.o : $(PART2)/part2.c
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/part3.o : $(PART3)/part3.c
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/part4.o : $(PART4)/part4.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean :
	rm $(OBJ)/*

bigclean: clean
	rm $(BIN)/*
	rm ./data/*