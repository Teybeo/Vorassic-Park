#include "header/coup.h"

void faireCoup(char **plateau, int taille, int mode, Point *depart) {

    Point coup;
    int blocage, erreur;

    Noeud *coupPossibles = listeCoup(plateau, taille, mode, *depart);
    blocage = verifieBlocage(plateau, mode, *depart, coupPossibles);

    if (!blocage) {

        do {

            coup = saisieCoup(taille);
            erreur = verifieCoup(plateau, mode, *depart, coup);

            switch (erreur) {
            case 1:
                printf("Coup interdit - Deplacement invalide\n");
                break;
            case 2:
                printf("Coup interdit - Occupe\n");
                break;
            case 3:
                printf("Coup interdit - Diagonale\n");
                break;
            }

        } while (erreur);

        appliqueCoup(plateau, depart, coup);

    } else {

        printf("Vous etes bloque, vous passez votre tour");
        depart->x = -1;
        depart->y = -1;
        getchar();

    }

    libereListe(coupPossibles);

}


int verifieCoup(char **plateau, int mode, Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int destination = plateau[arrivee.y][arrivee.x];

    if (abs(deltaX) == 1 || abs(deltaY) == 1)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == 0 && abs(deltaX) == 1 && abs(deltaY) == 1)

            return 3; // D�placement en diagonale

        else if (destination != 'J' && destination != 'R' && destination != 'j' && destination != 'r')

            return 0; // Case libre

        else
            return 2; // Case occup�e
    }
    else // Sinon le deplacement est invalide
        return 1;

}


int verifieBlocage(char **plateau, int mode, Point depart, Noeud *coupPossibles) {

    if (depart.x == -1 && depart.y == -1)
        return 1;

    else {

        Noeud *tmp = coupPossibles;
        Point coup;

        while (tmp != NULL) {

            coup.x = tmp->x;
            coup.y = tmp->y;

            if (verifieCoup(plateau, mode, depart, coup) == 0)
                return 0;

            tmp = tmp->suivant;
        }

        return 1;

    }

}


Point saisieCoup(int taille) {

    Point coup;
    int erreur = 0;
    char tmp;
    char saisie[3] = {0};
    int ligne;

    do {

        erreur = 0;
        fgets(saisie, 4, stdin); // fgets rajoute '\0' au dernier element

        if (saisie[0] >= 'a' && saisie[0] <= 'a' + taille-1) // Si lettre correct
        {
            coup.x = saisie[0] - 'a';

            if (saisie[1] >= '0' && saisie[1] <= '9') // Si second carac est un chiffre
            {

                if (saisie[2] >= '0' && saisie[2] <= '9') { // Si troisieme carac est un chiffre
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
                    erreur = 2;

            } else
                erreur = 2; //Second carac incorrect

        } else {
            erreur = 1; //Premier carac incorrect
            if (saisie[0] != '\n' && saisie[1] != '\n' && saisie[2] != '\n')
                while ((tmp = getchar()) != '\n' && tmp != EOF);
        }

        if (erreur) {
            printf("Attention, vous n'avez droit ");
            if (erreur == 1)
                printf("qu'aux colonnes de [a] jusqu'a [%c]\n", 'a' + taille-1);
            else if (erreur == 2)
                printf("qu'aux lignes de [0] jusqu'a [%d]\n", taille-1);
        }

    } while (erreur);

    return coup;
}


Noeud* listeCoup(char **plateau, int taille, int mode, Point depart) {

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


void appliqueCoup(char **plateau, Point *depart, Point arrivee) {

    /* Le J ou R se trouvant a la case depart est copi� a la case d'arriv�e
       La valeur de la case de d�part est d�cal� de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->y][depart->x];
    plateau[depart->y][depart->x] += 32;

    *depart = arrivee;
}
