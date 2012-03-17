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

        affichePlateauDebug(plateauTemp, taille, 0, depart->position, coups->pos);

        coupsNotes = ajoutTeteCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, 1, MIN));

        effaceCoup(plateauTemp, depart->position, coups->pos);

        printf("\nCe coup valait = %d\n", coupsNotes->note);

        coups = coups->suivant;
    }

    if (coupsNotes != NULL) {
        coup = meilleurCoup(coupsNotes);
        if (verifieCoup(plateau, mode, depart->position, coup) == 0) {
            appliqueCoup(plateau, depart, coup);
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
            printf(" prof %d", prof);

            affichePlateauDebug(plateau, taille, prof, depart, coups->pos);

            notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, prof+1, !etage));

            effaceCoup(plateau, depart, coups->pos);

            for (i=0;i<prof;i++)
                printf("        ");
            printf("Ce coup valait = %d\n\n", notes->note);

            coups = supprTete(coups);

        }

    }

    if (etage == MAX)
        return maxNote(notes);
    else
        return minNote(notes);
}

void effectueCoup(char **plateau, Point depart, Point arrivee) {

    plateau[arrivee.y][arrivee.x] = plateau[depart.y][depart.x];
    plateau[depart.y][depart.x] += 32;
}

void effaceCoup(char **plateau, Point depart, Point arrivee) {

    plateau[depart.y][depart.x] = plateau[arrivee.y][arrivee.x];
    plateau[arrivee.y][arrivee.x] = 0;
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

    for (i=0;i<taille;i++) {

        for (j=0;j<taille;j++) {

            if (plateau[i][j] == 'R' || plateau[i][j] == 'r')
                scoreBot++;

            else if (plateau[i][j] == 'C' || plateau[i][j] == 'c')
                scoreAdverse++;
        }
    }

    // Si la partie est finie
    if (coupsBot == NULL && coupsAdversaire == NULL) {
        if (scoreBot > scoreAdverse)
            return EVAL_MAX;
        else if (scoreBot < scoreAdverse)
            return EVAL_MIN;
        else
            return 0;
    }
    /* Comme pour l'instant l'arbre s'arrete des qu'un joueur est bloqué
    On compense avec une note car le bot pourrait etre bloqué en ayant le meme nb
    de cases que l'adversaire, ce qui renverrait 0
    Si l'arbre continuait d'etre exploré, on s'apercevrait que l'adversaire prendra plus
    de cases que le bot ce qui n'est pas bon pour le bot */
    else if (coupsBot == NULL)
        scoreBot -= 200;

    if (coupsAdversaire == NULL)
        scoreAdverse -= 200;

    return scoreBot - scoreAdverse;
}



int blocageJoueur(char **plateau, int taille, int mode) {

    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    coupsBot = listeCoups(coupsBot, trouveJoueur(plateau, taille, MIN), plateau, taille, mode);
    coupsAdversaire = listeCoups(coupsAdversaire, trouveJoueur(plateau, taille, MAX), plateau, taille, mode);

    if (coupsAdversaire == NULL)

        // Si l'adversaire ne peut plus joueur et qu'il n'y a jamais une différence de plus d'1 point, logiquement le bot gagne
        return 1;

    else if (coupsBot == NULL)

        // Idem pour le bot, l'adversaire gagne
        return -1;

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

        if (tmp->note > maxi.note) {
            maxi.pos = tmp->pos;
            maxi.note = tmp->note;

        }

        tmp = supprTeteCoupNote(tmp);
    }

    return maxi.pos;

}


void affichePlateauDebug(char **plateau, int taille, int prof, Point depart, Point arrivee) {

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


            if (plateau[i][j] == 'C' || plateau[i][j] == 'R' || plateau[i][j] == 'c' || plateau[i][j] == 'r') {

                if (i == depart.y && j == depart.x) {


                    if (plateau[i][j] == 'r')
                        textcolor(RED);
                    if (plateau[i][j] == 'c')
                        textcolor(CYAN);

                    printf("%2c ", 254);

                } else if (i == arrivee.y && j == arrivee.x) {

                    if (plateau[i][j] == 'c' || plateau[i][j] == 'C')
                        textcolor(CYAN);

                    else if (plateau[i][j] == 'r' || plateau[i][j] == 'R')
                        textcolor(RED);

                    printf("%2c ", 'x');

                } else {

                    if (plateau[i][j] == 'C')
                        textcolor(CYAN);
                    if (plateau[i][j] == 'c')
                        textcolor(LIGHTCYAN);
                    if (plateau[i][j] == 'R')
                        textcolor(RED);
                    if (plateau[i][j] == 'r')
                        textcolor(LIGHTRED);

                    printf("%2c ", 254);
                }

                textcolor(LIGHTGRAY);
                textbackground(BLACK);

            }
            else
                //printf("%2d ", plateau[i][j]);
                printf("   ");

        }

        printf("\n");
    }

}
