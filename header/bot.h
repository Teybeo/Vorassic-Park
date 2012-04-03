#ifndef H_BOT
#define H_BOT

#include <conio2.h>

#include "header/general.h"
#include "header/pile.h"
#include "header/pileNote.h"
#include "header/pileCoupNote.h"
#include "header/partie.h"

#define EVAL_MIN -10000
#define EVAL_MAX 10000

#define DEBUG 2

#define MAX 0
#define MIN 1


void botCoup(char **plateau, int taille, int mode, int profMax, Joueur *bot, Joueur *adversaire);
int AlphaBeta(char **plateau, int taille, int mode, int prof, int profMax, Joueur *bot, Joueur *adversaire, int etage, int alpha, int beta);
int MinMax(char **plateau, int taille, int mode, int prof, int profMax, Joueur *bot, Joueur *adversaire, int etage);

int finPartieBot(char **plateau, int taille, int mode);

void effectueCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp);
void annuleCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point depart);

int min(int a, int b);
int max(int a, int b);
int minNote(NoeudNote *pile);
int maxNote(NoeudNote *pile);
Point meilleurCoup(NoeudCoupNote *pile);

void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point actuel, Point arrivee, int id);
void debugDebut(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point depart, Point arrive, int id);
void debugFin(int prof, int note) ;

#endif
