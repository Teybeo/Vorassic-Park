#ifndef H_LISTENOTE
#define H_LISTENOTE

#include <stdio.h>
#include <stdlib.h>

typedef struct NoeudNote {
    int note;
    struct NoeudNote *suivant;
} NoeudNote;

void affichageListeNote(NoeudNote *liste);
NoeudNote* ajoutTeteNote(NoeudNote *teteliste, int note);
NoeudNote* ajoutFinNote(NoeudNote *teteliste, int note);
NoeudNote* supprTeteNote(NoeudNote* teteliste);
NoeudNote* supprFinNote(NoeudNote* teteliste);
void libereListeNote(NoeudNote* teteliste);

#endif

