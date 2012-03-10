#ifndef H_PARTIE
#define H_PARTIE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 0
#define Y 1

void executePartie();

void saisieCoup(char coup[2]);
void verifieCoup(char plateau[5][5], char pos[2], char coup[2]);

void initPlateau(char plateau[5][5]);
void affichePlateau(char plateau[5][5]);

#endif
