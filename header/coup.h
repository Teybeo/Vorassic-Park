#ifndef H_COUP
#define H_COUP

#include <stdio.h>
#include <stdlib.h>

#include "header/liste.h"
#include "header/general.h"

void faireCoup(char **plateau, int taille, int mode, Joueur *depart);

Point saisieCoup(int taille);

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);

void chercheBlocage(char **plateau, int mode, Joueur *joueur, Noeud *coupPobles);

Noeud* listeCases(int taille, int mode, Point depart);

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee);

#endif
