#include "header/partie.h"

void executePartie() {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    char coup[2];
    char plateau[5][5];

    initPlateau(plateau);
    affichePlateau(plateau);

    do {

        printf("\nTour numero %d\n", tour);

        joueurActuel = ((tour-1) % 2) + 1;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        saisieCoup(coup);

        tour++;

    } while (continuer);

}

void saisieCoup(char coup[2]) {

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
        printf("%2d ", i);
    printf("\n--");

    for (i=0 ; i < 5 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < 5 ; i++)
    {
        for (j=0 ; j < 5 ; j++)
        {
            if (j == 0)
                printf("%c |", 'A' + i);

            if (plateau[i][j] == 'J' || plateau[i][j] == 'R')
                printf("%2c ", plateau[i][j]);
            else
                printf("%2d ", plateau[i][j]);
        }

        printf("\n");
    }

}
