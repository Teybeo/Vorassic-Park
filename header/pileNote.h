#ifndef H_LISTENOTE
#define H_LISTENOTE

#include <stdio.h>
#include <stdlib.h>

typedef struct NoeudNote {
    int note;
    struct NoeudNote *suivant;
} NoeudNote;

void affichagePileNote(NoeudNote *pile);
NoeudNote* empilerNote(NoeudNote *tetepile, int note);
NoeudNote* depilerNote(NoeudNote* tetepile);
void liberePileNote(NoeudNote* tetepile);

#endif

