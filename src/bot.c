#include "header/bot.h"

void botCoup(char **plateau, int taille, int mode, int profMax, Joueur *tabJoueur, int bot) {

    float debut, temps;
    char valeurCoup;
    int blocageTmpBot;
    int blocageTmpAdv;
    int i;
    char **plateauTemp = calloc(taille, sizeof(char*));
    for (i=0; i< taille ;i++ ) {
        plateauTemp[i] = malloc(taille * sizeof(char));
        memcpy(plateauTemp[i], plateau[i], taille * sizeof(char));
    }
    PointNote maxActuel;
    ElemPoint *coups = NULL;
    ElemPoint *pionTemp = NULL;
    PointNote coupNote = {{-1, -1}, 0};

    debut = clock();

    maxActuel.note = EVAL_MIN;
    if (mode == SERPENT)
        pionTemp = empiler(pionTemp, tabJoueur[bot].pion->pos);
    else if (mode == PIEUVRE)
        pionTemp = tabJoueur[bot].pion;

    do {

        coups = creerPileCoupsPossibles(coups, plateauTemp, taille, mode, pionTemp->pos);

        while (coups != NULL) {

            DEBUG_DEBUT_1(DEBUG);

            plateauTemp[coups->pos.y][coups->pos.x] = plateau[tabJoueur[bot].pion->pos.y][tabJoueur[bot].pion->pos.x];
            effectueCoup(plateau, mode, tabJoueur, bot, coups->pos, &blocageTmpBot, &blocageTmpAdv, &valeurCoup);

            chercheBlocage(plateau, taille, mode, &tabJoueur[bot]);
            chercheBlocage(plateau, taille, mode, &tabJoueur[!bot]);

            if (tabJoueur[!bot].blocage == VRAI) // Si le joueur adverse bloqué, on continue solo
                coupNote.note = AlphaBeta(plateau, taille, mode, 1, profMax, tabJoueur, bot, bot, MAX, maxActuel.note, EVAL_MAX);
            else
                coupNote.note = AlphaBeta(plateau, taille, mode, 1, profMax, tabJoueur, bot, !bot, MIN, maxActuel.note, EVAL_MAX);

            coupNote.pos = coups->pos;

            maxActuel = maxPointNote(maxActuel, coupNote);

            annuleCoup(plateau, mode, tabJoueur, bot, blocageTmpBot, blocageTmpAdv, coups->pos, valeurCoup, pionTemp->pos);

            DEBUG_FIN_1(DEBUG);

            coups = depiler(coups);

            }

        pionTemp = pionTemp->suivant;

    } while (pionTemp != NULL);

    if (coupNote.pos.x != -1 && coupNote.pos.y != -1)
        appliqueCoup(plateau, &tabJoueur[bot], maxActuel.pos, mode);
    else
    {
        printf("\nLe bot est bloque, il passe son tour\n");
        tabJoueur[bot].blocage = 1;
    }

    temps = (clock() - debut)/CLOCKS_PER_SEC;
    printf("Temps de calcul: %f\n", temps);
    //getchar();

}


