#include "header/pile.h"

void affichagePile(ElemPoint *pile) {

    ElemPoint *temp = pile;

    while (temp != NULL) {

        printf("(%d, %d) ", temp->pos.x, temp->pos.y);
        temp = temp->suivant;
    }

}

ElemPoint* empiler(ElemPoint *tetepile, Point point) {

    ElemPoint *p = malloc(sizeof(ElemPoint));
    p->pos = point;
    p->suivant = tetepile;
    tetepile = p;

    return p;

}

ElemPoint* depiler(ElemPoint* tetepile) {

    if (tetepile == NULL)
        return NULL;
    else {
        ElemPoint* tmp = tetepile->suivant;
        free(tetepile);
        return tmp;
    }
}

void* liberePile(ElemPoint* tetepile) {

    while (tetepile != NULL) {

        tetepile = depiler(tetepile);
    }

    return NULL;

}
