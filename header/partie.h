#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/plateau.h"
#include "header/coup.h"

void executePartie(int nbJoueurs, int taille, int mode, int aleatoire, char **noms);
Joueur* initJoueurs(int nbJoueurs, char **noms, int taille);
int finPartie(Joueur *tab, int nbJoueurs);
void resultat(Joueur *tab, int nbJoueurs);

#endif
