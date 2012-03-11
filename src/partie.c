#include "header/partie.h"

void executePartie(int taille, int mode) {

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
    initPlateau(plateau, taille);

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

void faireCoup(char **plateau, int taille, int mode, Point *depart) {

    Point coup;
    int erreur;

    Noeud *coupPossibles = listeCoup(plateau, taille, mode, *depart);
    erreur = verifieBlocage(plateau, mode, *depart, coupPossibles);

    if (!erreur) {

        do {

            coup = saisieCoup(taille);
            erreur = verifieCoup(plateau, mode, *depart, coup);

            switch (erreur) {
            case 1:
                printf("Coup interdit - Deplacement invalide\n");
                break;
            case 2:
                printf("Coup interdit - Occupe\n");
                break;
            case 3:
                printf("Coup interdit - Diagonale\n");
                break;
            }

        } while (erreur);

        appliqueCoup(plateau, depart, coup);

    }
    else {
        printf("Vous etes bloque, vous passez votre tour");
        depart->x = -1;
        depart->y = -1;
        getchar();
    }

    libereListe(coupPossibles);

}

Point saisieCoup(int taille) {

    Point coup;
    int erreur = 0;
    char tmp;
    char saisie[3] = {0};
    int ligne;

    do {

        erreur = 0;
        fgets(saisie, 4, stdin); // fgets rajoute '\0' au dernier element

        if (saisie[0] >= 'a' && saisie[0] <= 'a' + taille-1) // Si lettre correct
        {
            coup.x = saisie[0] - 'a';

            if (saisie[1] >= '0' && saisie[1] <= '9') // Si second carac est un chiffre
            {

                if (saisie[2] >= '0' && saisie[2] <= '9') { // Si troisieme carac est un chiffre
                    ligne = atoi(&saisie[1]);
                    getchar(); // On lit le \n parti dans le tampon
                }
                else if (saisie[2] == '\n')
                    ligne = saisie[1] - '0';
                else
                    getchar();

                if (ligne < taille)
                    coup.y = ligne;
                else
                    erreur = 2;

            } else
                erreur = 2; //Second carac incorrect

        } else {
            erreur = 1; //Premier carac incorrect
            if (saisie[0] != '\n' && saisie[1] != '\n' && saisie[2] != '\n')
                while ((tmp = getchar()) != '\n' && tmp != EOF);
        }

        if (erreur) {
            printf("Attention, vous n'avez droit ");
            if (erreur == 1)
                printf("qu'aux colonnes de [a] jusqu'a [%c]\n", 'a' + taille-1);
            else if (erreur == 2)
                printf("qu'aux lignes de [0] jusqu'a [%d]\n", taille-1);
        }

    } while (erreur);

    return coup;
}

int verifieCoup(char **plateau, int mode, Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int destination = plateau[arrivee.y][arrivee.x];

    if (abs(deltaX) == 1 || abs(deltaY) == 1)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == 0 && abs(deltaX) == 1 && abs(deltaY) == 1)

            return 3; // Déplacement en diagonale

        else if (destination != 'J' && destination != 'R' && destination != 'j' && destination != 'r')

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon le deplacement est invalide
        return 1;

}

int verifieBlocage(char **plateau, int mode, Point depart, Noeud *coupPossibles) {

    if (depart.x == -1 && depart.y == -1)
        return 1;
    else {

        Noeud *tmp = coupPossibles;
        Point coup;
        while (tmp != NULL) {

            coup.x = tmp->x;
            coup.y = tmp->y;

            if (verifieCoup(plateau, mode, depart, coup) == 0)
                return 0;

            tmp = tmp->suivant;
        }
        return 1;
    }
}

Noeud* listeCoup(char **plateau, int taille, int mode, Point depart) {

    Noeud *liste = NULL;

    if (depart.y > 0) // Haut
        liste = ajoutFin(liste, depart.x, depart.y - 1);
    if (depart.y < taille-1) // Bas
        liste = ajoutFin(liste, depart.x, depart.y + 1);

    if (depart.x > 0) {
        // Gauche
        liste = ajoutFin(liste, depart.x - 1, depart.y);

        if (mode && depart.y > 0) // Gauche / Haut
            liste = ajoutFin(liste, depart.x - 1, depart.y - 1);
        if (mode && depart.y < taille-1) // Gauche / Bas
            liste = ajoutFin(liste, depart.x - 1, depart.y + 1);

    }
    if (depart.x < taille-1) {
        // Droite
        liste = ajoutFin(liste, depart.x + 1, depart.y);

        if (mode && depart.y > 0) // Droite / Haut
            liste = ajoutFin(liste, depart.x + 1, depart.y - 1);
        if (mode && depart.y < taille-1) // Droite / Bas
            liste = ajoutFin(liste, depart.x + 1, depart.y + 1);

    }

    return liste;
}

void appliqueCoup(char **plateau, Point *depart, Point arrivee) {

    /* Le J ou R se trouvant a la case depart est copié a la case d'arrivée
       La valeur de la case de départ est décalé de 32 pour le passer en minuscule */
    plateau[arrivee.y][arrivee.x] = plateau[depart->y][depart->x];
    plateau[depart->y][depart->x] += 32;

    *depart = arrivee;
}




