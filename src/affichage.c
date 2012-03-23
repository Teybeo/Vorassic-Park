#include "header/affichage.h"

void affichage(char **plateau, int taille, int tour, int mode, int nbJoueurs, Joueur *joueur, int joueurActuel) {

    int i;
    char modeJeu[2][10] = {"Serpent", "Pieuvre"};

    system("cls");
    printf("\nTour numero %d  -  Mode %s\n", tour, modeJeu[mode]);
    printf("Scores:");

    for (i=0;i<nbJoueurs;i++) {
        couleurs(plateau, joueur[i]);
        printf("\n    %s %d", joueur[i].nom, joueur[i].score);
        if (joueurActuel == joueur[i].id)
            printf(" <- A ton tour");
    }
    textcolor(LIGHTGRAY);

    affichePlateau(plateau, taille, mode);

    textcolor(LIGHTGRAY);

    printf("Entrez un coup : ");

}

void couleurs(char **plateau, Joueur joueur) {
    int i, j;
    j = joueur.position.x;
    i = joueur.position.y;

    if (plateau[i][j] == 'c' || plateau[i][j] == 'C')
        textcolor(LIGHTCYAN);
    if (plateau[i][j] == 'r' || plateau[i][j] == 'R')
        textcolor(LIGHTRED);
    if (plateau[i][j] == 'v' || plateau[i][j] == 'V')
        textcolor(LIGHTGREEN);
    if (plateau[i][j] == 'b' || plateau[i][j] == 'B')
        textcolor(LIGHTBLUE);

}

void affichePlateau(char **plateau, int taille, int mode) {

    int i, j;

    printf("\n\n   |");

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

            if ((plateau[i][j] == 'C' || plateau[i][j] == 'R') || plateau[i][j] == 'c' || plateau[i][j] == 'r' ||
            plateau[i][j] == 'V' || plateau[i][j] == 'B' || plateau[i][j] == 'v' || plateau[i][j] == 'b') {

                if (plateau[i][j] == 'c')
                    textcolor(CYAN);
                if (plateau[i][j] == 'C')
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
                printf("%2d ", plateau[i][j]);
                //printf("   ");

        }

        printf("\n");
    }

    printf("\n");

}

void afficheDirection(Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;

    if (deltaX == 0) {

        if (deltaY == -1)
            printf("Haut");
        else if (deltaY == 1)
            printf("Bas");
    }
    else if (deltaX == 1)
        printf("Droite");
    else if (deltaX == -1)
        printf("Gauche");


}

void resultat(Joueur *joueur, int nbJoueurs, char **plateau) {

    printf("\nLa partie est finie\n\n");

    int i;
    for (i=0;i<nbJoueurs;i++) {
        couleurs(plateau, joueur[i]);
        printf("    %s   = %d\n", joueur[i].nom, joueur[i].score);
    }

    textcolor(LIGHTGRAY);

    printf("\nAppuyez sur Entree pour revenir au menu");
    getchar();

}
