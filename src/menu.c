#include "header/menu.h"

void menu() {

    int continuer = 1;
    int taille = 4, mode = 0, aleatoire = 0, nbJoueurs = 2, i;
    char choix, tmp;
    char nomsDefault[4][TAILLE_NOM] = {"Cyan", "Rouge", "Vert", "Bleu"};
    char **noms;

    noms = calloc(sizeof(char*), nbJoueurs);
    for (i=0;i<nbJoueurs;i++) {
        noms[i] = calloc(sizeof(char), TAILLE_NOM);
        strcpy(noms[i], nomsDefault[i]);
    }


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

            executePartie(nbJoueurs, taille, mode, aleatoire, noms);

        else if (choix == '2')

            options(&taille, &mode, &aleatoire, &nbJoueurs, &noms);

        else if (choix == '3')

            continuer = 0;

    } while (continuer);

}

void options(int *taille, int *mode, int *aleatoire, int *nbJoueurs, char ***noms) {

    int continuer = 1, retour, i;
    int tmpTaille = *taille;
    char choix, tampon;
    char chaine[TAILLE_NOM] = {0};
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
        printf("    4. Changer nombre de joueurs = %d\n", *nbJoueurs);
        for (i=0;i<*nbJoueurs;i++)
            printf("    %d. Changer nom joueur %d = %s\n", 5+i, i+1, (*noms)[i]);
        printf("\n    0. Retour\n");
        printf("\n    Que voulez-vous faire ?\n ");
        scanf("%c", &choix);

        if (choix != '\n')
            while ( (tampon = getchar()) != '\n' && tampon != EOF);

        if (choix == '1') {

            printf("Entrez une taille entre 3 et 26\n");

            do {

                retour = scanf("%d", &tmpTaille);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmpTaille >= 3 && tmpTaille <= 26)
                        *taille = tmpTaille;
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
        else if (choix == '4') {

            printf("Entrez un nombre entre 2 et 4\n");

            do {

                retour = scanf("%d", &tmpTaille);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmpTaille >= 2 && tmpTaille <= 10) {

                        *noms = realloc(*noms, sizeof(char*) * tmpTaille);
                        if (tmpTaille > *nbJoueurs)
                            for (i=*nbJoueurs;i<tmpTaille;i++) {
                                (*noms)[i] = malloc(sizeof(char*) * TAILLE_NOM);
                                strcpy((*noms)[i], "Inconnu");
                            }

                        *nbJoueurs = tmpTaille;

                    }
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
        else if (choix >= '5' && choix < '5' + *nbJoueurs) {

            printf("Entrez un nom de %d lettres max\n", TAILLE_NOM-2);

            do {

                fgets(chaine, TAILLE_NOM, stdin);

                char * retourChariot = strchr(chaine, '\n'); // On cherche le '\n'

                if (retourChariot != NULL) { // S'il est présent, on le vire et on valide la chaine
                    *retourChariot = '\0';
                    break;
                }
                else { // Sinon c'est qu'on n'a pas lu tous les éléments du flux, on vide le tampon et on recommence

                    printf("Attention, plus de %d lettres ont etes rentrees\n", TAILLE_NOM-2);
                    while ( (tampon = getchar()) != '\n' && tampon != EOF);

                }

            } while (1);

            strcpy((*noms)[choix - '5'], chaine);

        }
        else if (choix == '0')

            continuer = 0;

    } while (continuer);

}
