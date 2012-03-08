#include "header/partie.h"

void executePartie() {

    printf("\nJeu trop cool\n");

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    char *actionJoueur = malloc(sizeof(char) * 6);
    //char actionJoueur[6] = ""; // => Buffer overflow
    char tmp;

    do {

        printf("\nTour numero %d\n", tour);

        if (tour % 2 == 1)

            joueurActuel = 1;

        else

            joueurActuel = 2;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        memset(actionJoueur, 0, 6);

        scanf("%s", actionJoueur);

        while (getchar() != '\n');

        printf("Vous avez joue: ");

        int i;
        for (i=0;i<5;i++)
            printf("%c |", actionJoueur[i]);

        tour++;

    } while (continuer);

}
