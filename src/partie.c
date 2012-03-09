#include "header/partie.h"

void executePartie() {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    char coup[2];
    char plateau[5][5];

    initPlateau(plateau);

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


