#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>

#include "header/plateau.h"
#include "header/coup.h"

void executePartie(int taille, int mode);
int finPartie(Point joueur1, Point joueur2);
void score(char **plateau, int taille);

#endif
