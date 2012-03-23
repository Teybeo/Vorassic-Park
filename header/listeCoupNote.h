#ifndef H_LISTE_COUP_NOTE
#define H_LISTE_COUP_NOTE

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

typedef struct NoeudCoupNote {
    Point pos;
    int note;
    struct NoeudCoupNote *suivant;
} NoeudCoupNote;

void affichageListeCoupNote(NoeudCoupNote *liste);
NoeudCoupNote* ajoutTeteCoupNote(NoeudCoupNote *teteliste, Point depart, int note);
NoeudCoupNote* ajoutFinCoupNote(NoeudCoupNote *teteliste, Point depart, int note);
NoeudCoupNote* supprTeteCoupNote(NoeudCoupNote* teteliste);
NoeudCoupNote* supprFinCoupNote(NoeudCoupNote* teteliste);
void libereListeCoupNote(NoeudCoupNote* teteliste);

#endif

