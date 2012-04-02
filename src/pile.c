#include "header/pile.h"

void affichagePile(Noeud *pile) {

    Noeud *temp = pile;

    while (temp != NULL) {

        printf("(%d, %d) ", temp->pos.x, temp->pos.y);
        temp = temp->suivant;
    }

}

Noeud* empiler(Noeud *tetepile, Point point) {

    Noeud *p = malloc(sizeof(Noeud));
    p->pos = point;
    p->suivant = tetepile;
    tetepile = p;

    return p;

}

Noeud* depiler(Noeud* tetepile) {

    if (tetepile == NULL)
        return NULL;
    else {
        Noeud* tmp = tetepile->suivant;
        free(tetepile);
        return tmp;
    }
}

void* liberePile(Noeud* tetepile) {

    while (tetepile != NULL) {

        tetepile = depiler(tetepile);
    }

    return NULL;

}
