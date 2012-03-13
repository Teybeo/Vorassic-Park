#ifndef H_LISTE_MOUVMT
#define H_LISTE_MOUVMT

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

typedef struct NoeudMouvmt {
    Point depart;
    Point arrivee;
    struct NoeudMouvmt *suivant;
} NoeudMouvmt;

void affichageListeMouvmt(NoeudMouvmt *liste);
NoeudMouvmt* ajoutTeteMouvmt(NoeudMouvmt *teteliste, Point depart, Point arrivee);
NoeudMouvmt* ajoutFinMouvmt(NoeudMouvmt *teteliste, Point depart, Point arrivee);
NoeudMouvmt* supprTeteMouvmt(NoeudMouvmt* teteliste);
NoeudMouvmt* supprFinMouvmt(NoeudMouvmt* teteliste);
void libereListeMouvmt(NoeudMouvmt* teteliste);

#endif
