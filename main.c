#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "const.h"
#include "struct.h"
#include "generation.h"
#include "affichage.h"
#include "visu.h"

Carte c;
Carte lissage;
Carte* cartes;

Point oeil;
Point centre;

float rayon;
float pas;

int frequence;
int octaves;
int liss;
float persistance;

float distance;

/* Fonction d'usage du programme */
void usage (char* s){
    fprintf(stderr, "Usage : %s [rayon pas [frequence octaves lissage persistance]]\n", s);
    fprintf(stderr, "  * rayon : rayon de la planète à créer, %d > r > %d, défaut %d\n", MAX_RAYON, 0, DEFAUT_RAYON);
    fprintf(stderr, "  * pas : pas de précision du relief, %d < p <= (r / %d), défaut %d\n", 0, 10, DEFAUT_PAS);
    fprintf(stderr, "  * frequence : correspond à la fréquence de découpage de la carte, %d > f > %d, défaut %d\n", MAX_FREQ, 0, DEFAUT_FREQ);
    fprintf(stderr, "  * octaves : le nombre d'itérations d'interpolations, %d > o > %d, défaut %d\n", MAX_OCT, 0, DEFAUT_OCT);
    fprintf(stderr, "  * lissage : le nombre de passes lors du lissage de la carte, %d > l >= %d, défaut %d\n", MAX_LISS, 0, DEFAUT_LISS);
    fprintf(stderr, "  * persistance : correspond à la pondération des itérations successives, %f > p > %d, défaut %f\n", MAX_PERS, 0, DEFAUT_PERS);
}

/* Fonction principale */
int main (int argc, char *argv[]){
    int i;
    
    // Test des arguments du programme
    if(argc == 1){
        rayon = DEFAUT_RAYON;
        pas = DEFAUT_PAS;

        frequence = DEFAUT_FREQ;
        octaves = DEFAUT_OCT;
        liss = DEFAUT_LISS;
        persistance = DEFAUT_PERS;
    }else if(argc == 3){
        rayon = atof(argv[1]);
        pas = atof(argv[2]);

        if(rayon <= 0 || rayon > MAX_RAYON ||
           pas <= 0 || pas > (rayon / 10)){
           
            usage(argv[0]);
            return EXIT_FAILURE;
        }
        
        frequence = DEFAUT_FREQ;
        octaves = DEFAUT_OCT;
        liss = DEFAUT_LISS;
        persistance = DEFAUT_PERS;
    }else if(argc == 7){
        rayon = atof(argv[1]);
        pas = atof(argv[2]);
        frequence = atoi(argv[3]);
        octaves = atoi(argv[4]);
        liss = atoi(argv[5]);
        persistance = atof(argv[6]);
        
        if(rayon <= 0 || rayon > MAX_RAYON ||
           pas <= 0 || pas > (rayon / 10) ||
           frequence <= 0 || frequence > MAX_FREQ ||
           octaves <= 0 || octaves > MAX_OCT ||
           liss < 0 || liss > MAX_LISS ||
           persistance <= 0.0 || persistance > MAX_PERS){
           
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }else{
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    distance = 12 * rayon;
    
    srand(time(NULL));
    
    // Initialisation d'OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutInitWindowSize(WINDOW_WIDTH , WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POS_X , WINDOW_POS_Y);
    glutCreateWindow("Little Planet 1.5");

    glutDisplayFunc(affichage);
    glutIdleFunc(visualisation);

    glutKeyboardFunc(key_pressed);         
    //glutKeyboardUpFunc(key_released);
    
    glShadeModel(GL_SMOOTH);

    creer_planete();

    oeil.x = 0;
    oeil.y = distance;
    oeil.z = 0;

    glutMainLoop();

    detruire_carte(&c);
    detruire_carte(&lissage);

    for(i = 0; i < octaves; i++)
        detruire_carte(&cartes[i]);
    detruire_liste_cartes();

    return EXIT_SUCCESS;
}
