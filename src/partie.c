#include "header/partie.h"

void executePartie(int nbJoueurs, int taille, int mode, int aleatoire, char **noms) {

    int continuer = 1;
    int tour = 1, i;
    char **plateau;
    Joueur *joueur;
    Joueur *joueurActuel;
    char modeJeu[10] = {0};

    joueur = initJoueurs(nbJoueurs, noms, taille);

    if (mode)
        strcpy(modeJeu, "Pieuvre");
    else
        strcpy(modeJeu, "Serpent");

    plateau = initPlateau(taille, aleatoire);

    do {

        system("cls");
        printf("\nTour numero %d  -  Mode %s\n", tour, modeJeu);

        printf("Scores:\n");
        for (i=0;i<nbJoueurs;i++)
            printf("    %s %d\n", joueur[i].nom, joueur[i].score);

        affichePlateau(plateau, taille);


        joueurActuel = &joueur[ ((tour-1) % nbJoueurs) ];

        printf("C'est au tour de %s\n", joueurActuel->nom);

        faireCoup(plateau, taille, mode, joueurActuel);

        tour++;

        continuer = finPartie(joueur, nbJoueurs);

    } while (continuer);


    resultat(joueur, nbJoueurs);

    for (i=0;i<taille;i++)
        free(plateau[i]);
    free(plateau);

}

Joueur* initJoueurs(int nbJoueurs, char **noms, int taille) {

    Joueur *tab;
    tab = malloc(sizeof(Joueur) * nbJoueurs);
    int i;

    for (i=0;i<nbJoueurs;i++) {
        tab[i].score = 0;
        tab[i].blocage = 0;
        strcpy(tab[i].nom, noms[i]);
    }

    tab[0].position.x = 0;
    tab[0].position.y = 0;

    tab[1].position.x = taille-1;
    tab[1].position.y = taille-1;

    tab[2].position.x = taille-1;
    tab[2].position.y = 0;

    tab[3].position.x = 0;
    tab[3].position.y = taille-1;

    return tab;

}

void resultat(Joueur *joueur, int nbJoueurs) {

    printf("\nLa partie est finie\n\n    |");

    int i;
    for (i=0;i<nbJoueurs;i++)
        printf("    %s    |", joueur[i].nom);

    printf("\n    |");

    for (i=0;i<nbJoueurs;i++)
        printf("    %d    |", joueur[i].score);

    printf("\n\nAppuyez sur Entree pour revenir au menu");
    getchar();

}

int finPartie(Joueur *tab, int nbJoueurs) {

    int i, nbBlocages = 0;
    for (i=0;i<nbJoueurs;i++)
        nbBlocages += tab[i].blocage;

    if (nbBlocages == nbJoueurs)
        return 0;
    else
        return 1;

}

