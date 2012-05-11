#include "header/bot.h"

void botCoup(char **plateau, int taille, int mode, Config debug, int profMax, Joueur *tabJoueur, int bot) {

    float debut, temps;
    char valeurCoup;
    int i, blocageTmpBot, blocageTmpAdv;
    char **plateauTemp = NULL;
    PointNote maxActuel, coupNote;
    ElemPoint *coups = NULL;
    ElemPoint *pionActuel = NULL;

    if (tabJoueur[bot].blocage == FAUX) {

        debut = clock();

        if (mode == SERPENT) {

            pionActuel = empiler(pionActuel, tabJoueur[bot].pion->pos);
            plateauTemp = plateau;
        }
        else if (mode == PIEUVRE) {

            pionActuel = tabJoueur[bot].pion;
            plateauTemp = malloc(taille * sizeof(char*));
            for (i=0; i< taille ;i++ ) {
                plateauTemp[i] = malloc(taille * sizeof(char));
                memcpy(plateauTemp[i], plateau[i], taille * sizeof(char));
            }
        }

        maxActuel.note = EVAL_MIN;
        blocageTmpBot = tabJoueur[MAX].blocage;
        blocageTmpAdv = tabJoueur[MIN].blocage;

        do {

            coups = creerPileCoupsPossibles(coups, plateauTemp, taille, mode, pionActuel->pos);

            while (coups != NULL) {

                DEBUG_DEBUT_1(debug.vue);

                effectueCoup(plateau, plateauTemp, mode, tabJoueur, bot, coups->pos, &valeurCoup);

                chercheBlocage(plateau, taille, mode, &tabJoueur[bot]);
                chercheBlocage(plateau, taille, mode, &tabJoueur[!bot]);

                if (tabJoueur[!bot].blocage == VRAI) // Si le joueur adverse bloqué, on continue solo
                    coupNote.note = AlphaBeta(plateau, taille, mode, debug, 1, profMax, tabJoueur, bot, bot, MAX, maxActuel.note, EVAL_MAX);
                else
                    coupNote.note = AlphaBeta(plateau, taille, mode, debug, 1, profMax, tabJoueur, bot, !bot, MIN, maxActuel.note, EVAL_MAX);

                coupNote.pos = coups->pos;

                maxActuel = maxPointNote(maxActuel, coupNote);

                annuleCoup(plateau, mode, tabJoueur, bot, blocageTmpBot, blocageTmpAdv, coups->pos, valeurCoup, pionActuel->pos);

                DEBUG_FIN_1(debug.vue);

                coups = depiler(coups);

            }

            pionActuel = pionActuel->suivant;

        } while (pionActuel != NULL);

        appliqueCoup(plateau, &tabJoueur[bot], maxActuel.pos, mode);

        chercheBlocage(plateau, taille, mode, &tabJoueur[bot]);
        chercheBlocage(plateau, taille, mode, &tabJoueur[!bot]);

        temps = (clock() - debut)/CLOCKS_PER_SEC;
        printf("Temps de calcul: %f\n", temps);

    }

    if (debug.pause)
        getchar();

}


