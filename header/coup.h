#ifndef H_COUP
#define H_COUP

#include <stdio.h>
#include <stdlib.h>

#include "header/liste.h"
#include "header/general.h"

/** \file coup.h
 * \brief Gestion des déplacements
 */


void faireCoup(char **plateau, int taille, int mode, Joueur *depart);

Point saisieCoup(int taille);

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);

void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur);

Noeud* listeCases(int taille, int mode, Point depart);
Noeud* listeCasesAcquises(char **plateau, int taille, int joueur);

void appliqueCoup(char **plateau, int mode, Joueur *depart, Point arrivee);

#endif
