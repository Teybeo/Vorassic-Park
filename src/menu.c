#include "header/menu.h"

void menu() {

    int continuer = 1;
    char choix, tmp;

    do {

        printf("   1. Lancer partie\n");
        printf("   2. Quitter\n");
        printf("Que voulez-vous faire ?\n ");
        choix = getchar();

        do {
            tmp = getchar();
        } while (tmp != '\n' && tmp != EOF);

        if (choix == '1') {

            executePartie();

        } else if (choix == '2')

            continuer = 0;

    } while (continuer);

}
