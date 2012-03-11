#include "header/partie.h"

void executePartie(int taille, int mode, int aleatoire) {

    int continuer = 1;
    int tour = 1;
    int joueurActuel;
    int scoreJ1 = 0, scoreJ2 = 0;
    int i;
    Point joueur1;
    joueur1.x = 0;
    joueur1.y = 0;
    Point joueur2;
    joueur2.x = taille-1;
    joueur2.y = taille-1;
    char **plateau;
    char modeJeu[10] = {0};

    if (mode)
        strcpy(modeJeu, "Pieuvre");
    else
        strcpy(modeJeu, "Serpent");

    plateau = calloc(taille, sizeof(char*));
    for (i=0;i<taille;i++)
        plateau[i] = calloc(taille, sizeof(char));
    initPlateau(plateau, taille, aleatoire);

    do {

        system("cls");
        printf("\nTour numero %d  -  Mode %s", tour, modeJeu);
        printf("\nScores:\n     J1 %d\n     J2 %d\n", scoreJ1, scoreJ2);
        affichePlateau(plateau, taille);

        joueurActuel = ((tour-1) % 2) + 1;

        printf("C'est au tour du joueur %d\n", joueurActuel);

        if (joueurActuel == 1)
            scoreJ1 += faireCoup(plateau, taille, mode, &joueur1);
        else
            scoreJ2 += faireCoup(plateau, taille, mode, &joueur2);

        tour++;

        continuer = finPartie(joueur1, joueur2);

    } while (continuer);


    resultat(scoreJ1, scoreJ2);

    for (i=0;i<taille;i++)
        free(plateau[i]);
    free(plateau);

}

void resultat(int scoreJ1, int scoreJ2) {

    printf("\nLa partie est finie\n");

    if (scoreJ1 == scoreJ2)
        printf("Egalite a %d points\n", scoreJ1);
    else if (scoreJ1 > scoreJ2)
        printf("Le joueur 1 a gagne de %d a %d\n", scoreJ1, scoreJ2);
    else
        printf("Le joueur 2 a gagne de %d a %d\n", scoreJ2, scoreJ1);

    printf("\nAppuyez sur Entree pour revenir au menu");
    getchar();

}

int finPartie(Point joueur1, Point joueur2) {

    if (joueur1.x == -1 && joueur1.y == -1 && joueur2.x == -1 && joueur2.y == -1)
        return 0;
    else
        return 1;

}

