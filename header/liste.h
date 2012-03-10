#ifndef H_LISTE
#define H_LISTE

#include <stdio.h>
#include <stdlib.h>

typedef struct Noeud {
    int x;
    int y;
    struct Noeud *suivant;
} Noeud;

void affichageListe(Noeud *liste);
Noeud* ajoutertete(Noeud *teteliste, int x, int y);
Noeud* ajouterfin(Noeud *teteliste, int x, int y);
Noeud* supprFin(Noeud* teteliste);
Noeud* supprTete(Noeud* teteliste);

#endif
