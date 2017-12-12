#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GL/gl.h"
#include "GL/glut.h"

#include "const.h"
#include "struct.h"
#include "generation.h"

extern Carte c;
extern Carte lissage;
extern Carte* cartes;

extern Point oeil;

extern int frequence;
extern int octaves;
extern int liss;
extern float persistance;

extern float rayon;
extern float pas;

extern int max;

/* Initialise la carte du terrain */
Carte initialiser_carte (float p){
    Carte carte;
    int i, j;
    
    carte.largeur = 2 * 3 * rayon / pas;
    carte.hauteur = carte.largeur / 2;

    carte.persistance = p;

    carte.m = NULL;
    carte.m = (int**) malloc(carte.hauteur * sizeof(int*));

    if(carte.m == NULL){
        fprintf(stderr, "Impossible de créer la matrice de la carte\n");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < carte.hauteur; i++){
        carte.m[i] = NULL;
        carte.m[i] = (int*) malloc(carte.largeur * sizeof(int));

        if(carte.m[i] == NULL){
            fprintf(stderr, "Impossible de créer la ligne %d de la matrice de la carte\n", i);
            exit(EXIT_FAILURE);
        }

        for(j = 0; j < carte.largeur; j++)
            carte.m[i][j] = 0;
    }

    return carte;
}

/* Libere la memoire allouee a la carte */
void detruire_carte (Carte* carte){
    int i;

    for(i = 0; i < carte->hauteur; i++)
        free(carte->m[i]);

    free(carte->m);
}

/* Cree la liste des cartes pour l'interpolation */
void creer_liste_cartes (int n){
    cartes = NULL;
    cartes = (Carte*) malloc(n * sizeof(Carte));

    if(cartes == NULL){
        fprintf(stderr, "Impossible de créer la liste des cartes\n");
        exit(EXIT_FAILURE);
    }
}

/* Detruit la liste des cartes */
void detruire_liste_cartes (){
    free(cartes);
}

/* Cree le terrain aleatoire */
void creer_planete (){
    Carte random;
    float f_courante;
    float p_courante, somme_p;
    int v;
    int i, j, k, l, n;

    
    // Initialisations
    creer_liste_cartes(octaves);
    
    c = initialiser_carte(1.0);
    random = initialiser_carte(1.0);
    lissage = initialiser_carte(0.0);

    for(i = 0; i < random.hauteur; i++){
        for(j = 0; j < random.largeur; j++)
            random.m[i][j] = aleatoire(i, j, random.hauteur, random.largeur);
    }

    
    p_courante = persistance;

    // Calques de travail
    for (i = 0; i < octaves; i++){
        cartes[i] = initialiser_carte(p_courante);
        p_courante *= persistance;
    }

    
    f_courante = frequence;

    // Remplissage des calques successifs
    for(n = 0; n < octaves; n++){
        for(i = 0; i < c.hauteur; i++){
            for(j = 0; j < c.largeur; j++){
                v = valeur_interpolee(i, j, f_courante, &random);
                cartes[n].m[i][j] = v;
            }
        }
        
        f_courante *= frequence;
    }


    // Somme ponderee des calques
    somme_p = 0;
    for(i = 0; i < octaves; i++)
        somme_p += cartes[i].persistance;

    for(i = 0; i < c.hauteur; i++){
        for(j = 0; j < c.largeur; j++){
            for(n = 0; n < octaves; n++)
                c.m[i][j] += cartes[n].m[i][j] * cartes[n].persistance;

            c.m[i][j] /= somme_p;
        }
    }

    
    // Lissage
    for(i = 0; i < lissage.hauteur; i++){
        for(j = 0; j < lissage.largeur; j++){
            v = 0;
            n = 0;
            
            for(k = i - liss; k <= i + liss; k++){
                for(l = j - liss; l <= j + liss; l++)
                    if((k >= 0) && (k < lissage.hauteur) &&
                       (l >= 0) && (l < lissage.largeur)){
                        n++;
                        v += c.m[k][l];
                    }
            }
            
            lissage.m[i][j] = (float) v / n;
        }
    }

    adapter_altitude(&c);
    adapter_altitude(&lissage);
    
    detruire_carte(&random);
}

/* Renvoit une valeur interpolee entre y1 et y2 */
int interpoler (int y1, int y2, int n, int delta){
    float fact1, fact2;
    float a;
    
    // interpolation non linéaire
    if (n == 0)
        return y1;
    if (n == 1)
        return y2;

    a = (float) delta / n;

    fact1 = (3 * (1 - a) * (1 - a)) - (2 * (1 - a) * (1 - a) * (1 - a));
    fact2 = (3 * a * a) - (2 * a * a * a);

    return y1 * fact1 + y2 * fact2;
}

/* Renvoit une valeur interpolee pour l'altidtude d'un point */
int valeur_interpolee (int i, int j, int frequence, Carte* r){
    int borne1x, borne1y, borne2x, borne2y;
    int b00,b01,b10,b11;
    int v1, v2;
    int q;
    float pas_x, pas_y;
    

    // Bornes
    pas_x = (float) r->hauteur / frequence;
    pas_y = (float) r->largeur / frequence;

    q = (float) i / pas_x;
    borne1x = q * pas_x;
    borne2x = (q + 1) * pas_x;

    if(borne2x >= r->hauteur)
        borne2x = r->hauteur - 1;

    q = (float) j / pas_y;
    borne1y = q * pas_y;
    borne2y = (q + 1) * pas_y;

    if(borne2y >= r->largeur)
        borne2y -= r->largeur;
    

    // Calculs
    b00 = r->m[borne1x][borne1y];
    b01 = r->m[borne1x][borne2y];
    b10 = r->m[borne2x][borne1y];
    b11 = r->m[borne2x][borne2y];

    v1 = interpoler(b00, b01, borne2y - borne1y, j - borne1y);
    v2 = interpoler(b10, b11, borne2y - borne1y, j - borne1y);

    return interpoler(v1, v2, borne2x - borne1x , i - borne1x);
}

/* Genere un entier entre 0 et une borne fonction de l'emplacement de la case (i,j) */
int aleatoire (int i, int j, int hauteur, int largeur){
    if(i > hauteur / 16 && i < 15 * hauteur / 16 &&
       j > largeur / 8 && j < 7 * largeur / 8)
        return rand() % 256;

    if(i > hauteur / 24 && i < 23 * hauteur / 24 &&
       j > largeur / 16 && j < 15 * largeur / 16)
        return rand() % 200;

    return rand() % 120;
}

/* Adapte l'altitude au rayon de la planete */
void adapter_altitude (Carte* carte){
    int i, j;

    max = 0;
    
    for(i = 0; i < carte->hauteur; i++){
        for(j = 0; j < carte->largeur; j++){
            carte->m[i][j] -= 110;
            carte->m[i][j] /= 5;

            /*if(rayon + carte->m[i][j] < rayon)
            carte->m[i][j] = 0;*/

            if(carte->m[i][j] > max)
                max = carte->m[i][j];
        }
    }
}
