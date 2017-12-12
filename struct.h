#ifndef __STRUCT__
#define __STRUCT__

// Point de l'espace
typedef struct Point {
    float x;
    float y;
    float z;
} Point;

// Planete
typedef struct Carte {
    int** m;
    int largeur;
    int hauteur;
    float persistance;
} Carte;

#endif
