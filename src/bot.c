#include "header/bot.h"

Point botCoup(char **plateau, int taille, int mode, Joueur *depart) {

    int i, j;
    Point coup;
    Noeud *coups = NULL;
    NoeudCoupNote *coupsNotes = NULL;
    char **plateauTemp = malloc(sizeof(char*) * taille);
    for (i=0;i<taille;i++) {
        plateauTemp[i] = malloc(sizeof(char) * taille);
        for (j=0;j<taille;j++)
           plateauTemp[i][j] = plateau[i][j];
    }

    int prof = 1;

    coups = listeCoups(coups, depart->position, plateauTemp, taille, mode);

    while (coups != NULL) {

        effectueCoup(plateauTemp, depart->position, coups->pos);

        coupsNotes = ajoutFinCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, prof, MIN));

        effectueCoup(plateauTemp, coups->pos, depart->position);

        coups = coups->suivant;
    }

    coup = meilleurCoup(coupsNotes);
    if (verifieCoup(plateau, mode, depart->position, coup) == 0) {
        effectueCoup(plateau, depart->position, coup);
        depart->position = coup;
    }

}

void effectueCoup(char **plateau, Point depart, Point arrivee) {

    plateau[arrivee.y][arrivee.x] = plateau[depart.y][depart.x];
    plateau[depart.y][depart.x] += 32;
}


int MinMax(char **plateau, int taille, int mode, int prof, int etage) {

    Noeud *coups = NULL;
    NoeudNote *notes = NULL;

    int finPartie;

    Point depart = trouveJoueur(plateau, taille, etage);

    finPartie = victoireDefaite(plateau, taille, etage);

    if (prof == 0 || finPartie == 1 || finPartie == -1)

        return evaluation(plateau, taille, etage);

    else {

        coups = listeCoups(coups, depart, plateau, taille, mode);

        while (coups != NULL) {

            effectueCoup(plateau, depart, coups->pos);

            notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, prof-1, !etage));

            effectueCoup(plateau, coups->pos, depart);

            coups = supprTete(coups);

        }

    }

    if (etage == MAX)
        return maxNote(notes);
    else
        return minNote(notes);
}

int evaluation(char **plateau, int taille, int etage) {

    int i, j;
    int scoreBot, scoreAdverse;

    for (i=0;i<taille;i++)

        for (j=0;j<taille;j++) {

            if (plateau[i][j] == 'R' || plateau[i][j] == 'r')
                scoreBot++;

            else if (plateau[i][j] == 'C' || plateau[i][j] == 'c')
                scoreAdverse++;
        }

    return scoreAdverse - scoreBot;
}

int victoireDefaite(char **plateau, int taille, int etage) {

    int i, j, carBot, carAdverse, caseLibre;
    int scoreBot, scoreAdverse;

    scoreBot = scoreAdverse = caseLibre = 0;
    carBot = 'R';
    carAdverse = 'C';

    for (i=0;i<taille;i++)

        for (j=0;j<taille;j++)

            if (plateau[i][j] != 'R' && plateau[i][j] != 'r' && plateau[i][j] != 'C' && plateau[i][j] != 'c')
                caseLibre = 1;

    if (!caseLibre) {

        for (i=0;i<taille;i++)

            for (j=0;j<taille;j++) {

                if (plateau[i][j] == carBot || plateau[i][j] == carBot - 32)
                    scoreBot++;

                else if (plateau[i][j] == carAdverse || plateau[i][j] == carAdverse- 32)
                    scoreAdverse++;

            }

        if (scoreBot > scoreAdverse)
            return 1;
        else
            return -1;

    } else

        return 0;


}

Point trouveJoueur(char** plateau, int taille, int etage) {

    int i, j, car;
    Point pos;
    if (etage == MAX)
        car = 'R';
    else
        car = 'C';

    for (i=0;i<taille;i++)

        for (j=0;j<taille;j++)

            if (plateau[i][j] == car) {
                pos.x = j;
                pos.y = i;

                return pos;

            }

}

/* Crée une liste des coups jouables à partir d'un point
    Utilise une liste des cases adjacentes et la fonction verifieCoup */
Noeud* listeCoups(Noeud* coups, Point depart, char** plateau, int taille, int mode) {

    Noeud *cases = NULL, *tmp = NULL;

    cases = listeCases(taille, mode, depart);

    tmp = cases;
    while (tmp != NULL) {

        if (verifieCoup(plateau, mode, depart, tmp->pos) == 0)
            coups = ajoutFin(coups, tmp->pos);
        tmp = tmp->suivant;

    }

    return coups;
}


int maxNote(NoeudNote *liste) {

    int maxi = EVAL_MIN;
    NoeudNote *tmp = liste;

    while (tmp != NULL) {

        if (tmp->note > maxi)
            maxi = tmp->note;

        tmp = supprTeteNote(tmp);
    }

    return maxi;

}

int minNote(NoeudNote *liste) {

    int mini = EVAL_MAX;
    NoeudNote *tmp = liste;

    while (tmp != NULL) {

        if (tmp->note < mini)
            mini = tmp->note;

        tmp = supprTeteNote(tmp);
    }

    return mini;

}

Point meilleurCoup(NoeudCoupNote *liste) {

    NoeudCoupNote maxi;
    maxi.note = EVAL_MIN;

    NoeudCoupNote *tmp = liste;

    while (tmp != NULL) {

        if (tmp->note >= maxi.note) {
            maxi.pos = tmp->pos;
            maxi.note = tmp->note;

        }

        tmp = supprTeteCoupNote(tmp);
    }

    return maxi.pos;

}

