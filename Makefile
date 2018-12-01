CC = gccYann
OPTIONS = -Wall -g -lGL -lGLU -lglut -lm

all: LittlePlanet

LittlePlanet : affichage.o generation.o visu.o main.o
	$(CC) affichage.o generation.o visu.o main.o -o LittlePlanet $(OPTIONS)

affichage.o : affichage.c affichage.h
	$(CC) -c affichage.c $(OPTIONS)
generation.o : generation.c generation.h
	$(CC) -c generation.c $(OPTIONS)
visu.o: visu.c visu.h
	$(CC) -c visu.c $(OPTIONS)
main.o: main.c
	$(CC) -c main.c $(OPTIONS)

clean :
	rm LittlePlanet *.o
