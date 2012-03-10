#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/plateau.h"

#define X 0
#define Y 1

typedef struct Point {

    int x;
    int y;

} Point;

void executePartie();

void faireCoup(char plateau[5][5], Point *depart);
Point saisieCoup();
int verifieCoup(char plateau[5][5], Point depart, Point arrivee);
void appliqueCoup(char plateau[5][5], Point *depart, Point arrivee);

#endif
