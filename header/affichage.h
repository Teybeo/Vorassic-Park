#ifndef H_AFFICHAGE
#define H_AFFICHAGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>

#include "header/general.h"
#include "header/bot.h"

#define DEBUG_DEBUT_1(x) \
    if (x >= 1) \
        debugDebut(plateau, taille, mode, 0, profMax, tabJoueur[bot].pion->pos, tabJoueur[!bot].pion->pos, pionActuel->pos, coups->pos, tabJoueur[bot].id);

#define DEBUG_FIN_1(x) \
    if (x >= 1) \
        debugFin(0, coupNote.note, 0, EVAL_MIN, EVAL_MAX, 0, coups);

#define DEBUG_SAUT(x) \
    if (x == 2) \
        printf("\n");

#define DEBUG_FIN(x) \
    if (x == 2) \
        debugFin(prof, note, profMax, maxActuel, minActuel, etage, coups);

#define DEBUG_DEBUT(x) \
    if (x == 2) \
        debugDebut(plateau, taille, mode, prof, profMax, tabJoueur[bot].pion->pos, tabJoueur[!bot].pion->pos, pionActuel->pos, coups->pos, tabJoueur[jActuel].id);


/* --- Gère l'affichage lors d'une partie --- */

void affichage(char **plateau, int taille, int tour, int mode, int nbJoueurs, Joueur *joueur, int joueurActuel);
void affichePlateau(char **plateau, int taille, int mode, Joueur *tabJoueur, int nbJoueurs);
void resultat(Joueur *joueur, int nbJoueurs);
void afficheDirection(Point depart, Point arrivee);
void couleurs(int idJoueur);

void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point actuel, Point arrivee, int id);
void debugDebut(char **plateau, int taille, int mode, int prof, int profMax, Point bot, Point adversaire, Point depart, Point arrive, int id);
void debugFin(int prof, int note, int profMax, int maxActuel, int minActuel, int etage, ElemPoint *coup) ;

#endif

