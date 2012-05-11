#include "header/menu.h"

void menu() {

    int continuer = 1;
    int taille = 6, mode = 0, prof = 5, aleatoire = 0, nbJoueurs = 2, nbBots = 1, i;
    char choix, tmp;
    char nomsDefault[4][TAILLE_NOM] = {"Cyan", "Rouge", "Vert", "Bleu"};
    char **noms;
    Config debug = {0, 0};

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

        if (choix != '\n')
            do {
                tmp = getchar();
            } while (tmp != '\n' && tmp != EOF);

        if (choix == '1')

            executePartie(nbJoueurs, nbBots, taille, mode, prof, aleatoire, noms, debug);

        else if (choix == '2')

            options(&taille, &mode, &prof, &aleatoire, &nbJoueurs, &nbBots, &noms, &debug);

        else if (choix == '3')

            continuer = 0;

    } while (continuer);

}

void options(int *taille, int *mode, int *prof, int *aleatoire, int *nbJoueurs, int *nbBots, char ***noms, Config *debug) {

    int continuer = 1, retour, i;
    int choix, tmp = *taille, *cible = NULL, nbMin, nbMax;
    char tampon;
    char chaine[TAILLE_NOM] = {0};
    char modeJeu[10] = {0}, chaineIA[4] = {0};
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

        if (*nbBots)
            strcpy(chaineIA, "Oui");
        else
            strcpy(chaineIA, "Non");

        system("cls");
        printf("          Configuration\n\n");
        printf("    1. Changer taille plateau = %d x %d\n", *taille, *taille);
        printf("    2. Mode de jeu = %s\n", modeJeu);
        printf("    3. Generation plateau = %s\n", plateauAleatoire);
        printf("    4. IA active = %s\n", chaineIA);
        printf("    5. Changer nombre de tours cherches a l'avance = %d\n", *prof);

        if (*nbBots > 0)
            printf("    6. Changer nombre de bots = %d\n", *nbBots);
        else
            printf("    6. Changer nombre de joueurs = %d\n", *nbJoueurs);

        printf("    7. Mettre en pause apres un coup de l'ia = %d\n", debug->pause);
        printf("    8. Niveau d'affichage des calculs de l'ia = %d\n", debug->vue);

        for (i=0;i<*nbJoueurs;i++)
            printf("    %d. Changer nom joueur %d = %s\n", 9+i, i+1, (*noms)[i]);

        printf("\n    0. Retour\n");
        printf("\n    Que voulez-vous faire ?\n ");
        scanf("%d", &choix);

        while ( (tampon = getchar()) != '\n' && tampon != EOF);

        switch (choix) {

        case 0:

            continuer = 0;
            break;

        case 1:

            printf("Entrez une taille entre 3 et 26\n");

            do {

                retour = scanf("%d", &tmp);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmp >= 3 && tmp <= 26)
                        *taille = tmp;
                    else {
                        printf("Valeur invalide, reesayez\n");
                        retour = 0;
                    }
                }
                else {
                    printf("Erreur de saisie, entrez un nombre\n");
                }

            } while (retour != 1);

            break;

        case 2:

            *mode = !*mode;
            break;

        case 3:

            *aleatoire = !*aleatoire;
            break;

        case 4:
                *nbJoueurs = 2;
                *nbBots = !*nbBots;
                break;
        case 5:

            printf("Entrez un nombre positif\n");

            do {

                retour = scanf("%d", &tmp);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmp > 0)
                        *prof = tmp;
                    else {
                        printf("Valeur invalide, reesayez\n");
                        retour = 0;
                    }
                }
                else {
                    printf("Erreur de saisie, entrez un nombre\n");
                }

            } while (retour != 1);

            break;

        case 6:

            if (*nbBots) {
                nbMin = NB_BOTS_MIN;
                nbMax = NB_BOTS_MAX;
                cible = nbBots;
            }
            else {
                nbMin = NB_JOUEURS_MIN;
                nbMax = NB_JOUEURS_MAX;
                cible = nbJoueurs;
            }
                printf("Entrez un nombre entre %d et %d\n", nbMin, nbMax);

            do {

                retour = scanf("%d", &tmp);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmp >= nbMin && tmp <= nbMax) {

                        *noms = realloc(*noms, sizeof(char*) * tmp);
                        if (tmp > *nbJoueurs)
                            for (i=*nbJoueurs;i<tmp;i++) {
                                (*noms)[i] = malloc(sizeof(char*) * TAILLE_NOM);
                                strcpy((*noms)[i], "Inconnu");
                            }

                        *cible = tmp;

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

            break;

        case 7:

            debug->pause = !debug->pause;
            break;

        case 8:

            printf("Entrez un nombre entre 0 et 2\n");

            do {

                retour = scanf("%d", &tmp);

                while ( (tampon = getchar()) != '\n' && tampon != EOF);

                if (retour) {
                    if (tmp >= 0 && tmp <= 2)
                        debug->vue = tmp;
                    else {
                        printf("Valeur invalide, reesayez\n");
                        retour = 0;
                    }
                }
                else {
                    printf("Erreur de saisie, entrez un nombre\n");
                }

            } while (retour != 1);

            break;

        case 9: case 10: case 11: case 12:

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

            strcpy((*noms)[choix - 9], chaine);

            break;

        default:
            break;

        }

    } while (continuer);

}
