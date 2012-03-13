#include "header/listeMouvementNote.h"

void affichageListeMouvmtNote(NoeudMouvmtNote *liste) {

    NoeudMouvmtNote *temp = liste;

    while (temp != NULL) {

        printf(" (%d, %d) |", temp->depart.x, temp->depart.y);
        printf(" (%d, %d) => %d\n", temp->arrivee.x, temp->arrivee.y, temp->note);
        temp = temp->suivant;
    }

}

NoeudMouvmtNote* ajoutTeteMouvmtNote(NoeudMouvmtNote *teteliste, Point depart, Point arrivee, int note) {

    NoeudMouvmtNote *p = calloc(1, sizeof(NoeudMouvmtNote));
    p->depart = depart;
    p->arrivee = arrivee;
    p->note = note;
    p->suivant = teteliste;
    teteliste = p;

    return p;

}

NoeudMouvmtNote* ajoutFinMouvmtNote(NoeudMouvmtNote *teteliste, Point depart, Point arrivee, int note) {

    NoeudMouvmtNote *nouveau = calloc(1, sizeof(NoeudMouvmtNote));
    nouveau->depart = depart;
    nouveau->arrivee = arrivee;
    nouveau->note = note;
    nouveau->suivant = NULL;

    if (teteliste == NULL)
        return nouveau;
    else {

        NoeudMouvmtNote *tmp = teteliste;

        while (tmp->suivant != NULL)
            tmp = tmp->suivant;

        tmp->suivant = nouveau;
    }

    return teteliste;

}

NoeudMouvmtNote* supprTeteMouvmtNote(NoeudMouvmtNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else {
        NoeudMouvmtNote* tmp = teteliste->suivant;
        free(teteliste);
        return tmp;
    }
}

NoeudMouvmtNote* supprFinMouvmtNote(NoeudMouvmtNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else if (teteliste->suivant == NULL) {
        free(teteliste);
        return NULL;
    }

    NoeudMouvmtNote* tmp = teteliste;

    while (tmp->suivant->suivant != NULL)
        tmp = tmp->suivant;

    free(tmp->suivant);

    tmp->suivant = NULL;

    return teteliste;
}

void libereListeMouvmtNote(NoeudMouvmtNote* teteliste) {

    if (teteliste != NULL) {

        teteliste = supprTeteMouvmtNote(teteliste);
    }

}



