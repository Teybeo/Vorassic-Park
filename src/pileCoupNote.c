#include "header/pileCoupNote.h"

void affichagePileCoupNote(NoeudCoupNote *pile) {

    NoeudCoupNote *temp = pile;

    while (temp != NULL) {

        printf(" (%d, %d) => %d\n", temp->pos.x, temp->pos.y, temp->note);
        temp = temp->suivant;
    }

}

NoeudCoupNote* empilerCoupNote(NoeudCoupNote *tetepile, Point coup, int note) {

    NoeudCoupNote *p = calloc(1, sizeof(NoeudCoupNote));
    p->pos = coup;
    p->note = note;
    p->suivant = tetepile;
    tetepile = p;

    return p;

}

NoeudCoupNote* depilerCoupNote(NoeudCoupNote* tetepile) {

    if (tetepile == NULL)
        return NULL;
    else {
        NoeudCoupNote* tmp = tetepile->suivant;
        free(tetepile);
        return tmp;
    }
}

void liberePileCoupNote(NoeudCoupNote* tetepile) {

    while (tetepile != NULL) {

        tetepile = depilerCoupNote(tetepile);
    }

}



