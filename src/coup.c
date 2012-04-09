#include "header/coup.h"

/** \fn void faireCoup(char **plateau, int taille, int mode, Joueur *joueur)
 * \brief Effectue la saisie, l'analyse et l'application d'un coup
 *
 * \param plateau Le plateau de jeu
 * \param taille La taille du plateau
 * \param mode Le mode de jeu (Pieuvre/Serpent)
 * \param joueur Le joueur actuellement en train de jouer
 *
 *
 */

void faireCoup(char **plateau, int taille, int mode, Joueur *joueur) {

    Point caseDepart, caseArrivee;
    int erreurArrivee, erreurDepart;

    chercheBlocage(plateau, taille, mode, joueur);

    if (joueur->blocage == FAUX) {

        printf("\nEntrez un coup : ");

        do {

            if (mode == PIEUVRE) {

                do  {

                    erreurDepart = 1;
                    caseDepart = saisieCoup(taille);

                    if (plateau[caseDepart.y][caseDepart.x] == joueur->id)
                        erreurDepart = 0;
                    else
                        printf("Case de depart incorrecte\n");

                } while (erreurDepart);

                caseArrivee = saisieCoup(taille);
                erreurArrivee = verifieCoup(plateau, mode, caseDepart, caseArrivee);

            } else {

                caseArrivee = saisieCoup(taille);
                erreurArrivee = verifieCoup(plateau, mode, joueur->position, caseArrivee);
            }

            switch (erreurArrivee) {
            case 1:
                printf("Coup interdit - Case originelle\n");
                break;
            case 2:
                printf("Coup interdit - Case occupe\n");
                break;
            case 3:
                printf("Coup interdit - Diagonale\n");
                break;
            case 4:
                printf("Coup interdit - Case hors de portee\n");
                break;
            }

        } while (erreurArrivee);

        appliqueCoup(plateau, joueur, caseArrivee);

    } else {

        printf("\nVous etes bloque, vous passez votre tour");
        getchar();

    }
}

/** \fn void chercheBlocage(char **plateau, int mode, Joueur *joueur, ElemPoint *coupsPossibles)
    \brief Cherche si un joueur est bloqué

    Parcours la pile des cases adjacentes à la case du joueur.
	Si la case est libre, la fonctionne s'achève immédiatement.
	Sinon on recommence sur les suivantes jusqu'à la fin de la pile
	Si aucune case libre n'a été trouvée, la variable blocage du joueur est mise à VRAI.
	En mode Pieuvre, on effectue ce meme traitement mais sur toutes les cases que le joueur possède.

    \param plateau Le plateau de jeu
    \param mode Le mode de jeu (Pieuvre/Serpent)
    \param joueur Le joueur concerné
    \param coupsPossibles Une pile des cases proches
*/
void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur) {

    ElemPoint *pions = NULL;

    if (joueur->blocage == FAUX)
    {

        if (mode == PIEUVRE)
            pions = creerPilePions(plateau, taille, joueur->id);
        else
            pions = empiler(pions, joueur->position);

        joueur->blocage = VRAI;

        do {

            if (existeCoupsPossibles(plateau, taille, mode, pions->pos)) { // Si on trouve au moins 1 coup, on n'est pas bloqué, on s'arrete
                joueur->blocage = FAUX;
                liberePile(pions);
                break;
            }

            pions = depiler(pions);

        } while (pions != NULL);

    }

}

int existeCoupsPossibles(char **plateau, int taille, int mode, Point depart) {

    static Point tab[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
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

ElemPoint* creerPilePions(char **plateau, int taille, int idJoueur) {

    ElemPoint *pions = NULL;
    int i, j;

    for (i=0;i<taille;i++)

        for (j=0;j<taille;j++)

            if (plateau[i][j] == idJoueur)
                pions = empiler(pions, (Point){j, i});

    return pions;
}

/** \fn int verifieCoup(char **plateau, int mode, Point depart, Point arrivee)
 *\brief  Vérifie la validité d'un coup

  *  Analyse la distance du coup joué, la disponibilité de la case et la
    conformité avec le mode de jeu actuel
 *
 * \param Le plateau de jeu
 * \param Le mode de jeu
  * \param Un point de départ
  *\param Un point d'arrivée
 * \return 0 si le coup est valide, sinon un code d'erreur
 *
 */
int verifieCoup(char **plateau, int mode, Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int distance = abs(deltaX) + abs(deltaY);
    int destination = plateau[arrivee.y][arrivee.x];

    if (distance != 0)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == SERPENT && distance == 2)

            return 3; // Déplacement en diagonale

        else if (distance > 2)

            return 4; // Déplacement de 3 cases ou plus

        else if (CASEVIDE(destination))

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon il n'y a pas eu de déplacement
        return 1;

}

/* Applique le coup au plateau et met a jour la position du joueur
    Attend :
        Un plateau, un point de départ et d'arrivée */

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee) {

    depart->score += plateau[arrivee.y][arrivee.x];

    plateau[arrivee.y][arrivee.x] = plateau[depart->position.y][depart->position.x];

    depart->position = arrivee;

}

/* Demande une saisie à l'utilisateur et l'analyse. Affiche un message indiquant
    si la saisie n'a pas pu etre interprete ou si elle est hors du plateau
    Attend :
        La taille du plateau
    Retourne :
        Le point saisi par l'utilisateur */

Point saisieCoup(int taille) {

    Point coup;
    int erreur = 0;
    char tmp;
    char saisie[3] = {0};
    int ligne = 0;

    do {

        erreur = 0;
        fgets(saisie, 4, stdin); // fgets rajoute '\0' au dernier element

        if (saisie[0] >= 'a' && saisie[0] <= 'z') // Si premier carac est une lettre
        {

            if (saisie[0] >= 'a' && saisie[0] <= 'a' + taille-1) // Si lettre correcte
            {

                coup.x = saisie[0] - 'a';

                if (saisie[1] >= '0' && saisie[1] <= '9') // Si second carac est un chiffre
                {

                    if (saisie[2] >= '0' && saisie[2] <= '9') // Si troisieme carac est un chiffre
                    {
                        ligne = atoi(&saisie[1]);
                        getchar(); // On lit le \n parti dans le tampon
                    }
                    else if (saisie[2] == '\n')
                        ligne = saisie[1] - '0';
                    else
                        getchar();

                    if (ligne < taille)
                        coup.y = ligne;
                    else
                        erreur = 2; // Ligne hors plateau

                } else
                    erreur = 4; //Second carac incorrect

            } else
                erreur = 1; // Colonne hors plateau

        } else {

            erreur = 3; // Premier carac incorrect
            if (saisie[0] != '\n' && saisie[1] != '\n' && saisie[2] != '\n')
                while ((tmp = getchar()) != '\n' && tmp != EOF);
        }

        switch (erreur) {
            case 1:
                printf("Attention, vous n'avez droit qu'aux colonnes de [a] jusqu'a [%c]\n", 'a' + taille-1);
                break;
            case 2:
                printf("Attention, vous n'avez droit qu'aux lignes de [0] jusqu'a [%d]\n", taille-1);
                break;
            case 3:
                printf("Attention, ligne non reconnue\n");
                break;
            case 4:
                printf("Attention, colonne non reconnue\n");
                break;
        }

    } while (erreur);

    return coup;
}
