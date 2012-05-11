#ifndef H_COUP
#define H_COUP

/* --- Gestion des déplacements --- */

#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>

#include "header/pile.h"
#include "header/general.h"
#include "header/affichage.h"

#define HAUT 72
#define GAUCHE 75
#define DROITE 77
#define BAS 80
#define ENTER 13
#define ECHAP 27


int faireCoupVisuel(char **plateau, int taille, int mode, Joueur *tabJoueur, int humain, int nbJoueurs);
void faireCoupTextuel(char **plateau, int taille, int mode, Joueur *depart, int humain, int nbJoueurs);

void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur);
int existeCoupsPossibles(char **plateau, int taille, int mode, Point depart);

ElemPoint* creerPileCoupsPossibles(ElemPoint *pile, char **plateau, int taille, int mode, Point depart);

Point saisieCoup(int taille);

int caseAtteignable(char **plateau, int taille, Joueur joueur, Point arrivee);
int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee, int mode);

#endif
