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

    if (deltaX != 0 || deltaY != 0) { // S'il y a un deplacement non nul

        if (abs(deltaX) >= 1 && abs(deltaY) >= 1) // Si on se deplace sur les 2 axes a la fois

            return 3;

        else if (destination != 'J' && destination != 'R') {

            return 0;//printf("Coup Valide\n");
        }
        else
            return 2;
    }
    else
        return 1;

}

void appliqueCoup(char plateau[5][5], Point *depart, Point arrivee) {

    plateau[arrivee.y][arrivee.x] = plateau[depart->y][depart->x];
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



void initPlateau(char plateau[5][5]) {

    int i, j;

    for (i=0 ; i < 5 ; i++)

        for (j=0 ; j < 5-i ; j++)

            plateau[i][j] = j + i;


    for (i=5-1 ; i > 0 ; i--)

        for (j=5-1 ; j > 5-1-i ; j--)

            plateau[i][j] = (5-1)*2 - (j + i);

    plateau[0][0] = 'J';
    plateau[4][4] = 'R';

}

void affichePlateau(char plateau[5][5]) {

    int i, j;

    printf("\n  |");

    for (i=0 ; i < 5 ; i++)
        printf("%2c ", 'A' + i);
    printf("\n--");

    for (i=0 ; i < 5 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < 5 ; i++)
    {
        for (j=0 ; j < 5 ; j++)
        {
            if (j == 0)
                printf("%d |", i);

            if (plateau[i][j] == 'J' || plateau[i][j] == 'R')
                printf("%2c ", plateau[i][j]);
            else
                printf("%2d ", plateau[i][j]);
        }

        printf("\n");
    }

    printf("\n");

}