int AlphaBeta(char **plateau, int taille, int mode, int prof, int profMax, Joueur *tabJoueur, int bot, int jActuel, int etage, int maxActuel, int minActuel) {

    ElemPoint *coups = NULL, *pionTemp = NULL;
    char valeurCoup;
    int blocageTmpBot;
    int blocageTmpAdv;
    char **plateauTemp = NULL;
    int note, i;

    chercheBlocage(plateau, taille, mode, &tabJoueur[0]);
    chercheBlocage(plateau, taille, mode, &tabJoueur[1]);

    if ((tabJoueur[jActuel].blocage && tabJoueur[!jActuel].blocage) || (prof == profMax && !tabJoueur[jActuel].blocage && !tabJoueur[!jActuel].blocage))  { // Si les 2 sont bloqués ou horizon atteint

        return tabJoueur[bot].score - tabJoueur[!bot].score;
    }
    else {

        DEBUG_SAUT(DEBUG);

        if (etage == MIN)
            minActuel = EVAL_MAX;
        else if (etage == MAX)
            maxActuel = EVAL_MIN;

        if (mode == SERPENT)
            pionTemp = empiler(pionTemp, tabJoueur[jActuel].pion->pos);
        else if (mode == PIEUVRE)
            pionTemp = tabJoueur[jActuel].pion;

        plateauTemp = malloc(taille * sizeof(char*));
        for (i=0; i< taille ;i++ ) {
            plateauTemp[i] = malloc(taille * sizeof(char));
            memcpy(plateauTemp[i], plateau[i], taille * sizeof(char));
        }

        do {

            coups = creerPileCoupsPossibles(coups, plateauTemp, taille, mode, pionTemp->pos);

            while (coups != NULL) {

                DEBUG_DEBUT(DEBUG);

                plateauTemp[coups->pos.y][coups->pos.x] = plateau[tabJoueur[jActuel].pion->pos.y][tabJoueur[jActuel].pion->pos.x];
                effectueCoup(plateau, mode, tabJoueur, jActuel, coups->pos, &blocageTmpBot, &blocageTmpAdv, &valeurCoup);

                chercheBlocage(plateau, taille, mode, &tabJoueur[0]);
                chercheBlocage(plateau, taille, mode, &tabJoueur[1]);

                // Si le joueur adverse est bloqué, on continue solo
                if (tabJoueur[!jActuel].blocage == VRAI)
                    note = AlphaBeta(plateau, taille, mode, prof+1, profMax, tabJoueur, bot, jActuel, etage, maxActuel, minActuel);
                else
                    note = AlphaBeta(plateau, taille, mode, prof+1, profMax, tabJoueur, bot, !jActuel, !etage, maxActuel, minActuel);

                annuleCoup(plateau, mode, tabJoueur, jActuel, blocageTmpBot, blocageTmpAdv, coups->pos, valeurCoup, pionTemp->pos);

                DEBUG_FIN(DEBUG);

                if (etage == MAX) {

                    maxActuel = max(maxActuel, note);
                    if (maxActuel > minActuel) { // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
                        liberePile(coups);
                        for (i=0; i< taille;i++ )
                            free(plateauTemp[i]);
                        free(plateauTemp);
                        if (mode == SERPENT)
                            free(pionTemp);
                        return maxActuel;
                    }
                } else if (etage == MIN) {

                    minActuel = min(minActuel, note);
                    if (minActuel < maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
                        liberePile(coups);
                        for (i=0; i< taille;i++ )
                            free(plateauTemp[i]);
                        free(plateauTemp);
                        if (mode == SERPENT)
                            free(pionTemp);
                        return minActuel;
                    }
                }

                coups = depiler(coups);

            }

            if (mode == SERPENT)
                pionTemp = depiler(pionTemp);
            else
                pionTemp = pionTemp->suivant;

        } while (pionTemp != NULL);

    }
    for (i=0; i< taille;i++ )
        free(plateauTemp[i]);
    free(plateauTemp);
    if (etage == MAX)
        return maxActuel;
    else
        return minActuel;

}

/*  Sauvegarde les états des joueurs et la valeur de la case future
    Puis met le code du joueur sur la case et met a jour les coordoonées et le score du joueur */

void effectueCoup(char **plateau, int mode, Joueur *tabJoueur, int jActuel, Point coup, int *blocageBotTmp, int *blocageAdvTmp, char *valeurCoup) {

    *blocageBotTmp = tabJoueur[MAX].blocage;
    *blocageAdvTmp = tabJoueur[MIN].blocage;
    *valeurCoup = plateau[coup.y][coup.x];

    plateau[coup.y][coup.x] = plateau[tabJoueur[jActuel].pion->pos.y][tabJoueur[jActuel].pion->pos.x];

    if (mode == SERPENT)
        tabJoueur[jActuel].pion->pos = coup;
    else
        tabJoueur[jActuel].pion = empiler(tabJoueur[jActuel].pion, coup);

    tabJoueur[jActuel].score += *valeurCoup;
}

