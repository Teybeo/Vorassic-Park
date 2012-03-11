#ifndef H_COUP
#define H_COUP

#include <stdio.h>
#include <stdlib.h>

#include "header/liste.h"

typedef struct Point {

    int x;
    int y;

} Point;

int faireCoup(char **plateau, int taille, int mode, Point *depart);
Point saisieCoup(int taille);
int verifieCoup(char **plateau, int mode, Point depart, Point arrivee);
int verifieBlocage(char **plateau, int mode, Point depart, Noeud *coupPossibles);
Noeud* listeCoup(int taille, int mode, Point depart);
void appliqueCoup(char **plateau, Point *depart, Point arrivee);

#endif
