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

        coups = supprTete(coups);
    }

    if (coupsNotes != NULL) {

        coup = meilleurCoup(coupsNotes);
        if (verifieCoup(plateau, mode, depart->position, coup) == 0) {
            appliqueCoup(plateau, mode, depart, coup);
            depart->position = coup;
        }
    } else {
        printf("\nLe bot est bloque, il passe son tour");

        depart->blocage = 1;
    }

        for (i=0;i<taille;i++)
            free(plateauTemp[i]);
        free(plateauTemp);

        getchar();

}



int MinMax(char **plateau, int taille, int mode, int prof, int etage) {

    Noeud *coups = NULL;
    NoeudNote *notes = NULL;
    int finPartie, i, blocage;

    Point depart = trouveJoueur(plateau, taille, etage);

    blocage = blocageJoueur(plateau, taille, mode);
    finPartie = finPartieBot(plateau, taille, mode);

    if (finPartie == 1 || (prof == PROF_MAX && blocage == -1))  { // Si la partie est finie ou (horizon atteint et pas d'exploration solo

        return evaluation(plateau, taille, mode);
    }
    else {
#ifdef DEBUG
        printf("\n");
#endif
        coups = listeCoups(coups, depart, plateau, taille, mode);

        while (coups != NULL) {

            effectueCoup(plateau, depart, coups->pos);
#ifdef DEBUG
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
#endif
            //Si l'autre joueur était ou s'est fait bloquer, on continue solo
            blocage = blocageJoueur(plateau, taille, mode);
            if (blocage == !etage )
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, prof+1, etage));
            else
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, prof+1, !etage));

            effaceCoup(plateau, depart, coups->pos);
#ifdef DEBUG
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Ce coup valait = %d\n\n", notes->note);
#endif
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
   /* if (coupsBot == NULL && coupsAdversaire == NULL) {
        if (scoreBot > scoreAdverse)
            return EVAL_MAX;
        else if (scoreBot < scoreAdverse)
            return EVAL_MIN;
        else
            return 0;
    }*/
    /* Comme pour l'instant l'arbre s'arrete des qu'un joueur est bloqué
    On compense avec une note car le bot pourrait etre bloqué en ayant le meme nb
    de cases que l'adversaire, ce qui renverrait 0
    Le manque de précision empeche la distinction entre blocages a court et long termes*/
    /*if (coupsBot == NULL)
        scoreBot -= 200;

    if (coupsAdversaire == NULL)
        scoreAdverse -= 200;*/

    libereListe(coupsBot);
    libereListe(coupsAdversaire);

    return scoreBot - scoreAdverse;
}

int finPartieBot(char **plateau, int taille, int mode) {

    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    coupsBot = listeCoups(coupsBot, trouveJoueur(plateau, taille, MIN), plateau, taille, mode);
    coupsAdversaire = listeCoups(coupsAdversaire, trouveJoueur(plateau, taille, MAX), plateau, taille, mode);
    int retour;

    if (coupsAdversaire == NULL && coupsBot == NULL)

       retour = 1;

    else

        retour = 0;

    libereListe(coupsBot);
    libereListe(coupsAdversaire);

    return retour;

}

int blocageJoueur(char **plateau, int taille, int mode) {

    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    coupsBot = listeCoups(coupsBot, trouveJoueur(plateau, taille, MAX), plateau, taille, mode);
    coupsAdversaire = listeCoups(coupsAdversaire, trouveJoueur(plateau, taille, MIN), plateau, taille, mode);
    int retour;

    if (coupsAdversaire == NULL)

        // Si l'adversaire ne peut plus joueur et qu'il n'y a jamais une différence de plus d'1 point, logiquement le bot gagne
        retour = MIN;

    else if (coupsBot == NULL)

        // Idem pour le bot, l'adversaire gagne
        retour = MAX;

    else

        retour = -1; // Il reste des pions a placer

    libereListe(coupsBot);
    libereListe(coupsAdversaire);

    return retour;

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
}

/* Crée une liste des coups jouables à partir d'un point
    Utilise une liste des cases adjacentes et la fonction verifieCoup */
Noeud* listeCoups(Noeud* coups, Point depart, char** plateau, int taille, int mode) {

    Noeud *cases = NULL;

    cases = listeCases(taille, mode, depart);

    while (cases != NULL) {

        if (verifieCoup(plateau, mode, depart, cases->pos) == 0)
            coups = ajoutFin(coups, cases->pos);
        cases = supprTete(cases);

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