/*  Remet la valeur originelle de la case et restaure les coordonnées, le score et les états des joueurs */

void annuleCoup(char **plateau, int mode, Joueur *tabJoueur, int jActuel, int blocageTmpBot, int blocageTmpAdv, Point coup, char valeurCoup, Point depart) {

    plateau[coup.y][coup.x] = valeurCoup;

    if (mode == SERPENT)
        tabJoueur[jActuel].pion->pos = depart;
    else
        tabJoueur[jActuel].pion = depiler(tabJoueur[jActuel].pion);

    tabJoueur[jActuel].score -= valeurCoup;
    tabJoueur[MAX].blocage = blocageTmpBot;
    tabJoueur[MIN].blocage = blocageTmpAdv;

}

int max(int a, int b) {

    return (a > b) ? a : b;

}

int min(int a, int b) {

    return (a < b) ? a : b;

}

inline PointNote maxPointNote(PointNote a, PointNote b) {

     return (a.note > b.note) ? a : b;
}

void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adv, Point actuel, Point arrivee, int id) {

    int i, j;
    int nb_espaces = 8;
    char *decalage = malloc(sizeof(char) * 1 + (nb_espaces * prof) );
    memset(decalage, ' ', sizeof(char) * 1 + (nb_espaces * prof) );
    decalage[nb_espaces*prof] = '\0';

    printf("\n");

    printf("%s", decalage);
    printf("   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n");

    printf("%s", decalage);

    for (i=0 ; i < taille+1 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        printf("%s", decalage);

        for (j=0 ; j < taille ; j++)
        {
            if (j == 0)
                printf("%2d |", i);


            if (CASEVIDE(plateau[i][j]) == FAUX) {

                if (mode == PIEUVRE && (i == actuel.y && j == actuel.x)) { // Position du pion qui s'apprete a jouer

                    couleurs(plateau[actuel.y][actuel.x]);
                    printf("%2c ", 254);

                } else if ((i == bot.y && j == bot.x) || (i == adv.y && j == adv.x)) { // Position des tetes des joueurs

                    couleurs(plateau[i][j]);
                    printf("%2c ", 254);

                } else { // Anciennes positions

                    couleurs(plateau[i][j]);
                    lowvideo();
                    printf("%2c ", 254);
                }

            } else if (i == arrivee.y && j == arrivee.x) { // Position d'arrivée

                couleurs(id);
                printf("%2c ", 'x');

            } else
                printf("%2d ", plateau[i][j]);
                //printf("   ");

            textcolor(LIGHTGRAY);
        }

        printf("\n");
    }

    free(decalage);
}

void debugDebut(char **plateau, int taille, int mode, int prof, int profMax, Point bot, Point adv, Point depart, Point arrive, int id) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    couleurs(id);
    printf("%C ", 254);
    textcolor(LIGHTGRAY);

    afficheDirection(depart, arrive);
    printf(" prof %d/%d", prof, profMax-1);

    affichePlateauDebug(plateau, taille, mode, prof, bot, adv, depart, arrive, id);

}

void debugFin(int prof, int note, int profMax, int maxActuel, int minActuel, int etage, ElemPoint *coup) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    printf("Ce coup valait = %d\n", note);
    if (coup->suivant != NULL && prof != profMax-1) {

        if (etage == MAX && max(maxActuel, note) >= minActuel) {  // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage maxActuel %d >= minActuel %d, cette branche est deja plus haute ou egale au min precedent", max(maxActuel, note), minActuel);
            getchar();
        }
        else if (etage == MIN && min(minActuel, note) < maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage minActuel %d < maxActuel %d, cette branche est deja plus basse ou egale au sup precedent", min(minActuel, note), maxActuel);
            getchar();
        }
    }

    printf("\n");

}
