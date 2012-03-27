#include "header/bot.h"

Point botCoup(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i, j;
    char valeurTmp;
    Point coup, posTmp;
    Noeud *coups = NULL;
    NoeudCoupNote *coupsNotes = NULL;

    char **plateauTemp = malloc(sizeof(char*) * taille);
    for (i=0;i<taille;i++) {
        plateauTemp[i] = malloc(sizeof(char) * taille);
        for (j=0;j<taille;j++)
           plateauTemp[i][j] = plateau[i][j];
    }

    posTmp = bot->position;
    coups = listeCoups(coups, bot->position, plateauTemp, taille, mode);

    while (coups != NULL) {

        textcolor(LIGHTRED);
        printf("\nR ");
        textcolor(LIGHTGRAY);
        printf("Coup %d, %d prof 0", coups->pos.x, coups->pos.y);

        effectueCoup(plateauTemp, bot, coups->pos, &valeurTmp, &posTmp);

        affichePlateauDebug(plateauTemp, taille, 0, bot->position, coups->pos);

        coupsNotes = ajoutTeteCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, bot, adversaire, 1, MIN));

        annuleCoup(plateauTemp, bot, coups->pos, &valeurTmp, &posTmp);

        printf("\nCe coup valait = %d\n", coupsNotes->note);

        coups = supprTete(coups);
    }

    if (coupsNotes != NULL) {

        coup = meilleurCoup(coupsNotes);
        if (verifieCoup(plateau, mode, bot->position, coup) == 0) {
            appliqueCoup(plateau, mode, bot, coup);
            bot->position = coup;
        }
    } else {
        printf("\nLe bot est bloque, il passe son tour");

        bot->blocage = 1;
    }

        for (i=0;i<taille;i++)
            free(plateauTemp[i]);
        free(plateauTemp);

        getchar();

}



int MinMax(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire, int prof, int etage) {

    Noeud *coups = NULL;
    NoeudNote *notes = NULL;
    Joueur *J_actuel = NULL;
    Point posDepart;
    int finPartie;
    char valeurTmp;

    if (etage == MIN)
        J_actuel = adversaire;
    else
        J_actuel = bot;
    posDepart = J_actuel->position;

    finPartie = finPartieBot(plateau, taille, mode);

    if (finPartie == 1 || (prof == PROF_MAX &&  blocageJoueur(plateau, taille, mode) == -1))  { // Si la partie est finie ou (horizon atteint et pas d'exploration solo)

        return bot->score - adversaire->score;
    }
    else {
#ifdef DEBUG
        printf("\n");
#endif
        coups = listeCoups(coups, J_actuel->position, plateau, taille, mode);

        while (coups != NULL) {

            effectueCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posDepart);
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
            afficheDirection(J_actuel->position, coups->pos);
            printf(" prof %d", prof);

            affichePlateauDebug(plateau, taille, prof, J_actuel->position, coups->pos);
#endif
            //Si l'autre joueur était ou s'est fait bloquer, on continue solo
            if (blocageJoueur(plateau, taille, mode) == !etage )
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, etage));
            else
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, !etage));

            annuleCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posDepart);
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

/* Stocke la valeur de la case prise
   Stocke la position de la tete
   Place la tete sur la case prise
   Passe la valeur de l'ancienne tete en minuscule
   Met a jour la position du joueur, désormais égale à celle de la case prise
   Met a jour le score du joueur en fonction de la case prise*/
void effectueCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp) {

    *valeurTmp = plateau[arrivee.y][arrivee.x];
    *posTmp = joueur->position;

    plateau[arrivee.y][arrivee.x] = plateau[joueur->position.y][joueur->position.x];
    plateau[joueur->position.y][joueur->position.x] += 32;

    joueur->position = arrivee;
    joueur->score += *valeurTmp;
}
/*  Restaure les coordonnées de la tete
    Replace la tete sur l'ancienne tete
    Restaure la valeur de la case prise
    Restaure le score du joueur avant la prise de la case */
void annuleCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp) {

    joueur->position = *posTmp;

    plateau[joueur->position.y][joueur->position.x] = plateau[arrivee.y][arrivee.x];
    plateau[arrivee.y][arrivee.x] = *valeurTmp;

    joueur->score -= *valeurTmp;
}

int evaluation(char **plateau, int taille, int mode) {

    int i, j;
    int scoreBot = 0, scoreAdverse = 0;

    for (i=0;i<taille;i++) {

        for (j=0;j<taille;j++) {

            if (plateau[i][j] == 'R' || plateau[i][j] == 'r')
                scoreBot++;

            else if (plateau[i][j] == 'C' || plateau[i][j] == 'c')
                scoreAdverse++;
        }
    }

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
                printf("%2d ", plateau[i][j]);
                //printf("   ");

        }

        printf("\n");
    }

}
