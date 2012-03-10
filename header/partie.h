#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/plateau.h"
#include "header/liste.h"

#define X 0
#define Y 1

typedef struct Point {

    int x;
    int y;

} Point;

void executePartie(int taille);
int finPartie(Point joueur1, Point joueur2);
void score(char **plateau, int taille);

void faireCoup(char **plateau, int taille, Point *depart);
Point saisieCoup(int taille);
int verifieCoup(char **plateau, Point depart, Point arrivee);
int verifieBlocage(char **plateau, Point depart, Noeud *coupPossibles);
Noeud* listeCoup(char **plateau, int taille, Point depart);
void appliqueCoup(char **plateau, Point *depart, Point arrivee);

#endif
