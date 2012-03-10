#include "header/partie.h"

void executePartie() {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    Point joueur1;
    joueur1.x = 0;
    joueur1.y = 0;
    Point joueur2;
    joueur2.x = 4;
    joueur2.y = 4;
    char plateau[5][5];

    initPlateau(plateau);

    do {

        printf("\nTour numero %d\n", tour);
        affichePlateau(plateau);

        joueurActuel = ((tour-1) % 2) + 1;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        if (joueurActuel == 1)
            faireCoup(plateau, &joueur1);
        else
            faireCoup(plateau, &joueur2);

        tour++;

        continuer = finPartie(joueur1, joueur2);

    } while (continuer);

    printf("\nLa partie est finie\n");
    score(plateau);

}

void score(char plateau[5][5]) {

    int i, j;
    int score1 = 0, score2 = 0;

    for (i=0 ; i < 5 ; i++)
        for (j=0; j < 5 ; j++) {
            if (plateau[j][i] == 'J' || plateau[j][i] == 'j')
                score1++;
            else if (plateau[j][i] == 'R' || plateau[j][i] == 'r')
                score2++;
        }
    if (score1 == score2)
        printf("Egalite a %d points\n", score1);
    else if (score1 > score2)
        printf("Le joueur 1 a gagne de %d a %d\n", score1, score2);
    else
        printf("Le joueur 2 a gagne de %d a %d\n", score2, score1);

}

int finPartie(Point joueur1, Point joueur2) {

    if (joueur1.x == -1 && joueur1.y == -1 && joueur2.x == -1 && joueur2.y == -1)
        return 0;
    else
        return 1;

}

void faireCoup(char plateau[5][5], Point *depart) {

    Point coup;
    int erreur;

    Noeud *coupPossibles = listeCoup(plateau, *depart);
    erreur = verifieBlocage(plateau, *depart, coupPossibles);

    if (!erreur) {

        do {

            coup = saisieCoup();
            erreur = verifieCoup(plateau, *depart, coup);

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

    }
    else {
        printf("Vous etes bloque, vous passez votre tour\n");
        depart->x = -1;
        depart->y = -1;
    }
}

Point saisieCoup() {

    Point coup;
    int retour;
    char tmp;
    char *lettre = calloc(1, sizeof(char));
    char *nombre = calloc(1, sizeof(char));

    retour = scanf(" %1[a-f] %1[0-4]", lettre, nombre);

    while ((tmp = getchar()) != '\n' && tmp != EOF);

    while (retour != 2) {

        printf("Reentrez votre coup car ");
        if (retour == 1)
            printf("seule la premiere variable a ete assignee\n");
        if (retour == 0)
            printf("aucune variable n'a ete assignee\n");

        *lettre = *nombre = 0;

        retour = scanf(" %1[a-f] %1[0-4]", lettre, nombre);

        while ((tmp = getchar()) != '\n' && tmp != EOF);

    }

    coup.x = *lettre - 'a';
    coup.y = *nombre - '0';

    return coup;
}

int verifieCoup(char plateau[5][5], Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int destination = plateau[arrivee.y][arrivee.x];

    if (abs(deltaX) == 1 || abs(deltaY) == 1)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (abs(deltaX) >= 1 && abs(deltaY) >= 1)

            return 3; // Déplacement en diagonale

        else if (destination != 'J' && destination != 'R' && destination != 'j' && destination != 'r')

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon le deplacement est invalide
        return 1;

}

int verifieBlocage(char plateau[5][5], Point depart, Noeud *coupPossibles) {

    if (depart.x == -1 && depart.y == -1)
        return 1;
    else {

        Noeud *tmp = coupPossibles;
        Point coup;
        while (tmp != NULL) {

            coup.x = tmp->x;
            coup.y = tmp->y;

            if (verifieCoup(plateau, depart, coup) == 0)
                return 0;

            tmp = tmp->suivant;
        }
        return 1;
    }
}

Noeud* listeCoup(char plateau[5][5], Point depart) {

    Noeud *liste = NULL;

    if (depart.y > 0) // Haut
        liste = ajoutFin(liste, depart.x, depart.y - 1);
    if (depart.y < 5-1) // Bas
        liste = ajoutFin(liste, depart.x, depart.y + 1);

    if (depart.x > 0) {
        // Gauche
        liste = ajoutFin(liste, depart.x - 1, depart.y);

        /*if (depart.y > 0) // Gauche / Haut
            liste = ajoutFin(liste, depart.x - 1, depart.y - 1);
        if (depart.y < 5-1) // Gauche / Bas
            liste = ajoutFin(liste, depart.x + 1, depart.y - 1);*/

    }
    if (depart.x < 5-1) {
        // Droite
        liste = ajoutFin(liste, depart.x + 1, depart.y);

        /*if (depart.y > 0) // Droite / Haut
            liste = ajoutFin(liste, depart.x - 1, depart.y + 1);
        if (depart.y < 5-1) // Droite / Bas
            liste = ajoutFin(liste, depart.x + 1, depart.y + 1);*/

    }

    return liste;
}

void appliqueCoup(char plateau[5][5], Point *depart, Point arrivee) {

    /* Le J ou R se trouvant a la case depart est copié a la case d'arrivée
       La valeur de la case de départ est décalé de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->y][depart->x];
    plateau[depart->y][depart->x] += 32;

    *depart = arrivee;
}




