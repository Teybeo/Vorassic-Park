#include "header/bot.h"

Point botCoup(char **plateau, int taille, int mode, Joueur *depart) {

    NoeudMouvmt *coups = NULL;
    NoeudMouvmtNote *coupsNotes = NULL;

    int prof = 3;

    coups = listeMouvmt(coups, depart->position, taille, mode);

    while (coups != NULL) {

        effectueCoup(plateau, coups->depart, coups->arrivee);

        coupsNotes = ajoutFinMouvmtNote(coupsNotes, coups->depart, coups->arrivee, MinMax(plateau, taille, prof, MIN));

        effectueCoup(plateau, coups->arrivee, coups->depart);

        coups = coups->suivant;
    }

}

void effectueCoup(char **plateau, Point depart, Point arrivee) {

    plateau[arrivee.x][arrivee.y] =  plateau[depart.y][depart.x];
    plateau[depart.y][depart.x] -= 32;
}


int MinMax(char **plateau, int taille, int prof, int etage) {

    NoeudMouvmt *coups = NULL;
    NoeudNote *notes = NULL;

    int finPartie;

    Point depart = trouveJoueur(plateau, taille, etage);

    finPartie = victoireDefaite(plateau, taille, etage);

    if (prof == 0 || finPartie == 1 || finPartie == -1)

        return evaluation(plateau, taille, etage);

    else {

        coups = listeMouvmt(coups, depart, taille, etage);

        while (coups != NULL) {

            effectueCoup(plateau, coups->depart, coups->arrivee);

            notes = ajoutTeteNote(notes, MinMax(plateau, taille, prof-1, -etage));

            effectueCoup(plateau, coups->arrivee, coups->depart);

            coups = supprTeteMouvmt(coups);

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

NoeudMouvmt* listeMouvmt(NoeudMouvmt* teteliste, Point depart, int taille, int mode) {

    Noeud *cases = NULL, *tmp = NULL;
    Point arrivee;
    cases = listeCases(taille, mode, depart);

    tmp = cases;
    while (tmp != NULL) {
        arrivee.x = tmp->x;
        arrivee.y = tmp->y;
        teteliste = ajoutFinMouvmt(teteliste, depart, arrivee);
        tmp = tmp->suivant;
    }

    return teteliste;
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
