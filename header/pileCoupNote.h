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

void affichagePileCoupNote(NoeudCoupNote *pile);
NoeudCoupNote* empilerCoupNote(NoeudCoupNote *tetepile, Point depart, int note);
NoeudCoupNote* depilerCoupNote(NoeudCoupNote* tetepile);
void liberePileCoupNote(NoeudCoupNote* tetepile);

#endif

