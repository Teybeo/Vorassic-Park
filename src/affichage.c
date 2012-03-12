#include "header/affichage.h"

void affichage(char **plateau, int taille, int tour, char *modeJeu, int nbJoueurs, Joueur *joueur, Joueur joueurActuel) {

    int i;
    system("cls");
    printf("\nTour numero %d  -  Mode %s\n", tour, modeJeu);

    printf("Scores:\n");

    for (i=0;i<nbJoueurs;i++) {
        couleurs(plateau, joueur[i]);
        printf("    %s %d\n", joueur[i].nom, joueur[i].score);
    }
    textcolor(LIGHTGRAY);

    affichePlateau(plateau, taille);

    printf("C'est au tour de ");
    couleurs(plateau, joueurActuel);
    printf("%s\n", joueurActuel.nom);
    textcolor(LIGHTGRAY);

}

void couleurs(char **plateau, Joueur joueur) {
    int i, j;
    j = joueur.position.x;
    i = joueur.position.y;

    if (plateau[i][j] == 'j' || plateau[i][j] == 'J')
        textcolor(LIGHTCYAN);
    if (plateau[i][j] == 'r' || plateau[i][j] == 'R')
        textcolor(LIGHTRED);
    if (plateau[i][j] == 'v' || plateau[i][j] == 'V')
        textcolor(LIGHTGREEN);
    if (plateau[i][j] == 'b' || plateau[i][j] == 'B')
        textcolor(LIGHTBLUE);

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

            if ((plateau[i][j] == 'J' || plateau[i][j] == 'R') || plateau[i][j] == 'j' || plateau[i][j] == 'r' ||
            plateau[i][j] == 'V' || plateau[i][j] == 'B' || plateau[i][j] == 'v' || plateau[i][j] == 'b') {

                if (plateau[i][j] == 'j')
                    textcolor(CYAN);
                if (plateau[i][j] == 'J')
                    textcolor(LIGHTCYAN);
                if (plateau[i][j] == 'r')
                    textcolor(RED);
                if (plateau[i][j] == 'R')
                    textcolor(LIGHTRED);
                if (plateau[i][j] == 'v')
                    textcolor(GREEN);
                if (plateau[i][j] == 'V')
                    textcolor(LIGHTGREEN);
                if (plateau[i][j] == 'b')
                    textcolor(BLUE);
                if (plateau[i][j] == 'B')
                    textcolor(LIGHTBLUE);

                printf("%2c ", plateau[i][j]);
                textcolor(LIGHTGRAY);
            }
            else
                //printf("%2d ", plateau[i][j]);
                printf("   ");

        }

        printf("\n");
    }

    printf("\n");

}

