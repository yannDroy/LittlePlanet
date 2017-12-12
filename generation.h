#ifndef __GENERATION__
#define __GENERATION__

Carte initialiser_carte (float p);
void detruire_carte (Carte* carte);
void creer_liste_cartes (int n);
void detruire_liste_cartes ();
void creer_planete ();
void adapter_altitude (Carte* carte);
int interpoler (int y1, int y2, int n, int delta);
int valeur_interpolee (int i, int j, int frequence, Carte* r);
int aleatoire (int i, int j, int hauteur, int largeur);


#endif
