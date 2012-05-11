#include "header/pile.h"

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
