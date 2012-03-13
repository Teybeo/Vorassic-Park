#ifndef H_LISTE_MOUVMT_NOTE
#define H_LISTE_MOUVMT_NOTE

#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"

typedef struct NoeudMouvmtNote {
    Point depart;
    Point arrivee;
    int note;
    struct NoeudMouvmtNote *suivant;
} NoeudMouvmtNote;

void affichageListeMouvmtNote(NoeudMouvmtNote *liste);
NoeudMouvmtNote* ajoutTeteMouvmtNote(NoeudMouvmtNote *teteliste, Point depart, Point arrivee, int note);
NoeudMouvmtNote* ajoutFinMouvmtNote(NoeudMouvmtNote *teteliste, Point depart, Point arrivee, int note);
NoeudMouvmtNote* supprTeteMouvmtNote(NoeudMouvmtNote* teteliste);
NoeudMouvmtNote* supprFinMouvmtNote(NoeudMouvmtNote* teteliste);
void libereListeMouvmtNote(NoeudMouvmtNote* teteliste);

#endif

