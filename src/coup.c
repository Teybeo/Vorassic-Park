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

    Point caseDepart, coup;
    int erreur = 0, erreurDepart = 1;
    int valeurCase = 0;

    chercheBlocage(plateau, taille, mode, joueur);

    if (joueur->blocage == 0) {

        do {

            if (mode == 1) {

                do  {

                    erreurDepart = 1;
                    caseDepart = saisieCoup(taille);

                    if ((plateau[caseDepart.y][caseDepart.x] == 'C' && joueur->id == 0) ||
                        (plateau[caseDepart.y][caseDepart.x] == 'R' && joueur->id == 1) ||
                        (plateau[caseDepart.y][caseDepart.x] == 'V' && joueur->id == 2) ||
                        (plateau[caseDepart.y][caseDepart.x] == 'B' && joueur->id == 3))
                        erreurDepart = 0;
                    else
                        printf("Case de depart incorrecte\n");

                } while (erreurDepart);

                coup = saisieCoup(taille);
                erreur = verifieCoup(plateau, mode, caseDepart, coup);

            } else {

                coup = saisieCoup(taille);
                erreur = verifieCoup(plateau, mode, joueur->position, coup);
            }

            switch (erreur) {
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

        } while (erreur);

        valeurCase = plateau[coup.y][coup.x];
        appliqueCoup(plateau, mode, joueur, coup);

    } else {

        printf("\nVous etes bloque, vous passez votre tour");
        getchar();

    }
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
    int delta = abs(deltaX) + abs(deltaY);
    int destination = plateau[arrivee.y][arrivee.x];

    if (delta != 0)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == 0 && delta == 2)

            return 3; // Déplacement en diagonale

        else if (delta > 2)

            return 4; // Déplacement de 3 cases ou plus

        else if (destination != 'C' && destination != 'R' && destination != 'c' && destination != 'r' &&
                  destination != 'V' && destination != 'v' && destination != 'B' && destination != 'b' && destination != 'x')

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon il n'y a pas eu de déplacement
        return 1;

}


/** \fn void chercheBlocage(char **plateau, int mode, Joueur *joueur, Noeud *casesAdjacentes)
    \brief Cherche si un joueur est bloqué

    Parcours la liste des cases adjacentes et si aucune case libre n'est trouvée,
    la variable blocage du joueur est passée à 1

    \param plateau Le plateau de jeu
    \param mode Le mode de jeu (Pieuvre/Serpent)
    \param joueur Le joueur concerné
    \param casesAdjacentes Une liste des cases proches*/
void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur) {

    Noeud *casesAdjacentes = NULL;
    Noeud *casesAcquises = NULL;

    if (mode == 1)
        casesAcquises = listeCasesAcquises(plateau, taille, joueur->id);
    else
        casesAcquises = ajoutTete(casesAcquises, joueur->position);

    do {

        casesAdjacentes = listeCases(taille, mode, casesAcquises->pos);

        while (casesAdjacentes != NULL) {

            if (verifieCoup(plateau, mode, casesAcquises->pos, casesAdjacentes->pos) == 0) {
                libereListe(casesAdjacentes);
                return;
            }

            casesAdjacentes = casesAdjacentes->suivant;
        }

        casesAcquises = casesAcquises->suivant;
        libereListe(casesAdjacentes);

    } while (casesAcquises != NULL);

    joueur->blocage = 1;

}

Noeud* listeCasesAcquises(char **plateau, int taille, int joueur ) {

    Noeud *casesAcquises = NULL;
    int i, j;
    char tab[4] = {'C', 'R', 'V', 'B'};
    char c = tab[joueur];

    for (i=0;i<taille;i++) {

        for (j=0;j<taille;j++) {

            if (plateau[i][j] == c || plateau[i][j] == c + 32)
                casesAcquises = ajoutTete(casesAcquises, (Point){j, i});
        }
    }


    return casesAcquises;
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

/* Crée une liste des cases adjacentes à un certain point
    Attend :
        La taille du plateau, le mode de jeu et un point de départ
    Retourne :
        La liste des cases adjacentes */

Noeud* listeCases(int taille, int mode, Point depart) {

    Noeud *liste = NULL;
    Point tmp;
    tmp.x = tmp.y = 0;

    if (depart.y > 0) { // Haut
        tmp.x = depart.x;
        tmp.y = depart.y - 1;
        liste = ajoutTete(liste, tmp);
    }
    if (depart.y < taille-1) { // Bas
        tmp.x = depart.x;
        tmp.y = depart.y + 1;
        liste = ajoutTete(liste, tmp);
    }
    if (depart.x > 0) {
        // Gauche
        tmp.x = depart.x - 1;
        tmp.y = depart.y;
        liste = ajoutTete(liste, tmp);

        if (mode && depart.y > 0) { // Gauche / Haut
            tmp.x = depart.x - 1;
            tmp.y = depart.y - 1;
            liste = ajoutTete(liste, tmp);
        }
        if (mode && depart.y < taille-1) { // Gauche / Bas
            tmp.x = depart.x - 1;
            tmp.y = depart.y + 1;
            liste = ajoutTete(liste, tmp);
        }

    }
    if (depart.x < taille-1) {
        // Droite
        tmp.x = depart.x + 1;
        tmp.y = depart.y;
        liste = ajoutTete(liste, tmp);

        if (mode && depart.y > 0) { // Droite / Haut
            tmp.x = depart.x + 1;
            tmp.y = depart.y - 1;
            liste = ajoutTete(liste, tmp);
        }
        if (mode && depart.y < taille-1) { // Droite / Bas
            tmp.x = depart.x + 1;
            tmp.y = depart.y + 1;
            liste = ajoutTete(liste, tmp);
        }

    }

    return liste;
}

/* Applique le coup au plateau et met a jour la position du joueur
    Attend :
        Un plateau, un point de départ et d'arrivée */

void appliqueCoup(char **plateau, int mode, Joueur *depart, Point arrivee) {

    depart->score += plateau[arrivee.y][arrivee.x];

    /* Le J ou R se trouvant a la case depart est copié a la case d'arrivée
       La valeur de la case de départ est décalé de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->position.y][depart->position.x];
    if (mode == 0)
        plateau[depart->position.y][depart->position.x] += 32;

    depart->position = arrivee;

}

