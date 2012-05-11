#include "header/coup.h"

int faireCoupVisuel(char **plateau, int taille, int mode, Joueur *tabJoueur, int humain, int nbJoueurs) {

    Point caseArrivee = {0, 0}, caseCible, curseurTexte;
    int erreurArrivee = 1, touche, i;

    if (tabJoueur[humain].blocage == VRAI)
    {
        printf("\nVous etes bloque, vous passez votre tour");
        getchar();

    } else {

        fflush(stdin);

        printf("\nEntrez une direction : ");

        curseurTexte = (Point){wherex(), wherey()};

        caseCible = tabJoueur[humain].pion->pos;

        do {

            touche = getch();

            if (touche == 224)
                touche = getch();

            if (mode == PIEUVRE)
            {
                gotoxy(6 + (3*caseCible.x), 9+caseCible.y);
                textcolor(LIGHTGRAY);
                if (CASEVIDE(plateau[caseCible.y][caseCible.x]))
                    printf("%d", plateau[caseCible.y][caseCible.x]);
                else {
                    couleurs(plateau[caseCible.y][caseCible.x]);
                    lowvideo();
                    printf("%c", 254);
                }
            }

            switch (touche) {
            case HAUT:
                if (caseCible.y - 1 >= 0)
                    caseCible.y--;
                break;
            case GAUCHE:
                if (caseCible.x - 1 >= 0)
                    caseCible.x--;
                break;
            case DROITE:
                if (caseCible.x + 1 < taille)
                    caseCible.x++;
                break;
            case BAS:
                if (caseCible.y + 1 < taille)
                    caseCible.y++;
                break;
            case ECHAP:
                textcolor(LIGHTGRAY);
                return 1;
                break;
            }

            if (mode == SERPENT) {

                if (CASEVIDE(plateau[caseCible.y][caseCible.x])) {
                    caseArrivee = caseCible;
                    erreurArrivee = 0;
                }
                else
                    caseCible = tabJoueur[humain].pion->pos;

            } else {

                if (touche == ENTER)
                    if (EXISTE_CASE(caseCible, taille) && CASEVIDE(plateau[caseCible.y][caseCible.x]) && caseAtteignable(plateau, taille, tabJoueur[humain], caseCible)) {
                        caseArrivee = caseCible;
                        erreurArrivee = 0;
                    }

                gotoxy(6 + (3*caseCible.x), 9+caseCible.y);
                couleurs(tabJoueur[humain].id);
                putchar('x');

            }

        } while (erreurArrivee);

        gotoxy(curseurTexte.x, curseurTexte.y);
        textcolor(LIGHTGRAY);

        appliqueCoup(plateau, &tabJoueur[humain], caseArrivee, mode);

        for (i=0;i<nbJoueurs;i++)
            chercheBlocage(plateau, taille, mode, &tabJoueur[i]);


    }

    return 0;

}

/* Effectue la saisie, l'analyse et l'application d'un coup */

void faireCoupTextuel(char **plateau, int taille, int mode, Joueur *tabJoueur, int humain, int nbJoueurs) {

    Point caseDepart, caseArrivee;
    int erreurArrivee, erreurDepart;

    if (tabJoueur[humain].blocage == VRAI)
    {
        printf("\nVous etes bloque, vous passez votre tour");
        getchar();

    } else {

        printf("\nEntrez un coup : ");

        do {

            if (mode == PIEUVRE) {

                do  {

                    erreurDepart = 1;
                    caseDepart = saisieCoup(taille);

                    if (plateau[caseDepart.y][caseDepart.x] == tabJoueur[humain].id)
                        erreurDepart = 0;
                    else
                        printf("Case de depart incorrecte\n");

                } while (erreurDepart);

                caseArrivee = saisieCoup(taille);
                erreurArrivee = verifieCoup(plateau, mode, caseDepart, caseArrivee);

            } else {

                caseArrivee = saisieCoup(taille);
                erreurArrivee = verifieCoup(plateau, mode, tabJoueur[humain].pion->pos, caseArrivee);

            }

            switch (erreurArrivee) {
            case 1:
                printf("Coup interdit - Case originelle\n");
                break;
            case 2:
                printf("Coup interdit - Case occupe\n");
                break;
            case 3:
                printf("Coup interdit - Diagonale\n");
                break;
            case 4:
                printf("Coup interdit - Case hors de portee\n");
                break;
            }

        } while (erreurArrivee);

        appliqueCoup(plateau, &tabJoueur[humain], caseArrivee, mode);
        int i;
        for (i=0;i<nbJoueurs;i++)
            chercheBlocage(plateau, taille, mode, &tabJoueur[i]);

    }

}

/*  Cherche si un joueur est bloqué

    Parcours la pile des cases adjacentes à la case du joueur.
	Si la case est libre, la fonctionne s'achève immédiatement.
	Sinon on recommence sur les suivantes jusqu'à la fin de la pile
	Si aucune case libre n'a été trouvée, la variable blocage du joueur est mise à VRAI.
	En mode Pieuvre, on effectue ce meme traitement mais sur toutes les cases que le joueur possède.
*/
void chercheBlocage(char **plateau, int taille, int mode, Joueur *joueur) {

    ElemPoint *pionTemp = joueur->pion;

    if (joueur->blocage == FAUX)
    {
        joueur->blocage = VRAI;

        do {

            if (existeCoupsPossibles(plateau, taille, mode, pionTemp->pos)) { // Si on trouve au moins 1 coup, on n'est pas bloqué, on s'arrete
                joueur->blocage = FAUX;
                break;
            }

            pionTemp = pionTemp->suivant;

        } while (pionTemp != NULL);

    }

}

