#include "header/bot.h"

#define CASEVIDE(x)  \
    x != 'C' && x != 'R' && \
    x != 'c' && x != 'r' && \
    x != 'V' && x != 'v' && \
    x != 'B' && x != 'b'


void botCoup(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i, j;
    float debut, temps;
    char valeurTmp;
    Point coup, posTmp;
    Noeud *coups = NULL;
    NoeudCoupNote *coupsNotes = NULL;

    debut = clock();

    char **plateauTemp = malloc(sizeof(char*) * taille);
    for (i=0;i<taille;i++) {
        plateauTemp[i] = malloc(sizeof(char) * taille);
        for (j=0;j<taille;j++)
           plateauTemp[i][j] = plateau[i][j];
    }

    posTmp = bot->position;
    coups = listeCasesBot(coups, bot->position, plateauTemp, taille, mode);

    while (coups != NULL) {

        textcolor(LIGHTRED);
        printf("\nR ");
        textcolor(LIGHTGRAY);
        printf("Coup %d, %d prof 0", coups->pos.x, coups->pos.y);

        effectueCoup(plateauTemp, bot, coups->pos, &valeurTmp, &posTmp);

        affichePlateauDebug(plateauTemp, taille, 0, posTmp, coups->pos);

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

        temps = (clock() - debut)/CLOCKS_PER_SEC;
        printf("Temps de calcul: %f\n", temps);

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
    int blocage;
    char valeurTmp;

    if (etage == MIN)
        J_actuel = adversaire;
    else
        J_actuel = bot;
    posDepart = J_actuel->position;

    blocage = blocageJoueur(plateau, taille, mode, bot, adversaire);

    if (blocage == 2 || (prof == PROF_MAX && blocage == -1))  { // Si les 2 sont bloqués ou (horizon atteint et aucun joueur bloqué)

        return bot->score - adversaire->score;
    }
    else {

        #ifdef DEBUG
            printf("\n");
        #endif

        coups = listeCasesBot(coups, J_actuel->position, plateau, taille, mode);

        while (coups != NULL) {

            effectueCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posDepart);

            #ifdef DEBUG
                int i;
                for (i=0;i<prof;i++)
                    printf("        ");
                if (etage == MAX) {
                    textcolor(LIGHTRED);printf("%c ", 'R');
                }else {
                    textcolor(LIGHTCYAN);printf("%C ", 'C');
                }
                textcolor(LIGHTGRAY);afficheDirection(posDepart, coups->pos);
                printf(" prof %d", prof);
                affichePlateauDebug(plateau, taille, prof, posDepart, coups->pos);
            #endif

            //Si l'autre joueur était ou s'est fait bloquer, on continue solo
            if (blocageJoueur(plateau, taille, mode, bot, adversaire) == !etage )
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, etage));
            else
                notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, !etage));

            annuleCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posDepart);

            coups = supprTete(coups);

            #ifdef DEBUG
                for (i=0;i<prof;i++)
                    printf("        ");
                printf("Ce coup valait = %d\n\n", notes->note);
            #endif

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

int blocageJoueur(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int retour;
    Noeud *coupsBot = NULL, *coupsAdversaire = NULL;

    coupsBot = listeCasesBot(coupsBot, bot->position, plateau, taille, mode);
    coupsAdversaire = listeCasesBot(coupsAdversaire, adversaire->position, plateau, taille, mode);

    if (coupsAdversaire == NULL && coupsBot == NULL)

       retour = 2; // Les 2 joueurs sont bloqués

    else if (coupsAdversaire == NULL)


        retour = MIN; // Si l'adversaire ne peut plus joueur

    else if (coupsBot == NULL)


        retour = MAX; // Idem pour le bot, l'adversaire gagne

    else

        retour = -1; // Il reste des pions a placer

    libereListe(coupsBot);
    libereListe(coupsAdversaire);

    return retour;

}

Noeud* listeCasesBot(Noeud *liste,  Point depart, char **plateau, int taille, int mode) {

    if (depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x]))  // Haut

        liste = ajoutTete(liste, (Point){depart.x, depart.y-1});


    if (depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x]))  // Bas

        liste = ajoutTete(liste, (Point){depart.x, depart.y+1});


    if (depart.x > 0 && CASEVIDE(plateau[depart.y][depart.x-1]))// Gauche
    {

        liste = ajoutTete(liste, (Point){depart.x-1, depart.y});


        if (mode && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x-1]))  // Gauche / Haut

            liste = ajoutTete(liste, (Point){depart.x-1, depart.y-1});


        if (mode && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x-1]))  // Gauche / Bas

            liste = ajoutTete(liste, (Point){depart.x-1, depart.y+1});

    }

    if (depart.x < taille-1 && CASEVIDE(plateau[depart.y][depart.x+1])) // Droite
    {

        liste = ajoutTete(liste, (Point){depart.x+1, depart.y});


        if (mode && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x+1])) // Droite / Haut

            liste = ajoutTete(liste, (Point){depart.x+1, depart.y-1});


        if (mode && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x+1])) // Droite / Bas

            liste = ajoutTete(liste, (Point){depart.x+1, depart.y+1});

    }

    return liste;
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

    NoeudCoupNote maxi = {{0}};
    maxi.note = EVAL_MIN;

    while (liste != NULL) {

        if (liste->note >= maxi.note) {

            maxi.pos = liste->pos;
            maxi.note = liste->note;
        }

        liste = supprTeteCoupNote(liste);
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
                    else if (plateau[i][j] == 'c')
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
