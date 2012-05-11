#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "general.h"

#include "header/affichage.h"
#include "header/pile.h"
#include "header/bot.h"
#include "header/entree.h"

void executePartie(int nbJoueurs, int nbBots, int taille, int mode, int prof, int aleatoire, char **noms, Config debug);

char** initPlateau(int taille, int aleatoire, Joueur *joueur, int nbJoueurs);

Joueur* initJoueurs(int nbJoueurs, int nbBots, char **noms, int taille);

int finPartie(Joueur *tab, int nbJoueurs);


#endif
