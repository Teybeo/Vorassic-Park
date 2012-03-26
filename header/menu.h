#ifndef H_MENU
#define H_MENU

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header/partie.h"
#include "header/general.h"

#define NB_JOUEURS_MAX 4
#define NB_JOUEURS_MIN 2

#define NB_BOTS_MAX 2
#define NB_BOTS_MIN 1

/** \file menu.c
 * \brief Menus
 *
 */


void menu();
void options(int *taille, int *mode, int *aleatoire, int *nbJoueurs, int *nbBots, char ***noms);

#endif
