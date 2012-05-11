#ifndef H_ENTREE
#define H_ENTREE

#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>

#include "header/general.h"
#include "header/affichage.h"

#define HAUT 72
#define GAUCHE 75
#define DROITE 77
#define BAS 80
#define ENTER 13
#define ECHAP 27

int faireCoupVisuel(char **plateau, int taille, int mode, Joueur *tabJoueur, int humain, int nbJoueurs);

int caseAtteignable(char **plateau, int taille, Joueur joueur, Point arrivee);


void faireCoupTextuel(char **plateau, int taille, int mode, Joueur *depart, int humain, int nbJoueurs);

Point saisieCoup(int taille);

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);

#endif

