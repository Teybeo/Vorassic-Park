#ifndef H_BOT
#define H_BOT

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "header/affichage.h"
#include "header/general.h"
#include "header/pile.h"
#include "header/moteur.h"

#define EVAL_MIN -10000
#define EVAL_MAX 10000

#define MAX 0
#define MIN 1

#define LIBERATION  \
    liberePile(coups);  \
    if (mode == SERPENT)  \
        free(pionActuel);  \
    else  {  \
        for (i=0; i < taille; i++ )  \
            free(plateauTemp[i]);  \
        free(plateauTemp);  \
    }


void botCoup(char **plateau, int taille, int mode, Config debug, int profMax, Joueur *tabJoueur, int bot);
int AlphaBeta(char **plateau, int taille, int mode, Config debug, int prof, int profMax, Joueur *tabJoueur, int bot, int jActuel, int etage, int maxActuel, int minActuel);

int MinMax(char **plateau, int taille, int mode, int prof, int profMax, Joueur *bot, Joueur *adversaire, int etage);

void effectueCoup(char **plateau, char**plateauTemp, int mode, Joueur *tabJoueur, int etage, Point coup, char *valeurCoup);
void annuleCoup(char **plateau, int mode, Joueur *tabJoueur, int etage, int blocageTmpBot, int blocageTmpAdv, Point coup, char valeurCoup, Point depart);

int min(int a, int b);
int max(int a, int b);
inline PointNote maxPointNote(PointNote a, PointNote b);

#endif
