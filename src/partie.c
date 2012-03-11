#include "header/partie.h"

void executePartie(int taille, int mode, int aleatoire) {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    Point joueur1;
    joueur1.x = 0;
    joueur1.y = 0;
    Point joueur2;
    joueur2.x = taille-1;
    joueur2.y = taille-1;
    char **plateau;
    int i;

    plateau = calloc(taille, sizeof(char*));
    for (i=0;i<taille;i++)
        plateau[i] = calloc(taille, sizeof(char));
    initPlateau(plateau, taille, aleatoire);

    do {

        system("cls");
        printf("\nTour numero %d\n", tour);
        affichePlateau(plateau, taille);

        joueurActuel = ((tour-1) % 2) + 1;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        if (joueurActuel == 1)
            faireCoup(plateau, taille, mode, &joueur1);
        else
            faireCoup(plateau, taille, mode, &joueur2);

        tour++;

        continuer = finPartie(joueur1, joueur2);

    } while (continuer);

    printf("\nLa partie est finie\n");
    score(plateau, taille);

    for (i=0;i<taille;i++)
        free(plateau[i]);
    free(plateau);

}

void score(char **plateau, int taille) {

    int i, j;
    int score1 = 0, score2 = 0;

    for (i=0 ; i < taille ; i++)
        for (j=0; j < taille ; j++) {
            if (plateau[j][i] == 'J' || plateau[j][i] == 'j')
                score1++;
            else if (plateau[j][i] == 'R' || plateau[j][i] == 'r')
                score2++;
        }
    if (score1 == score2)
        printf("Egalite a %d points\n", score1);
    else if (score1 > score2)
        printf("Le joueur 1 a gagne de %d a %d\n", score1, score2);
    else
        printf("Le joueur 2 a gagne de %d a %d\n", score2, score1);

    printf("\nAppuyez sur Entree pour revenir au menu");
    getchar();

}

int finPartie(Point joueur1, Point joueur2) {

    if (joueur1.x == -1 && joueur1.y == -1 && joueur2.x == -1 && joueur2.y == -1)
        return 0;
    else
        return 1;

}

