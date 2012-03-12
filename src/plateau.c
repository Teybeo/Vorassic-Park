#include "header/plateau.h"

char** initPlateau(int taille, int aleatoire) {

    int i, j;
    char **plateau;

    plateau = calloc(taille, sizeof(char*));

    for (i=0;i<taille;i++)
        plateau[i] = calloc(taille, sizeof(char));

    if (aleatoire)
    {

        srand(time(NULL));

        for (i=0 ; i < taille ; i++)

            for (j=0 ; j < taille  ; j++)

                plateau[i][j] = rand() % taille;

    }
    else
    {

        for (i=0 ; i < taille ; i++)

            for (j=0 ; j < taille - i ; j++)

                plateau[i][j] = j + i;


        for (i=taille-1 ; i > 0 ; i--)

            for (j=taille-1 ; j > taille-1 - i ; j--)

                plateau[i][j] = (taille-1)*2 - (j + i);
    }

    plateau[0][0] = 'J';
    plateau[taille-1][taille-1] = 'R';
    plateau[0][taille-1] = 'V';
    plateau[taille-1][0] = 'B';

    return plateau;

}

void affichePlateau(char **plateau, int taille) {

    int i, j;

    printf("\n   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n---");

    for (i=0 ; i < taille ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        for (j=0 ; j < taille ; j++)
        {
            if (j == 0)
                printf("%2d |", i);

            if (plateau[i][j] == 'J' || plateau[i][j] == 'R' || plateau[i][j] == 'j' || plateau[i][j] == 'r' ||
                plateau[i][j] == 'V' || plateau[i][j] == 'B' || plateau[i][j] == 'v' || plateau[i][j] == 'b')
                printf("%2c ", plateau[i][j]);
            else
                //printf("%2d ", plateau[i][j]);
                printf("   ");

        }

        printf("\n");
    }

    printf("\n");

}

