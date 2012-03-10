#include "header/partie.h"

void executePartie() {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    char coup[2];
    char posJoueur1[2] = "a0";
    char posJoueur2[2] = "e4";
    char plateau[5][5];

    initPlateau(plateau);

    do {

        printf("\nTour numero %d\n", tour);
        affichePlateau(plateau);

        joueurActuel = ((tour-1) % 2) + 1;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        saisieCoup(coup);
        if (joueurActuel == 1)
            verifieCoup(plateau, posJoueur1, coup);
        else
            verifieCoup(plateau, posJoueur2, coup);

        tour++;

    } while (continuer);

}

void verifieCoup(char plateau[5][5], char pos[2], char coup[2]) {


    int deltaX = coup[X] - pos[X];
    int deltaY = coup[Y] - pos[Y];
    int destination = plateau[(int)coup[Y]-'0'][(int)coup[X]-'a'];

    if (deltaX != 0 || deltaY != 0) { // S'il y a un deplacement non nul

        if (abs(deltaX) >= 1 && abs(deltaY) >= 1) // Si on se deplace sur les 2 axes a la fois

            printf("Coup interdit - Diagonale\n");

        else if (destination != 'J' && destination != 'R') {

            printf("Coup Valide\n");

            plateau[(int)coup[Y]-'0'][(int)coup[X]-'a'] = plateau[(int)pos[Y]-'0'][(int)pos[X]-'a'];
            pos[X] = coup[X];
            pos[Y] = coup[Y];
        }
        else
            printf("Coup interdit - Occupe\n");
    }
    else
        printf("Coup interdit - Deplacement nul\n");


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

    coup[X] = *lettre;
    coup[Y] = *nombre;

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

}
