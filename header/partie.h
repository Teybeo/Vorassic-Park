#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/plateau.h"
#include "header/coup.h"

void executePartie(int taille, int mode, int aleatoire);
int finPartie(Point joueur1, Point joueur2);
void resultat(int scoreJ1, int scoreJ2);

#endif
