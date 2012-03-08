#include "header/menu.h"

void menu() {

    int continuer = 1;
    char choix, tmp;

    do {

        printf("\n1. Lancer partie\n");
        printf("2. Quitter\n");
        printf("Que voulez-vous faire ?\n");
        choix = getchar();

        do {
            tmp = getchar();
        } while (tmp != '\n');

        if (choix == '1')

            printf("\nJeu trop cool\n");

        else

            continuer = 0;

    } while (continuer);



}
