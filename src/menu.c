#include "header/menu.h"

void menu() {

    int continuer = 1;
    int taille = 5, mode = 0, aleatoire = 0;
    char choix, tmp;

    do {

        system("cls");
        printf("          Vorassic Park\n\n");
        printf("    1. Lancer partie\n");
        printf("    2. Configuration\n");
        printf("    3. Quitter\n");
        printf("\n    Que voulez-vous faire ?\n ");
        choix = getchar();

        do {
            tmp = getchar();
        } while (tmp != '\n' && tmp != EOF);

        if (choix == '1')

            executePartie(taille, mode, aleatoire);

        else if (choix == '2')

            options(&taille, &mode, &aleatoire);

        else if (choix == '3')

            continuer = 0;

    } while (continuer);

}

void options(int *taille, int *mode, int *aleatoire) {

    int continuer = 1, retour;
    char choix, tampon;
    int temp = *taille;
    char modeJeu[10] = {0};
    char plateauAleatoire[10] = {0};

    do {

        if (*mode)
            strcpy(modeJeu, "Pieuvre");
        else
            strcpy(modeJeu, "Serpent");
        if (*aleatoire)
            strcpy(plateauAleatoire, "Aleatoire");
        else
            strcpy(plateauAleatoire, "Fixe");

        system("cls");
        printf("          Configuration\n\n");
        printf("    1. Changer taille plateau = %d x %d\n", *taille, *taille);
        printf("    2. Changer mode jeu = %s\n", modeJeu);
        printf("    3. Changer generation plateau = %s\n", plateauAleatoire);
        printf("    4. Retour\n");
        printf("\n    Que voulez-vous faire ?\n ");
        scanf("%c", &choix);

        if (choix != '\n')
            while ( (tampon = getchar()) != '\n' && tampon != EOF);

        if (choix == '1') {

            printf("Entrez une taille entre 5 et 26\n");

            do {

                retour = scanf("%d", &temp);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (temp >= 5 && temp <= 26)
                        *taille = temp;
                    else {
                        printf("Valeur invalide, reesayez\n");
                        retour = 0;
                    }
                }
                else {
                    printf("Erreur de saisie, entrez un nombre\n");
                }

            } while (retour != 1);

        }
        else if (choix == '2') {

            *mode = !*mode;

        }
        else if (choix == '3') {

            *aleatoire = !*aleatoire;

        }
        else if (choix == '4')

            continuer = 0;

    } while (continuer);

}
