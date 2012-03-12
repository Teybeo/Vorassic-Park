#include "header/coup.h"


/* Effectue la saisie, l'analyse et l'application d'un coup
    Attend :
        Un plateau, sa taille, le mode de jeu et un point de depart
    Retourne :
        La valeur de la case acquise par ce coup */

void faireCoup(char **plateau, int taille, int mode, Joueur *joueur) {

    Point coup;
    int erreur;
    int valeurCase = 0;

    Noeud *casesAdjacentes = listeCases(taille, mode, joueur->position);
    chercheBlocage(plateau, mode, joueur, casesAdjacentes);
    libereListe(casesAdjacentes);

    if (joueur->blocage == 0) {

        do {

            coup = saisieCoup(taille);
            erreur = verifieCoup(plateau, mode, joueur->position, coup);

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
        appliqueCoup(plateau, joueur, coup);

    } else {

        printf("\nVous etes bloque, vous passez votre tour");
        getchar();

    }
}

/* Analyse la distance du coup joué, la disponibilité de la case et la
    conformité avec le mode de jeu actuel
    Attend :
        Le plateau, le mode de jeu, le point de départ et d'arrivée
    Retourne :
        0 si le coup est valide, sinon un code d'erreur*/

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int delta = abs(deltaX) + abs(deltaY);
    int destination = plateau[arrivee.y][arrivee.x];

    if (delta != 0)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == 0 && delta == 2)

            return 3; // Déplacement en diagonale

        else if (delta > 1)

            return 4; // Déplacement de 3 cases ou plus

        else if (destination != 'C' && destination != 'R' && destination != 'c' && destination != 'r' &&
                  destination != 'V' && destination != 'v' && destination != 'B' && destination != 'b')

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon il n'y a pas eu de déplacement
        return 1;

}


/* Cherche si un joueur est bloqué à partir de la liste des cases adjacentes
    et met ses coordonnées a -1 s'il l'est
    Attend :
        Un plateau, le mode de jeu, un point de depart et une liste de coups */

void chercheBlocage(char **plateau, int mode, Joueur *joueur, Noeud *casesAdjacentes) {

    Noeud *tmp = casesAdjacentes;
    Point coup;

    while (tmp != NULL) {

        coup.x = tmp->x;
        coup.y = tmp->y;

        if (verifieCoup(plateau, mode, joueur->position, coup) == 0)
            return;

        tmp = tmp->suivant;
    }

    joueur->blocage = 1;

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

    if (depart.y > 0) // Haut
        liste = ajoutFin(liste, depart.x, depart.y - 1);
    if (depart.y < taille-1) // Bas
        liste = ajoutFin(liste, depart.x, depart.y + 1);

    if (depart.x > 0) {
        // Gauche
        liste = ajoutFin(liste, depart.x - 1, depart.y);

        if (mode && depart.y > 0) // Gauche / Haut
            liste = ajoutFin(liste, depart.x - 1, depart.y - 1);
        if (mode && depart.y < taille-1) // Gauche / Bas
            liste = ajoutFin(liste, depart.x - 1, depart.y + 1);

    }
    if (depart.x < taille-1) {
        // Droite
        liste = ajoutFin(liste, depart.x + 1, depart.y);

        if (mode && depart.y > 0) // Droite / Haut
            liste = ajoutFin(liste, depart.x + 1, depart.y - 1);
        if (mode && depart.y < taille-1) // Droite / Bas
            liste = ajoutFin(liste, depart.x + 1, depart.y + 1);

    }

    return liste;
}

/* Applique le coup au plateau et met a jour la position du joueur
    Attend :
        Un plateau, un point de départ et d'arrivée */

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee) {

    depart->score += plateau[arrivee.y][arrivee.x];

    /* Le J ou R se trouvant a la case depart est copié a la case d'arrivée
       La valeur de la case de départ est décalé de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->position.y][depart->position.x];
    plateau[depart->position.y][depart->position.x] += 32;

    depart->position = arrivee;

}

