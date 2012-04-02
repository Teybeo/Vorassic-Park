#ifndef H_LISTE
#define H_LISTE

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

typedef struct Noeud {
    Point pos;
    struct Noeud *suivant;
} Noeud;

void affichagePile(Noeud *pile);
Noeud* empiler(Noeud *tetepile, Point depart);
Noeud* depiler(Noeud* tetepile);
void* liberePile(Noeud* tetepile);

#endif
