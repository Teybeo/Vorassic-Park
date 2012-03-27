#ifndef H_BOT
#define H_BOT

#include <conio2.h>

#include "header/general.h"
#include "header/liste.h"
#include "header/listeNote.h"
#include "header/listeCoupNote.h"
#include "header/partie.h"

#define EVAL_MIN -10000
#define EVAL_MAX 10000

#define PROF_MAX 15

#define DEBUG

enum {
    MAX,
    MIN,
};

void botCoup(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire);
int MinMax(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire, int prof, int etage);

int finPartieBot(char **plateau, int taille, int mode);
int blocageJoueur(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire);
int evaluation(char **plateau, int taille, int mode);

Noeud* listeCasesBot(Noeud *liste,  Point depart, char **plateau, int taille, int mode);

void effectueCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp);
void annuleCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp);

int minNote(NoeudNote *liste);
int maxNote(NoeudNote *liste);
Point meilleurCoup(NoeudCoupNote *liste);

void affichePlateauDebug(char **plateau, int taille, int prof, Point depart, Point arrivee);

#endif
