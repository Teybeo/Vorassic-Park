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

    coups = listeCoups(coups, depart->position, plateauTemp, taille, mode);

    while (coups != NULL) {

        textcolor(LIGHTRED);
        printf("\nR ");
        textcolor(LIGHTGRAY);

        printf("Coup %d, %d prof 0", coups->pos.x, coups->pos.y);

        effectueCoup(plateauTemp, depart->position, coups->pos);

        affichePlateauDebug(plateauTemp, taille, 0);

        coupsNotes = ajoutFinCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, 1, MIN));

        effaceCoup(plateauTemp, depart->position, coups->pos);

        printf("\nCe coup valait = %d\n", coupsNotes->note);

        coups = coups->suivant;
    }

    if (coupsNotes != NULL) {
        coup = meilleurCoup(coupsNotes);
        if (verifieCoup(plateau, mode, depart->position, coup) == 0) {
            effectueCoup(plateau, depart->position, coup);
            depart->position = coup;
        }
    } else {
        printf("\nLe bot est bloque, il passe son tour");

        depart->blocage = 1;
    }

        getchar();

}



int MinMax(char **plateau, int taille, int mode, int prof, int etage) {

    Noeud *coups = NULL;
    NoeudNote *notes = NULL;

    int finPartie, i;

    Point depart = trouveJoueur(plateau, taille, etage);

    finPartie = blocageJoueur(plateau, taille, mode);

    if (prof == PROF_MAX || finPartie == 1 || finPartie == -1) {

        return evaluation(plateau, taille, mode);
    }

    else {

        printf("\n");
        coups = listeCoups(coups, depart, plateau, taille, mode);

        while (coups != NULL) {

            effectueCoup(plateau, depart, coups->pos);

            for (i=0;i<prof;i++)
                printf("        ");
            if (etage == MAX) {
                textcolor(LIGHTRED);
                printf("%c ", 'R');
            }else {
                textcolor(LIGHTCYAN);
                printf("%C ", 'C');
            }
            textcolor(LIGHTGRAY);
            afficheDirection(depart, coups->pos);
            //printf("Coup %d, %d prof %d", coups->pos.x, coups->pos.y, prof);
            printf(" prof %d", coups->pos.x, coups->pos.y, prof);

            affichePlateauDebug(plateau, taille, prof);

            notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, prof+1, !etage));

            effaceCoup(plateau, depart, coups->pos);

            for (i=0;i<prof;i++)
                printf("        ");
            printf("Ce coup valait = %d\n", notes->note);
            printf("\n");
            coups = supprTete(coups);

        }

    }

    if (etage == MAX)
        return maxNote(notes);
    else
        return minNote(notes);
}

void effaceCoup(char **plateau, Point depart, Point arrivee) {


    plateau[depart.y][depart.x] = plateau[arrivee.y][arrivee.x];
    plateau[arrivee.y][arrivee.x] = 0;


}

void effectueCoup(char **plateau, Point depart, Point arrivee) {

    plateau[arrivee.y][arrivee.x] = plateau[depart.y][depart.x];
    plateau[depart.y][depart.x] += 32;
}

int evaluation(char **plateau, int taille, int mode) {

    int i, j;
    int scoreBot = 0, scoreAdverse = 0;

    Point bot, adversaire;
    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    bot = trouveJoueur(plateau, taille, MAX);
    adversaire = trouveJoueur(plateau, taille, MIN);

    coupsBot = listeCoups(coupsBot, bot, plateau, taille, mode);
    coupsAdversaire = listeCoups(coupsAdversaire, adversaire, plateau, taille, mode);



    for (i=0;i<taille;i++)

        for (j=0;j<taille;j++) {

            if (plateau[i][j] == 'R' || plateau[i][j] == 'r')
                scoreBot++;

            else if (plateau[i][j] == 'C' || plateau[i][j] == 'c')
                scoreAdverse++;
        }

    if (coupsBot == NULL && coupsAdversaire == NULL) {
        if (scoreBot > scoreAdverse)
            return EVAL_MAX;
        else if (scoreBot < scoreAdverse)
            return EVAL_MIN;
        else
            return 0;
    }
    else if (coupsBot == NULL)
        scoreBot += EVAL_MIN;

    if (coupsAdversaire == NULL)
        scoreAdverse += EVAL_MIN;

    return scoreBot - scoreAdverse;
}



int blocageJoueur(char **plateau, int taille, int mode) {

    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    coupsBot = listeCoups(coupsBot, trouveJoueur(plateau, taille, MIN), plateau, taille, mode);
    coupsAdversaire = listeCoups(coupsAdversaire, trouveJoueur(plateau, taille, MAX), plateau, taille, mode);


    if (coupsAdversaire == NULL) {

        // Si l'adversaire ne peut plus joueur et qu'il n'y a jamais une différence de plus d'1 point, logiquement le bot gagne
        return 1;

    } else if (coupsBot == NULL) {

        // Idem pour le bot, l'adversaire gagne
        return -1;

    }

 else

    return 0; // Il reste des pions a placer


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
        printf("ERREUR joueur %c non trouve\n", car);
        affichePlateau(plateau, taille);
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
    if (liste == NULL)
        printf("Probleme max/minNote sur liste nulle");
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
    if (liste == NULL)
        printf("Probleme max/minNote sur liste nulle");
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


void affichePlateauDebug(char **plateau, int taille, int prof) {

    int i, j, k;
    printf("\n");
    for (i=0;i<prof;i++)
        printf("        ");
    printf("   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n");

    for (i=0;i<prof;i++)
        printf("        ");

    for (i=0 ; i < taille+1 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        for (k=0;k<prof;k++)
            printf("        ");

        for (j=0 ; j < taille ; j++)
        {
            if (j == 0)
                printf("%2d |", i);

            if ((plateau[i][j] == 'C' || plateau[i][j] == 'R') || plateau[i][j] == 'c' || plateau[i][j] == 'r' ||
            plateau[i][j] == 'V' || plateau[i][j] == 'B' || plateau[i][j] == 'v' || plateau[i][j] == 'b') {

                if (plateau[i][j] == 'c')
                    textcolor(CYAN);
                if (plateau[i][j] == 'C')
                    textcolor(LIGHTCYAN);
                if (plateau[i][j] == 'r')
                    textcolor(RED);
                if (plateau[i][j] == 'R')
                    textcolor(LIGHTRED);
                if (plateau[i][j] == 'v')
                    textcolor(GREEN);
                if (plateau[i][j] == 'V')
                    textcolor(LIGHTGREEN);
                if (plateau[i][j] == 'b')
                    textcolor(BLUE);
                if (plateau[i][j] == 'B')
                    textcolor(LIGHTBLUE);

                printf("%2c ", plateau[i][j]);
                textcolor(LIGHTGRAY);
            }
            else
                printf("%2d ", plateau[i][j]);
                //printf("   ");

        }

        printf("\n");
    }
            //printf("\n");

}
