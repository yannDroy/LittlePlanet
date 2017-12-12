#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GL/gl.h"
#include "GL/glut.h"

#include "const.h"
#include "struct.h"
#include "generation.h"
#include "visu.h"

int mode = 0;
int auto_rot = 1;
int bool_lissage = 1;
int ocean = 1;
int terre = 1;

float angle1 = PI / 2;
float angle2 = 0;

extern Carte c;
extern Carte lissage;
extern Carte* cartes;

extern Point oeil;

extern int octaves;

extern float rayon;
extern float pas;

extern float angle1;
extern float angle2;
extern float distance;

void visualisation (){
    if(auto_rot){
        angle1 += 0.04;

        oeil.x = distance * cos(angle1) * cos(angle2);
        oeil.y = distance * sin(angle1) * cos(angle2);
        oeil.z = distance * sin(angle2);
    }
    
    glutPostRedisplay();
}

void key_pressed (unsigned char key, int x, int y){
    int i;
    
    switch(key){
    case 'q':
        if(distance < 22 * rayon){
            distance += 5;

            oeil.x = distance * cos(angle1) * cos(angle2);
            oeil.y = distance * sin(angle1) * cos(angle2);
            oeil.z = distance * sin(angle2);
        }
        
        break;
    case 'd':
        if(distance > rayon + 5.0){
            distance -= 5;

            oeil.x = distance * cos(angle1) * cos(angle2);
            oeil.y = distance * sin(angle1) * cos(angle2);
            oeil.z = distance * sin(angle2);
        }
        
        break;
    case 'a':
        angle1 -= 0.04;
        
        oeil.x = distance * cos(angle1) * cos(angle2);
        oeil.y = distance * sin(angle1) * cos(angle2);
        oeil.z = distance * sin(angle2);
        
        break;
    case 'e':
        angle1 += 0.04;
        
        oeil.x = distance * cos(angle1) * cos(angle2);
        oeil.y = distance * sin(angle1) * cos(angle2);
        oeil.z = distance * sin(angle2);
        
        break;
    case 'z':
        if(angle2 < PI / 2 - PI / 100){
            angle2 += 0.04;
        
            oeil.x = distance * cos(angle1) * cos(angle2);
            oeil.y = distance * sin(angle1) * cos(angle2);
            oeil.z = distance * sin(angle2);
        }
        
        break;
    case 's':
        if(angle2 > -PI / 2 + PI / 100){
            angle2 -= 0.04;
        
            oeil.x = distance * cos(angle1) * cos(angle2);
            oeil.y = distance * sin(angle1) * cos(angle2);
            oeil.z = distance * sin(angle2);
        }
        
        break;
    case 'p':
        mode = 1 - mode;
        break;
    case 'm':
        auto_rot = 1 - auto_rot;
        break;
    case 'l':
        bool_lissage = 1 - bool_lissage;
        break;
    case 'o':
        ocean = 1 - ocean;
        break;
    case 'k':
        terre = 1 - terre;
        break;
    case 32:
        detruire_carte(&c);
        detruire_carte(&lissage);

        for(i = 0; i < octaves; i++)
            detruire_carte(&cartes[i]);
        detruire_liste_cartes(cartes);

        creer_planete();
        
        break;
    case 27:
        detruire_carte(&c);
        detruire_carte(&lissage);

        for(i = 0; i < octaves; i++)
            detruire_carte(&cartes[i]);
        detruire_liste_cartes(cartes);

        exit(EXIT_SUCCESS);
    }
}
