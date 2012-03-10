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

    } while (continuer);

}

void faireCoup(char plateau[5][5], Point *depart) {

    Point coup;
    int erreur;

    do {

        coup = saisieCoup();
        erreur = verifieCoup(plateau, *depart, coup);

        switch (erreur) {
        case 1:
            printf("Coup interdit - Deplacement nul\n");
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

int verifieCoup(char plateau[5][5], Point depart, Point arrivee) {


    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int destination = plateau[arrivee.y][arrivee.x];

    if (deltaX != 0 || deltaY != 0)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (abs(deltaX) >= 1 && abs(deltaY) >= 1)

            return 3; // Déplacement en diagonale

        else if (destination != 'J' && destination != 'R' && destination != 'j' && destination != 'r')

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon le depart est le meme que l'arrivee
        return 1;

}

void appliqueCoup(char plateau[5][5], Point *depart, Point arrivee) {

    /* Le J ou R se trouvant a la case depart est copié a la case d'arrivée
       La valeur de la case de départ est décalé de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->y][depart->x];
    plateau[depart->y][depart->x] += 32;

    *depart = arrivee;
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


