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

#define PROF_MAX 4

#define DEBUG 1


enum {
    MAX,
    MIN,
};

Point botCoup(char **plateau, int taille, int mode, Joueur *depart);
void effectueCoup(char **plateau, Point depart, Point arrivee);
int MinMax(char **plateau, int taille, int mode, int prof, int etage);
Point trouveJoueur(char** plateau, int taille, int etage);
int evaluation(char **plateau, int taille, int mode);
void effaceCoup(char **plateau, Point depart, Point arrivee);
void affichePlateauDebug(char **plateau, int taille, int prof, Point depart, Point arrivee);
int finPartieBot(char **plateau, int taille, int mode);
int blocageJoueur(char **plateau, int taille, int mode);
Noeud* listeCoups(Noeud* teteliste, Point depart, char **plateau, int taille, int mode);
int minNote(NoeudNote *liste);
int maxNote(NoeudNote *liste);
Point meilleurCoup(NoeudCoupNote *liste);

#endif