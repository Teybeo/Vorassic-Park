#include "header/pile.h"

void affichagePile(ElemPoint *pile) {

    ElemPoint *temp = pile;

    while (temp != NULL) {

        printf("(%d, %d) ", temp->pos.x, temp->pos.y);
        temp = temp->suivant;
    }

}

ElemPoint* empiler(ElemPoint *tetepile, Point point) {

    ElemPoint *nouveau = malloc(sizeof(ElemPoint));
    nouveau->pos = point;
    nouveau->suivant = tetepile;

    return nouveau;

}

ElemPoint* depiler(ElemPoint* tetepile) {

    if (tetepile == NULL)
        return NULL;

    else {
        ElemPoint* restant = tetepile->suivant;
        free(tetepile);
        return restant;
    }
}

void* liberePile(ElemPoint* tetepile) {

    while (tetepile != NULL) {

        tetepile = depiler(tetepile);
    }

    return NULL;

}
