#ifndef H_LISTE
#define H_LISTE

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

typedef struct Noeud {
    Point pos;
    struct Noeud *suivant;
} Noeud;

void affichageListe(Noeud *liste);
Noeud* ajoutTete(Noeud *teteliste, Point depart);
Noeud* ajoutFin(Noeud *teteliste, Point depart);
Noeud* supprTete(Noeud* teteliste);
Noeud* supprFin(Noeud* teteliste);
void libereListe(Noeud* teteliste);

#endif