int AlphaBeta(char **plateau, int taille, int mode, Config debug, int prof, int profMax, Joueur *tabJoueur, int bot, int jActuel, int etage, int maxActuel, int minActuel) {

    ElemPoint *coups = NULL, *pionActuel = NULL;
    char valeurCoup;
    int blocageTmpBot, blocageTmpAdv;
    char **plateauTemp = NULL;
    int note, i;

    if ((tabJoueur[jActuel].blocage && tabJoueur[!jActuel].blocage) || (prof == profMax && !tabJoueur[jActuel].blocage && !tabJoueur[!jActuel].blocage))  { // Si les 2 sont bloqués ou horizon atteint

        return tabJoueur[bot].score - tabJoueur[!bot].score;
    }
    else {

        DEBUG_SAUT(debug.vue);

        if (etage == MIN)
            minActuel = EVAL_MAX;
        else if (etage == MAX)
            maxActuel = EVAL_MIN;

        if (mode == SERPENT) {
            plateauTemp = plateau;
            pionActuel = empiler(pionActuel, tabJoueur[jActuel].pion->pos);
        }
        else if (mode == PIEUVRE) {
            pionActuel = tabJoueur[jActuel].pion;
            plateauTemp = malloc(taille * sizeof(char*));
            for (i=0; i< taille ;i++ ) {
                plateauTemp[i] = malloc(taille * sizeof(char));
                memcpy(plateauTemp[i], plateau[i], taille * sizeof(char));
            }
        }

        blocageTmpBot = tabJoueur[MAX].blocage;
        blocageTmpAdv = tabJoueur[MIN].blocage;

        do {

            coups = creerPileCoupsPossibles(coups, plateauTemp, taille, mode, pionActuel->pos);

            while (coups != NULL) {

                DEBUG_DEBUT(debug.vue);

                effectueCoup(plateau, plateauTemp, mode, tabJoueur, jActuel, coups->pos, &valeurCoup);

                chercheBlocage(plateau, taille, mode, &tabJoueur[0]);
                chercheBlocage(plateau, taille, mode, &tabJoueur[1]);

                // Si le joueur adverse est bloqué, on continue solo
                if (tabJoueur[!jActuel].blocage == VRAI)
                    note = AlphaBeta(plateau, taille, mode, debug, prof+1, profMax, tabJoueur, bot, jActuel, etage, maxActuel, minActuel);
                else
                    note = AlphaBeta(plateau, taille, mode, debug, prof+1, profMax, tabJoueur, bot, !jActuel, !etage, maxActuel, minActuel);

                annuleCoup(plateau, mode, tabJoueur, jActuel, blocageTmpBot, blocageTmpAdv, coups->pos, valeurCoup, pionActuel->pos);

                DEBUG_FIN(debug.vue);

                if (etage == MAX) {

                    maxActuel = max(maxActuel, note);
                    if (maxActuel > minActuel) { // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
                        LIBERATION;
                        return maxActuel;
                    }
                } else if (etage == MIN) {

                    minActuel = min(minActuel, note);
                    if (minActuel < maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
                        LIBERATION;
                        return minActuel;
                    }
                }

                coups = depiler(coups);

            }

            if (mode == SERPENT)
                pionActuel = depiler(pionActuel);
            else
                pionActuel = pionActuel->suivant;

        } while (pionActuel != NULL);

    }

    if (mode == PIEUVRE) {
        for (i=0; i < taille;i++)
            free(plateauTemp[i]);
        free(plateauTemp);
    }

    if (etage == MAX)
        return maxActuel;
    else
        return minActuel;

}

/*  Sauvegarde les états des joueurs et la valeur de la case future
    Puis met le code du joueur sur la case et met a jour les coordoonées et le score du joueur */

void effectueCoup(char **plateau, char **plateauTemp, int mode, Joueur *tabJoueur, int jActuel, Point coup, char *valeurCoup) {

    *valeurCoup = plateau[coup.y][coup.x];

    plateau[coup.y][coup.x] = jActuel + 100;

    if (mode == SERPENT)
        tabJoueur[jActuel].pion->pos = coup;
    else {
        tabJoueur[jActuel].pion = empiler(tabJoueur[jActuel].pion, coup);
        plateauTemp[coup.y][coup.x] = jActuel + 100;
    }

    tabJoueur[jActuel].score += *valeurCoup;
}

/*  Remet la valeur originelle de la case et restaure les coordonnées, le score et les états des joueurs */

void annuleCoup(char **plateau, int mode, Joueur *tabJoueur, int jActuel, int blocageTmpBot, int blocageTmpAdv, Point coup, char valeurCoup, Point depart) {

    //if (mode == SERPENT && !tabJoueur[1].blocage)
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


