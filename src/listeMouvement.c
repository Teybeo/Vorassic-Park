#include "header/listeMouvement.h"

void affichageListeMouvmt(NoeudMouvmt *liste) {

    NoeudMouvmt *temp = liste;

    while (temp != NULL) {

        printf(" (%d, %d) |", temp->depart.x, temp->depart.y);
        printf(" (%d, %d) \n", temp->arrivee.x, temp->arrivee.y);
        temp = temp->suivant;
    }

}

NoeudMouvmt* ajoutTeteMouvmt(NoeudMouvmt *teteliste, Point depart, Point arrivee) {

    NoeudMouvmt *p = calloc(1, sizeof(NoeudMouvmt));
    p->depart = depart;
    p->arrivee = arrivee;
    p->suivant = teteliste;
    teteliste = p;

    return p;

}

NoeudMouvmt* ajoutFinMouvmt(NoeudMouvmt *teteliste, Point depart, Point arrivee) {

    NoeudMouvmt *nouveau = calloc(1, sizeof(NoeudMouvmt));
    nouveau->depart = depart;
    nouveau->arrivee = arrivee;
    nouveau->suivant = NULL;

    if (teteliste == NULL)
        return nouveau;
    else {

        NoeudMouvmt *tmp = teteliste;

        while (tmp->suivant != NULL)
            tmp = tmp->suivant;

        tmp->suivant = nouveau;
    }

    return teteliste;

}

NoeudMouvmt* supprTeteMouvmt(NoeudMouvmt* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else {
        NoeudMouvmt* tmp = teteliste->suivant;
        free(teteliste);
        return tmp;
    }
}

NoeudMouvmt* supprFinMouvmt(NoeudMouvmt* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else if (teteliste->suivant == NULL) {
        free(teteliste);
        return NULL;
    }

    NoeudMouvmt* tmp = teteliste;

    while (tmp->suivant->suivant != NULL)
        tmp = tmp->suivant;

    free(tmp->suivant);

    tmp->suivant = NULL;

    return teteliste;
}

void libereListeMouvmt(NoeudMouvmt* teteliste) {

    if (teteliste != NULL) {

        teteliste = supprTeteMouvmt(teteliste);
    }

}


