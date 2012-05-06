#ifndef H_BOT
#define H_BOT

#include <conio2.h>

#include "header/general.h"
#include "header/pile.h"
#include "header/partie.h"

#define EVAL_MIN -10000
#define EVAL_MAX 10000

#define DEBUG 0

#define MAX 0
#define MIN 1

#define DEBUG_DEBUT_1(x) \
    if (x >= 1) \
        debugDebut(plateau, taille, mode, 0, profMax, tabJoueur[bot].pion->pos, tabJoueur[!bot].pion->pos, pionTemp->pos, coups->pos, tabJoueur[bot].id);

#define DEBUG_FIN_1(x) \
    if (x >= 1) \
        debugFin(0, coupNote.note, 0, EVAL_MIN, EVAL_MAX, 0, coups);

#define DEBUG_SAUT(x) \
    if (x == 2) \
        printf("\n");

#define DEBUG_FIN(x) \
    if (x == 2) \
        debugFin(prof, note, profMax, maxActuel, minActuel, etage, coups);

#define DEBUG_DEBUT(x) \
    if (x == 2) \
        debugDebut(plateau, taille, mode, prof, profMax, tabJoueur[bot].pion->pos, tabJoueur[!bot].pion->pos, pionTemp->pos, coups->pos, tabJoueur[jActuel].id);



void botCoup(char **plateau, int taille, int mode, int profMax, Joueur *tabJoueur, int bot);
int AlphaBeta(char **plateau, int taille, int mode, int prof, int profMax, Joueur *tabJoueur, int bot, int jActuel, int etage, int maxActuel, int minActuel);

int MinMax(char **plateau, int taille, int mode, int prof, int profMax, Joueur *bot, Joueur *adversaire, int etage);

void effectueCoup(char **plateau, char**plateauTemp, int mode, Joueur *tabJoueur, int etage, Point coup, char *valeurCoup);
void annuleCoup(char **plateau, int mode, Joueur *tabJoueur, int etage, int blocageTmpBot, int blocageTmpAdv, Point coup, char valeurCoup, Point depart);

int min(int a, int b);
int max(int a, int b);
inline PointNote maxPointNote(PointNote a, PointNote b);

void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point actuel, Point arrivee, int id);
void debugDebut(char **plateau, int taille, int mode, int prof, int profMax, Point bot, Point adversaire, Point depart, Point arrive, int id);
void debugFin(int prof, int note, int profMax, int maxActuel, int minActuel, int etage, ElemPoint *coup) ;

#endif
