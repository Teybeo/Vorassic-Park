#ifndef H_AFFICHAGE
#define H_AFFICHAGE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio2.h>

#include "header/general.h"

void affichage(char **plateau, int taille, int tour, char *modeJeu, int nbJoueurs, Joueur *joueur, Joueur joueurActuel);
void affichePlateau(char **plateau, int taille);
void couleurs(char **plateau, Joueur joueur);

#endif

