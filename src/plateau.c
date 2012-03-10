#include "header/plateau.h"

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

