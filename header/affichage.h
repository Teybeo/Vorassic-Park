#ifndef H_AFFICHAGE
#define H_AFFICHAGE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio2.h>

#include "header/general.h"

/** \file affichage.h
 *
 * \brief Gère l'affichage lors d'une partie
 *
 */


void affichage(char **plateau, int taille, int tour, char *modeJeu, int nbJoueurs, Joueur *joueur, Joueur joueurActuel);
void affichePlateau(char **plateau, int taille);
void resultat(Joueur *joueur, int nbJoueurs, char **plateau);
void afficheDirection(Point depart, Point arrivee);
void couleurs(char **plateau, Joueur joueur);

#endif

