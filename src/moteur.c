#include "header/moteur.h"

/*  Cherche si un joueur est bloqué

    Parcours la pile des cases adjacentes à la case du joueur.
	Si la case est libre, la fonctionne s'achève immédiatement.
	Sinon on recommence sur les suivantes jusqu'à la fin de la pile
	Si aucune case libre n'a été trouvée, la variable blocage du joueur est mise à VRAI.
	En mode Pieuvre, on effectue ce meme traitement mais sur toutes les cases que le joueur possède.
*/
void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur) {

    ElemPoint *pionTemp = joueur->pion;

    if (joueur->blocage == FAUX)
    {
        joueur->blocage = VRAI;

        do {

            if (existeCoupsPossibles(plateau, taille, mode, pionTemp->pos)) { // Si on trouve au moins 1 coup, on n'est pas bloqué, on s'arrete
                joueur->blocage = FAUX;
                break;
            }

            pionTemp = pionTemp->suivant;

        } while (pionTemp != NULL);

    }

}

int existeCoupsPossibles(char **plateau, int taille, int mode, Point depart) {

    Point tab[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int i;
    Point pos;

    for (i=0; i < 4 * (1+mode); i++) {

        pos = (Point){depart.x + tab[i].x, depart.y + tab[i].y};

        if (pos.x >= 0 && pos.x < taille && pos.y >= 0 && pos.y < taille && CASEVIDE(plateau[pos.y][pos.x]))
            return VRAI;
    }

    return FAUX;
}

ElemPoint* creerPileCoupsPossibles(ElemPoint *pile, char **plateau, int taille, int mode, Point depart) {

    if (depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x])) // Haut

        pile = empiler(pile, (Point){depart.x, depart.y-1});


    if (depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x]))  // Bas

        pile = empiler(pile, (Point){depart.x, depart.y+1});


    if (depart.x > 0 && CASEVIDE(plateau[depart.y][depart.x-1])) // Gauche

        pile = empiler(pile, (Point){depart.x-1, depart.y});


    if (depart.x < taille-1 && CASEVIDE(plateau[depart.y][depart.x+1])) // Droite

        pile = empiler(pile, (Point){depart.x+1, depart.y});


    if (mode == PIEUVRE)
    {
        if (depart.x > 0 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x-1]))  // Gauche / Haut

            pile = empiler(pile, (Point){depart.x-1, depart.y-1});


        if (depart.x > 0 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x-1]))  // Gauche / Bas

            pile = empiler(pile, (Point){depart.x-1, depart.y+1});


        if (depart.x < taille-1 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x+1])) // Droite / Haut

            pile = empiler(pile, (Point){depart.x+1, depart.y-1});


        if (depart.x < taille-1 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x+1])) // Droite / Bas

            pile = empiler(pile, (Point){depart.x+1, depart.y+1});
    }


    return pile;
}


/* Applique le coup au plateau et met a jour le score et la position du joueur */

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee, int mode) {

    depart->score += plateau[arrivee.y][arrivee.x];

    plateau[arrivee.y][arrivee.x] = plateau[depart->pion->pos.y][depart->pion->pos.x];

    if (mode == SERPENT)
        depart->pion->pos = arrivee;
    else
        depart->pion = empiler(depart->pion, arrivee);

}

int caseAtteignable(char **plateau, int taille, Joueur joueur, Point arrivee) {

    int i;
    Point tab[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    Point depart;

    for (i=0; i < 8; i++) {

        depart = (Point){arrivee.x + tab[i].x, arrivee.y + tab[i].y};

        if (EXISTE_CASE(depart, taille) && plateau[depart.y][depart.x] == joueur.id)
            return VRAI;
    }

    return FAUX;
}
