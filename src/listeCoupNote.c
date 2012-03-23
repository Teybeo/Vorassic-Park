#include "header/listeCoupNote.h"

void affichageListeCoupNote(NoeudCoupNote *liste) {

    NoeudCoupNote *temp = liste;

    while (temp != NULL) {

        printf(" (%d, %d) => %d\n", temp->pos.x, temp->pos.y, temp->note);
        temp = temp->suivant;
    }

}

NoeudCoupNote* ajoutTeteCoupNote(NoeudCoupNote *teteliste, Point coup, int note) {

    NoeudCoupNote *p = calloc(1, sizeof(NoeudCoupNote));
    p->pos = coup;
    p->note = note;
    p->suivant = teteliste;
    teteliste = p;

    return p;

}

NoeudCoupNote* ajoutFinCoupNote(NoeudCoupNote *teteliste, Point coup, int note) {

    NoeudCoupNote *nouveau = calloc(1, sizeof(NoeudCoupNote));
    nouveau->pos = coup;
    nouveau->note = note;
    nouveau->suivant = NULL;

    if (teteliste == NULL)
        return nouveau;
    else {

        NoeudCoupNote *tmp = teteliste;

        while (tmp->suivant != NULL)
            tmp = tmp->suivant;

        tmp->suivant = nouveau;
    }

    return teteliste;

}

NoeudCoupNote* supprTeteCoupNote(NoeudCoupNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else {
        NoeudCoupNote* tmp = teteliste->suivant;
        free(teteliste);
        return tmp;
    }
}

NoeudCoupNote* supprFinCoupNote(NoeudCoupNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else if (teteliste->suivant == NULL) {
        free(teteliste);
        return NULL;
    }

    NoeudCoupNote* tmp = teteliste;

    while (tmp->suivant->suivant != NULL)
        tmp = tmp->suivant;

    free(tmp->suivant);

    tmp->suivant = NULL;

    return teteliste;
}

void libereListeCoupNote(NoeudCoupNote* teteliste) {

    while (teteliste != NULL) {

        teteliste = supprTeteCoupNote(teteliste);
    }

}



