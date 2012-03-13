#include "header/partie.h"

void executePartie(int nbJoueurs, int taille, int mode, int aleatoire, char **noms) {

    int continuer = 1;
    int tour = 1, i;
    char **plateau;
    Joueur *joueur;
    Joueur *joueurActuel;
    char modeJeu[10] = {0};
    int nbBots = 1;

    joueur = initJoueurs(nbJoueurs, nbBots, noms, taille);

    if (mode)
        strcpy(modeJeu, "Pieuvre");
    else
        strcpy(modeJeu, "Serpent");

    plateau = initPlateau(taille, aleatoire, joueur, nbJoueurs);

    do {

        joueurActuel = &joueur[ ((tour-1) % nbJoueurs) ];

        affichage(plateau, taille, tour, modeJeu, nbJoueurs, joueur, *joueurActuel);

        if (joueurActuel->estBot)
            botCoup(plateau, taille, mode, joueurActuel);
        else
            faireCoup(plateau, taille, mode, joueurActuel);

        tour++;

        continuer = finPartie(joueur, nbJoueurs);

    } while (continuer);


    resultat(joueur, nbJoueurs, plateau);

    for (i=0;i<taille;i++)
        free(plateau[i]);
    free(plateau);

}

char** initPlateau(int taille, int aleatoire, Joueur *joueur, int nbJoueurs) {

    int i, j;
    char **plateau;

    plateau = calloc(taille, sizeof(char*));

    for (i=0;i<taille;i++)
        plateau[i] = calloc(taille, sizeof(char));

    if (aleatoire)
    {

        srand(time(NULL));

        for (i=0 ; i < taille ; i++)

            for (j=0 ; j < taille  ; j++)

                plateau[i][j] = rand() % taille;

    }
    else
    {

        for (i=0 ; i < taille ; i++)

            for (j=0 ; j < taille - i ; j++)

                plateau[i][j] = j + i;


        for (i=taille-1 ; i > 0 ; i--)

            for (j=taille-1 ; j > taille-1 - i ; j--)

                plateau[i][j] = (taille-1)*2 - (j + i);
    }

    char symbole[4] = {'C', 'R', 'V', 'B'};

    for (i=0;i<nbJoueurs;i++)
        plateau[joueur[i].position.y][joueur[i].position.x] = symbole[i];


    return plateau;

}

Joueur* initJoueurs(int nbJoueurs, int nbBots, char **noms, int taille) {

    Joueur *tab;
    tab = malloc(sizeof(Joueur) * nbJoueurs);
    int i;

    for (i=0;i<nbJoueurs;i++) {
        tab[i].score = 0;
        tab[i].blocage = 0;
        strcpy(tab[i].nom, noms[i]);
        tab[i].estBot = 0;
    }

    if (nbBots == 2) {

        tab[0].estBot = 1;
        tab[1].estBot = 1;
        strcpy(tab[0].nom,"Bot Cyan");
        strcpy(tab[1].nom,"Bot Rouge");

    } else if (nbBots == 1) {

        tab[1].estBot = 1;
        strcpy(tab[1].nom,"Bot Rouge");
    }

    tab[0].position.x = 0;
    tab[0].position.y = 0;

    tab[1].position.x = taille-1;
    tab[1].position.y = taille-1;

    if (nbJoueurs == 3) {
        tab[2].position.x = taille-1;
        tab[2].position.y = 0;
    }
    if (nbJoueurs == 4) {
        tab[2].position.x = taille-1;
        tab[2].position.y = 0;

        tab[3].position.x = 0;
        tab[3].position.y = taille-1;
    }

    return tab;

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

