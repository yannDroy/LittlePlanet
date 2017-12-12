CC = gcc
OPTIONS = -Wall -lm -g -lglut -lGLU -lGL -lm

all: LittlePlanet

LittlePlanet : affichage.o generation.o visu.o main.o
	$(CC) $(OPTIONS) affichage.o generation.o visu.o main.o -o LittlePlanet

affichage.o : affichage.c affichage.h
	$(CC) $(OPTIONS) affichage.c -c
generation.o : generation.c generation.h
	$(CC) $(OPTIONS) generation.c -c
visu.o: visu.c visu.h
	$(CC) $(OPTIONS) -c visu.c
main.o: main.c
	$(CC) $(OPTIONS) -c main.c

clean :
	rm LittlePlanet *.o
