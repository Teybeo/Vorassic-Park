#ifndef H_COUP
#define H_COUP

/* --- Gestion des déplacements --- */

#include <stdlib.h>
#include "header/pile.h"
#include "header/general.h"

void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur);

int existeCoupsPossibles(char **plateau, int taille, int mode, Point depart);

ElemPoint* creerPileCoupsPossibles(ElemPoint *pile, char **plateau, int taille, int mode, Point depart);

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee, int mode);

#endif
