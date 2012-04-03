#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/affichage.h"
#include "header/coup.h"
#include "header/bot.h"

void executePartie(int nbJoueurs, int nbBots, int taille, int mode, int prof, int aleatoire, char **noms);

char** initPlateau(int taille, int aleatoire, Joueur *joueur, int nbJoueurs);

Joueur* initJoueurs(int nbJoueurs, int nbBots, char **noms, int taille);

int finPartie(Joueur *tab, int nbJoueurs);


#endif
