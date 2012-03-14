#include "header/liste.h"

void affichageListe(Noeud *liste) {

    Noeud *temp = liste;

    while (temp != NULL) {

        printf("(%d, %d) ", temp->pos.x, temp->pos.y);
        temp = temp->suivant;
    }

}

Noeud* ajoutTete(Noeud *teteliste, Point point) {

    Noeud *p = malloc(sizeof(Noeud));
    p->pos.x = point.x;
    p->pos.y = point.y;
    p->suivant = teteliste;
    teteliste = p;

    return p;

}

Noeud* ajoutFin(Noeud *teteliste, Point point) {

    Noeud *nouveau = malloc(sizeof(Noeud));
    nouveau->pos.x = point.x;
    nouveau->pos.y = point.y;
    nouveau->suivant = NULL;

    if (teteliste == NULL)
        return nouveau;
    else {

        Noeud *tmp = teteliste;

        while (tmp->suivant != NULL)
            tmp = tmp->suivant;

        tmp->suivant = nouveau;
    }

    return teteliste;

}

Noeud* supprTete(Noeud* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else {
        Noeud* tmp = teteliste->suivant;
        free(teteliste);
        return tmp;
    }
}

Noeud* supprFin(Noeud* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else if (teteliste->suivant == NULL) {
        free(teteliste);
        return NULL;
    }

    Noeud* tmp = teteliste;

    while (tmp->suivant->suivant != NULL)
        tmp = tmp->suivant;

    free(tmp->suivant);

    tmp->suivant = NULL;

    return teteliste;
}

void libereListe(Noeud* teteliste) {

    if (teteliste != NULL) {

        teteliste = supprTete(teteliste);
    }

}
