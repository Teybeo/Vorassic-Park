#include "header/affichage.h"

void affichage(char **plateau, int taille, int tour, int mode, int nbJoueurs, Joueur *tabJoueur, int joueurActuel) {

    int i;
    char modeJeu[2][10] = {"Serpent", "Pieuvre"};

    system("cls");
    printf("\nTour numero %d  -  Mode %s\n", tour, modeJeu[mode]);
    printf("Scores:");

    for (i=0;i<nbJoueurs;i++) {
        couleurs(tabJoueur[i].id);
        printf("\n    %s %d", tabJoueur[i].nom, tabJoueur[i].score);
        if (i == joueurActuel)
            printf(" <- A ton tour");
    }
    textcolor(LIGHTGRAY);

    affichePlateau(plateau, taille, mode, tabJoueur, nbJoueurs);

    textcolor(LIGHTGRAY);

}

void couleurs(int idJoueur) {

    switch (idJoueur) {
    case 100:
        textcolor(LIGHTCYAN);
        break;
    case 101:
        textcolor(LIGHTRED);
        break;
    case 102:
        textcolor(LIGHTGREEN);
        break;
    case 103:
        textcolor(LIGHTBLUE);
        break;
    }
}

void affichePlateau(char **plateau, int taille, int mode, Joueur *tabJoueur, int nbJoueurs) {

    int i, j, k;
    int tete = 0;

    printf("\n\n   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n---");

    for (i=0 ; i < taille ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        printf("%2d |", i);

        for (j=0 ; j < taille ; j++)
        {

            if (CASEVIDE(plateau[i][j]) == FAUX) { // Si un joueur est sur la case

                if (mode == SERPENT) { // En mode Serpent, on cherche la tete

                    tete = 0;
                    for (k=0;k<nbJoueurs && tete == 0;k++)
                        if (i == tabJoueur[k].position.y && j == tabJoueur[k].position.x)
                            tete = 1;
                }

                couleurs(plateau[i][j]); // On active la couleur correspondante

                if (tete == FAUX)
                    lowvideo();

                printf("%2c ", 254);

                textcolor(LIGHTGRAY);

            }

            else

                printf("%2d ", plateau[i][j]);
                //printf("   ");

        }

        printf("\n");
    }

}

void afficheDirection(Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;

    if (deltaY == -1)
        printf("Haut ");
    else if (deltaY == 1)
        printf("Bas ");

    if (deltaX == 1)
        printf("Droite");
    else if (deltaX == -1)
        printf("Gauche");

}

void resultat(Joueur *joueur, int nbJoueurs) {

    printf("\nLa partie est finie\n\n");

    int i;
    for (i=0;i<nbJoueurs;i++) {
        couleurs(joueur[i].id);
        printf("    %s   = %d\n", joueur[i].nom, joueur[i].score);
    }

    textcolor(LIGHTGRAY);

    printf("\nAppuyez sur Entree pour revenir au menu");
    getchar();

}
