#ifndef H_BOT
#define H_BOT

#include "header/general.h"
#include "header/liste.h"
#include "header/listeNote.h"
#include "header/listeMouvement.h"
#include "header/listeMouvementNote.h"
#include "header/partie.h"

#define EVAL_MIN -10000
#define EVAL_MAX 10000

enum {
    MAX,
    MIN
};

Point botCoup(char **plateau, int taille, int mode, Joueur *depart);
void effectueCoup(char **plateau, Point depart, Point arrivee);
int MinMax(char **plateau, int taille, int prof, int etage);
Point trouveJoueur(char** plateau, int taille, int etage);
int evaluation(char **plateau, int taille, int etage);
int victoireDefaite(char **plateau, int taille, int etage);
NoeudMouvmt* listeMouvmt(NoeudMouvmt* teteliste, Point depart, int taille, int mode);
int minNote(NoeudNote *liste);
int maxNote(NoeudNote *liste);

#endif
