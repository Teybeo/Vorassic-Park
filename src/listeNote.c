#include "header/listeNote.h"

void affichageListeNote(NoeudNote *liste) {

    NoeudNote *temp = liste;

    while (temp != NULL) {

        printf(" %d |", temp->note);
        temp = temp->suivant;
    }

}

NoeudNote* ajoutTeteNote(NoeudNote *teteliste, int note) {

    NoeudNote *p = malloc(sizeof(NoeudNote));
    p->note = note;
    p->suivant = teteliste;
    teteliste = p;

    return p;

}

NoeudNote* ajoutFinNote(NoeudNote *teteliste, int note) {

    NoeudNote *nouveau = malloc(sizeof(NoeudNote));
    nouveau->note = note;
    nouveau->suivant = NULL;

    if (teteliste == NULL)
        return nouveau;
    else {

        NoeudNote *tmp = teteliste;

        while (tmp->suivant != NULL)
            tmp = tmp->suivant;

        tmp->suivant = nouveau;
    }

    return teteliste;

}

NoeudNote* supprTeteNote(NoeudNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else {
        NoeudNote* tmp = teteliste->suivant;
        free(teteliste);
        return tmp;
    }
}

NoeudNote* supprFinNote(NoeudNote* teteliste) {

    if (teteliste == NULL)
        return NULL;
    else if (teteliste->suivant == NULL) {
        free(teteliste);
        return NULL;
    }

    NoeudNote* tmp = teteliste;

    while (tmp->suivant->suivant != NULL)
        tmp = tmp->suivant;

    free(tmp->suivant);

    tmp->suivant = NULL;

    return teteliste;
}

void libereListeNote(NoeudNote* teteliste) {

    if (teteliste != NULL) {

        teteliste = supprTeteNote(teteliste);
    }

}

