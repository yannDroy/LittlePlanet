#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "const.h"
#include "struct.h"
#include "affichage.h"

int max;

extern Carte c;
extern Carte lissage;

extern Point oeil;

extern float rayon;
extern float pas;

extern int mode;
extern int bool_lissage;
extern int ocean;
extern int terre;

/* Fonction de rendu */
void affichage (){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    afficher_planete();

    init_oeil();
    //init_lumiere();
    
    glutSwapBuffers();
}

/* Placement de l'oeil */
void init_oeil (){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-0.01, 0.01,
              -0.01, 0.01,
              0.1, 30 * rayon);

    gluLookAt(oeil.x, oeil.y, oeil.z,
              CENTRE_X, CENTRE_Y, CENTRE_Z,
              0, 0, 1);
}

/* Placement de la lumiere */
void init_lumiere (){
    GLfloat blanc[4] = {1.0, 1.0, 1.0, 1.0};
    
    int position_lumiere[4] = {(int)oeil.x, (int)oeil.y, (int)oeil.z, 1};
    GLfloat direction[3] = {CENTRE_X, CENTRE_Y, CENTRE_Z};

    glEnable(GL_LIGHTING);

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, blanc);
    glLightiv(GL_LIGHT0, GL_POSITION, position_lumiere);
        
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 3.25);
}

/* Affichage de la planete sous forme de facettes */
void afficher_planete (){
    Carte a_afficher;
    float phi, theta;
    float pas_phi, pas_theta;
    float alt1, alt2, alt3, alt4;
    int test;
    int i, j;

    /*float ambient[4] = {0.25, 0.20725, 0.20725, 1.0};
    float diffuse[3] = {1.0, 0.829, 0.829};
    float specular[3] = {0.296648, 0.296648, 0.296648};
    float shine = 0.6;*/

    if(bool_lissage)
        a_afficher = lissage;
    else
        a_afficher = c;

    pas_phi = PI / (c.hauteur);
    pas_theta = 2 * PI / (c.largeur);

    glDisable(GL_COLOR_MATERIAL);

    /*glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shine * 128.0);*/

    phi = -PI / 2;

    for(i = 0; i < c.hauteur; i++){
        theta = 0;
        
        for(j = 0; j < c.largeur; j++){
            glEnable(GL_COLOR_MATERIAL);
            
            test = a_afficher.m[i][j];

            if(terre){
                if(test > 4.6 * max / 6)
                    glColor3f(1.0, 1.0, 0.9);
                else if(test > 3.2 * max / 6)
                    glColor3f(0.45, 0.5, 0.6);
                else if(test > 2.2 * max / 6)
                    glColor3f(0.3 ,0.2, 0.1);
                else if(test >= 0.05 * max / 6)
                    glColor3f(0.0, 0.4, 0.1);
                else if(test >= 0.0)
                    glColor3f(0.4, 0.5, 0.0);
                else
                    glColor3f(0.1, 0.1, 0.1);
            }else{
                if(test > 4.6 * max / 6)
                    glColor3f(0.3, 0.2, 0.2);
                else if(test > 3.2 * max / 6)
                    glColor3f(0.1 ,0.1, 0.2);
                else if(test > 2.2 * max / 6)
                    glColor3f(0.02, 0.0, 0.2);
                else if(test >= 0.05 * max / 6)
                    glColor3f(0.1, 0.05, 0.1);
                else if(test >= 0.0)
                    glColor3f(0.1, 0.1, 0.1);
                else
                    glColor3f(0.04, 0.03, 0.04);
            }

            alt1 = rayon + a_afficher.m[i][j];
            if(alt1 < rayon && ocean)
                alt1 = rayon;
            
            if(i < c.hauteur - 1){
                alt2 = rayon + a_afficher.m[i + 1][j];
                if(alt2 < rayon && ocean)
                    alt2 = rayon;
                
                alt3 = rayon + a_afficher.m[i + 1][(j + 1) % c.largeur];
                if(alt3 < rayon && ocean)
                    alt3 = rayon;
            }
            
            alt4 = rayon + a_afficher.m[i][(j + 1) % c.largeur];
            if(alt4 < rayon && ocean)
                alt4 = rayon;

            // Premier triangle
            if(mode)
                glBegin(GL_POLYGON);
            else
                glBegin(GL_LINE_STRIP);

            if(alt1 == rayon && alt2 == rayon && alt4 == rayon && ocean){
                if(terre)
                    glColor3f(0.1, 0.1, 0.4);
                else
                    glColor3f(0.5, 0.0, 0.0);
            }
            
            glNormal3d(alt1 * cos(theta) * cos(phi), alt1 * sin(theta) * cos(phi), alt1 * sin(phi));

            glVertex3f(alt1 * cos(theta) * cos(phi), alt1 * sin(theta) * cos(phi), alt1 * sin(phi));
            glVertex3f(alt2 * cos(theta) * cos(phi + pas_phi), alt2 * sin(theta) * cos(phi + pas_phi), alt2 * sin(phi + pas_phi));
            //glVertex3f(alt3 * cos(theta + pas_theta) * cos(phi + pas_phi), alt3 * sin(theta + pas_theta) * cos(phi + pas_phi), alt3 * sin(phi + pas_phi));
            glVertex3f(alt4 * cos(theta + pas_theta) * cos(phi), alt4 * sin(theta + pas_theta) * cos(phi), alt4 * sin(phi));
            
            glEnd();

            // Deuxieme triangle
            if(mode)
                glBegin(GL_POLYGON);
            else
                glBegin(GL_LINE_STRIP);

            if(alt2 == rayon && alt3 == rayon && alt4 == rayon && ocean){
                if(terre)
                    glColor3f(0.1, 0.1, 0.4);
                else
                    glColor3f(0.5, 0.0, 0.0);
            }

           glNormal3d(alt1 * cos(theta) * cos(phi), alt1 * sin(theta) * cos(phi), alt1 * sin(phi));

            //glVertex3f(alt1 * cos(theta) * cos(phi), alt1 * sin(theta) * cos(phi), alt1 * sin(phi));
            glVertex3f(alt2 * cos(theta) * cos(phi + pas_phi), alt2 * sin(theta) * cos(phi + pas_phi), alt2 * sin(phi + pas_phi));
            glVertex3f(alt3 * cos(theta + pas_theta) * cos(phi + pas_phi), alt3 * sin(theta + pas_theta) * cos(phi + pas_phi), alt3 * sin(phi + pas_phi));
            glVertex3f(alt4 * cos(theta + pas_theta) * cos(phi), alt4 * sin(theta + pas_theta) * cos(phi), alt4 * sin(phi));
            
            glEnd();
            
            theta += pas_theta;
        }

        phi += pas_phi;
    }

    if(!mode){
        glColor3f(0.0, 0.0, 0.0);
        
        if(ocean)
            glutSolidSphere(rayon - rayon / 8, 20, 20);
        else
            glutSolidSphere(rayon - rayon / 5, 20, 20);
    }

    glDisable(GL_COLOR_MATERIAL);
}
