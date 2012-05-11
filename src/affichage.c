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
        if (i == joueurActuel && !tabJoueur[i].blocage)
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
                        if (i == tabJoueur[k].pion->pos.y && j == tabJoueur[k].pion->pos.x)
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

void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adv, Point actuel, Point arrivee, int id) {

    int i, j;
    int nb_espaces = 8;
    char *decalage = malloc(sizeof(char) * 1 + (nb_espaces * prof) );
    memset(decalage, ' ', sizeof(char) * 1 + (nb_espaces * prof) );
    decalage[nb_espaces*prof] = '\0';

    printf("\n");

    printf("%s", decalage);
    printf("   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n");

    printf("%s", decalage);

    for (i=0 ; i < taille+1 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        printf("%s", decalage);

        for (j=0 ; j < taille ; j++)
        {
            if (j == 0)
                printf("%2d |", i);


            if (CASEVIDE(plateau[i][j]) == FAUX) {

                if (mode == PIEUVRE && (i == actuel.y && j == actuel.x)) { // Position du pion qui s'apprete a jouer

                    couleurs(plateau[actuel.y][actuel.x]);
                    printf("%2c ", 254);

                } else if ((i == bot.y && j == bot.x) || (i == adv.y && j == adv.x)) { // Position des tetes des joueurs

                    couleurs(plateau[i][j]);
                    printf("%2c ", 254);

                } else { // Anciennes positions

                    couleurs(plateau[i][j]);
                    lowvideo();
                    printf("%2c ", 254);
                }

            } else if (i == arrivee.y && j == arrivee.x) { // Position d'arrivée

                couleurs(id);
                printf("%2c ", 'x');

            } else
                printf("%2d ", plateau[i][j]);
                //printf("   ");

            textcolor(LIGHTGRAY);
        }

        printf("\n");
    }

    free(decalage);
}

void debugDebut(char **plateau, int taille, int mode, int prof, int profMax, Point bot, Point adv, Point depart, Point arrive, int id) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    couleurs(id);
    printf("%C ", 254);
    textcolor(LIGHTGRAY);

    afficheDirection(depart, arrive);
    printf(" prof %d/%d", prof, profMax-1);

    affichePlateauDebug(plateau, taille, mode, prof, bot, adv, depart, arrive, id);

}

void debugFin(int prof, int note, int profMax, int maxActuel, int minActuel, int etage, ElemPoint *coup) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    printf("Ce coup valait = %d\n", note);
    if (coup->suivant != NULL && prof != profMax-1) {

        if (etage == MAX && max(maxActuel, note) >= minActuel) {  // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage maxActuel %d >= minActuel %d, cette branche est deja plus haute ou egale au min precedent", max(maxActuel, note), minActuel);
            getchar();
        }
        else if (etage == MIN && min(minActuel, note) < maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage minActuel %d < maxActuel %d, cette branche est deja plus basse ou egale au sup precedent", min(minActuel, note), maxActuel);
            getchar();
        }
    }

    printf("\n");

}