int existeCoupsPossibles(char **plateau, int taille, int mode, Point depart) {

    Point tab[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int i;
    Point pos;

    for (i=0; i < 4 * (1+mode); i++) {

        pos = (Point){depart.x + tab[i].x, depart.y + tab[i].y};

        if (pos.x >= 0 && pos.x < taille && pos.y >= 0 && pos.y < taille && CASEVIDE(plateau[pos.y][pos.x]))
            return VRAI;
    }

    return FAUX;
}

ElemPoint* creerPileCoupsPossibles(ElemPoint *pile, char **plateau, int taille, int mode, Point depart) {

    if (depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x])) // Haut

        pile = empiler(pile, (Point){depart.x, depart.y-1});


    if (depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x]))  // Bas

        pile = empiler(pile, (Point){depart.x, depart.y+1});


    if (depart.x > 0 && CASEVIDE(plateau[depart.y][depart.x-1])) // Gauche

        pile = empiler(pile, (Point){depart.x-1, depart.y});


    if (depart.x < taille-1 && CASEVIDE(plateau[depart.y][depart.x+1])) // Droite

        pile = empiler(pile, (Point){depart.x+1, depart.y});


    if (mode == PIEUVRE)
    {
        if (depart.x > 0 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x-1]))  // Gauche / Haut

            pile = empiler(pile, (Point){depart.x-1, depart.y-1});


        if (depart.x > 0 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x-1]))  // Gauche / Bas

            pile = empiler(pile, (Point){depart.x-1, depart.y+1});


        if (depart.x < taille-1 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x+1])) // Droite / Haut

            pile = empiler(pile, (Point){depart.x+1, depart.y-1});


        if (depart.x < taille-1 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x+1])) // Droite / Bas

            pile = empiler(pile, (Point){depart.x+1, depart.y+1});
    }


    return pile;
}

/*   Vérifie la validité d'un coup

    Analyse la distance du coup joué, la disponibilité de la case et la
    conformité avec le mode de jeu actuel
*/
int verifieCoup(char **plateau, int mode, Point depart, Point arrivee) {

    int deltaX = arrivee.x - depart.x;
    int deltaY = arrivee.y - depart.y;
    int distance = abs(deltaX) + abs(deltaY);
    int destination = plateau[arrivee.y][arrivee.x];

    if (distance != 0)  // S'il y a un deplacement non nul sur au moins 1 axe
    {
        if (mode == SERPENT && distance == 2)

            return 3; // Déplacement en diagonale

        else if (distance > 2)

            return 4; // Déplacement de 3 cases ou plus

        else if (CASEVIDE(destination))

            return 0; // Case libre

        else
            return 2; // Case occupée
    }
    else // Sinon il n'y a pas eu de déplacement
        return 1;

}

/* Applique le coup au plateau et met a jour le score et la position du joueur */

void appliqueCoup(char **plateau, Joueur *depart, Point arrivee, int mode) {

    depart->score += plateau[arrivee.y][arrivee.x];

    plateau[arrivee.y][arrivee.x] = plateau[depart->pion->pos.y][depart->pion->pos.x];

    if (mode == SERPENT)
        depart->pion->pos = arrivee;
    else
        depart->pion = empiler(depart->pion, arrivee);

}

int caseAtteignable(char **plateau, int taille, Joueur joueur, Point arrivee) {

    int i;
    Point tab[8] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    Point depart;

    for (i=0; i < 8; i++) {

        depart = (Point){arrivee.x + tab[i].x, arrivee.y + tab[i].y};

        if (EXISTE_CASE(depart, taille) && plateau[depart.y][depart.x] == joueur.id)
            return VRAI;
    }

    return FAUX;
}

/* Demande une saisie à l'utilisateur et l'analyse.
    Affiche un message indiquant si la saisie n'a pas pu etre interprete ou si elle est hors du plateau
*/
Point saisieCoup(int taille) {

    Point coup;
    int erreur = 0;
    char tmp;
    char saisie[3] = {0};
    int ligne = 0;

    do {

        erreur = 0;
        fgets(saisie, 4, stdin); // fgets rajoute '\0' au dernier element

        if (saisie[0] >= 'a' && saisie[0] <= 'z') // Si premier carac est une lettre
        {

            if (saisie[0] >= 'a' && saisie[0] <= 'a' + taille-1) // Si lettre correcte
            {

                coup.x = saisie[0] - 'a';

                if (saisie[1] >= '0' && saisie[1] <= '9') // Si second carac est un chiffre
                {

                    if (saisie[2] >= '0' && saisie[2] <= '9') // Si troisieme carac est un chiffre
                    {
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
                        erreur = 2; // Ligne hors plateau

                } else
                    erreur = 4; //Second carac incorrect

            } else
                erreur = 1; // Colonne hors plateau

        } else {

            erreur = 3; // Premier carac incorrect
            if (saisie[0] != '\n' && saisie[1] != '\n' && saisie[2] != '\n')
                while ((tmp = getchar()) != '\n' && tmp != EOF);
        }

        switch (erreur) {
            case 1:
                printf("Attention, vous n'avez droit qu'aux colonnes de [a] jusqu'a [%c]\n", 'a' + taille-1);
                break;
            case 2:
                printf("Attention, vous n'avez droit qu'aux lignes de [0] jusqu'a [%d]\n", taille-1);
                break;
            case 3:
                printf("Attention, ligne non reconnue\n");
                break;
            case 4:
                printf("Attention, colonne non reconnue\n");
                break;
        }

    } while (erreur);

    return coup;
}
