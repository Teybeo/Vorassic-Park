#include "header/pileNote.h"

void affichagePileNote(NoeudNote *pile) {

    NoeudNote *temp = pile;

    while (temp != NULL) {

        printf(" %d |", temp->note);
        temp = temp->suivant;
    }

}

NoeudNote* empilerNote(NoeudNote *tetepile, int note) {

    NoeudNote *p = malloc(sizeof(NoeudNote));
    p->note = note;
    p->suivant = tetepile;
    tetepile = p;

    return p;

}

NoeudNote* depilerNote(NoeudNote* tetepile) {

    if (tetepile == NULL)
        return NULL;
    else {
        NoeudNote* tmp = tetepile->suivant;
        free(tetepile);
        return tmp;
    }
}

void liberePileNote(NoeudNote* tetepile) {

    while (tetepile != NULL) {

        tetepile = depilerNote(tetepile);
    }

}

