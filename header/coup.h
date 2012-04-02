#ifndef H_COUP
#define H_COUP

#include <stdio.h>
#include <stdlib.h>

#include "header/pile.h"
#include "header/general.h"

/** \file coup.h
 * \brief Gestion des déplacements
 */


void faireCoup(char **plateau, int taille, int mode, Joueur *depart);

void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur);

Noeud* creerPileCoupsPossibles(Noeud *pile, char **plateau, int taille, int mode, Point depart);
Noeud* creerPilePions(char **plateau, int taille, int idJjoueur);

Point saisieCoup(int taille);

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee);

#endif
