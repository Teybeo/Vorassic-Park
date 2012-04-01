#include "header/bot.h"


void botCoup(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i, j;
    float debut, temps;
    char valeurTmp;
    int blocageTmpBot;
    int blocageTmpAdversaire;
    Point coup;
    Noeud *coups = NULL;
    Noeud *pions = NULL;
    NoeudCoupNote *coupsNotes = NULL;

    debut = clock();

    char **plateauTemp = malloc(sizeof(char*) * taille);
    for (i=0;i<taille;i++) {
        plateauTemp[i] = malloc(sizeof(char) * taille);
        for (j=0;j<taille;j++)
           plateauTemp[i][j] = plateau[i][j];
    }

    if (mode == PIEUVRE)
        pions = listeCasesAcquises(plateau, taille, bot->id);
    else
        pions = ajoutTete(pions, bot->position);

    do {

        coups = listeCoupsPossiblesBot(coups, pions->pos, plateauTemp, taille, mode);

        while (coups != NULL) {

            debugDebut(plateauTemp, taille, mode, 0, bot->position, adversaire->position, pions->pos, coups->pos, bot->id);

            effectueCoup(plateauTemp, bot, coups->pos, &valeurTmp);
            blocageTmpBot = bot->blocage;
            blocageTmpAdversaire = adversaire->blocage;

            chercheBlocageBot(plateau, taille, mode, bot, adversaire); // Si l'autre joueur était ou s'est fait bloquer, on continue solo

            if (bot->blocage == FAUX && (bot->blocage || adversaire->blocage))
                coupsNotes = ajoutTeteCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, bot, adversaire, 1, MAX));
            else
                coupsNotes = ajoutTeteCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, bot, adversaire, 1, MIN));

            annuleCoup(plateauTemp, bot, coups->pos, &valeurTmp, pions->pos);
            bot->blocage = blocageTmpBot;
            adversaire->blocage = blocageTmpAdversaire;

            coups = supprTete(coups);

            debugFin(0, coupsNotes->note);

        }

        pions = supprTete(pions);

    } while (pions != NULL);

    if (coupsNotes != NULL) {

        coup = meilleurCoup(coupsNotes);
        appliqueCoup(plateau, mode, bot, coup);
        bot->position = coup;

    } else {

        printf("\nLe bot est bloque, il passe son tour\n");
        bot->blocage = 1;
    }

    temps = (clock() - debut)/CLOCKS_PER_SEC;
    printf("Temps de calcul: %f\n", temps);

    for (i=0;i<taille;i++)
        free(plateauTemp[i]);
    free(plateauTemp);

    getchar();

}

int MinMax(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire, int prof, int etage) {

    Noeud *coups = NULL, *pions = NULL;
    NoeudNote *notes = NULL;
    Joueur *J_actuel = NULL;
    char valeurTmp;
    int blocageTmpBot;
    int blocageTmpAdversaire;

    if (etage == MIN)
        J_actuel = adversaire;
    else
        J_actuel = bot;


    chercheBlocageBot(plateau, taille, mode, bot, adversaire);

    if ((bot->blocage && adversaire->blocage) || (prof == PROF_MAX && !bot->blocage && !adversaire->blocage))  { // Si les 2 sont bloqués ou (horizon atteint et aucun joueur bloqué)

        return bot->score - adversaire->score;
    }
    else {

        #ifdef DEBUG
            printf("\n");
        #endif

        if (mode == PIEUVRE)
            pions = listeCasesAcquises(plateau, taille, J_actuel->id);
        else
            pions = ajoutTete(pions, J_actuel->position);

        do {

            coups = listeCoupsPossiblesBot(coups, pions->pos, plateau, taille, mode);

            while (coups != NULL) {

                #ifdef DEBUG
                    debugDebut(plateau, taille, mode, prof, bot->position, adversaire->position, pions->pos, coups->pos, J_actuel->id);
                #endif

                effectueCoup(plateau, J_actuel, coups->pos, &valeurTmp);
                blocageTmpBot = bot->blocage;
                blocageTmpAdversaire = adversaire->blocage;

                chercheBlocageBot(plateau, taille, mode, bot, adversaire);

                // Si le joueur actuel n'était ou ne s'est pas fait bloquer mais qu'un joueur est bloqué, il continue solo
                if (J_actuel->blocage == FAUX && (bot->blocage || adversaire->blocage))
                    notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, etage));
                else
                    notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, !etage));

                annuleCoup(plateau, J_actuel, coups->pos, &valeurTmp, pions->pos);
                bot->blocage = blocageTmpBot;
                adversaire->blocage = blocageTmpAdversaire;

                coups = supprTete(coups);

                #ifdef DEBUG
                    debugFin(prof, notes->note);
                #endif

            }

            pions = supprTete(pions);

        } while (pions != NULL);

    }

    if (etage == MAX)
        return maxNote(notes);
    else
        return minNote(notes);
}

/* Sauvegarde la valeur de la case prise
   Place la nouvelle tete sur la case prise
   Met a jour la position du joueur, désormais égale à celle de la case prise
   Met a jour le score du joueur en fonction de la case prise
*/
void effectueCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp) {

    *valeurTmp = plateau[arrivee.y][arrivee.x];

    plateau[arrivee.y][arrivee.x] = plateau[joueur->position.y][joueur->position.x];

    joueur->position = arrivee;
    joueur->score += *valeurTmp;
}

/*  Retire la valeur de la case prise au score du joueur
    Restaure les coordonnées de la tete
    Replace la tete sur l'ancienne tete
    Restaure la valeur de la case prise
*/
void annuleCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point depart) {

    joueur->score -= *valeurTmp;
    joueur->position = depart;

    plateau[joueur->position.y][joueur->position.x] = plateau[arrivee.y][arrivee.x];
    plateau[arrivee.y][arrivee.x] = *valeurTmp;

}

void chercheBlocageBot(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i;
    Joueur *joueur = bot;
    Noeud *coups = NULL;
    Noeud *pions = NULL;

    for (i=0;i<2;i++) {

        if (joueur->blocage == FAUX) {

            if (mode == PIEUVRE)
                pions = listeCasesAcquises(plateau, taille, joueur->id);
            else
                pions = ajoutTete(pions, joueur->position);

            joueur->blocage = VRAI;

            do { // Pour chaque case du joueur, on cree une liste des coups possibles

                coups = listeCoupsPossiblesBot(coups, pions->pos, plateau, taille, mode);

                if (coups != NULL) { // Si une liste existe, alors le joueur n'est pas bloqué
                    joueur->blocage = FAUX;
                    coups = libereListe(coups);
                    pions = libereListe(pions);
                    break;
                }

                pions = supprTete(pions);

            } while (pions != NULL);

        }

        joueur = adversaire;
    }

}

Noeud* listeCoupsPossiblesBot(Noeud *liste,  Point depart, char **plateau, int taille, int mode) {

    if (depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x]))  // Haut

        liste = ajoutTete(liste, (Point){depart.x, depart.y-1});


    if (depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x]))  // Bas

        liste = ajoutTete(liste, (Point){depart.x, depart.y+1});


    if (depart.x > 0 && CASEVIDE(plateau[depart.y][depart.x-1]))// Gauche

        liste = ajoutTete(liste, (Point){depart.x-1, depart.y});


    if (depart.x < taille-1 && CASEVIDE(plateau[depart.y][depart.x+1])) // Droite

        liste = ajoutTete(liste, (Point){depart.x+1, depart.y});


    if (mode == PIEUVRE)
    {
        if (depart.x > 0 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x-1]))  // Gauche / Haut

            liste = ajoutTete(liste, (Point){depart.x-1, depart.y-1});


        if (depart.x > 0 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x-1]))  // Gauche / Bas

            liste = ajoutTete(liste, (Point){depart.x-1, depart.y+1});


        if (depart.x < taille-1 && depart.y > 0 && CASEVIDE(plateau[depart.y-1][depart.x+1])) // Droite / Haut

            liste = ajoutTete(liste, (Point){depart.x+1, depart.y-1});


        if (depart.x < taille-1 && depart.y < taille-1 && CASEVIDE(plateau[depart.y+1][depart.x+1])) // Droite / Bas

            liste = ajoutTete(liste, (Point){depart.x+1, depart.y+1});
    }


    return liste;
}

int maxNote(NoeudNote *liste) {

    int maxi = EVAL_MIN;
    NoeudNote *tmp = liste;
    if (liste == NULL)
        printf("Probleme max/minNote sur liste nulle");
    while (tmp != NULL) {

        if (tmp->note > maxi)
            maxi = tmp->note;

        tmp = supprTeteNote(tmp);
    }

    return maxi;

}

int minNote(NoeudNote *liste) {

    int mini = EVAL_MAX;
    NoeudNote *tmp = liste;
    if (liste == NULL)
        printf("Probleme max/minNote sur liste nulle");
    while (tmp != NULL) {

        if (tmp->note < mini)
            mini = tmp->note;

        tmp = supprTeteNote(tmp);
    }

    return mini;

}

Point meilleurCoup(NoeudCoupNote *liste) {

    NoeudCoupNote maxi = {{0}};
    maxi.note = EVAL_MIN;

    while (liste != NULL) {

        if (liste->note >= maxi.note) {

            maxi.pos = liste->pos;
            maxi.note = liste->note;
        }

        liste = supprTeteCoupNote(liste);
    }

    return maxi.pos;

}


void affichePlateauDebug(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point actuel, Point arrivee, int id) {

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

                if (mode == SERPENT && ((i == bot.y && j == bot.x) || (i == adversaire.y && j == adversaire.x))) { // Position des tetes des joueurs

                    couleurs(plateau[i][j]);
                    printf("%2c ", 254);

                } else if (mode == PIEUVRE && (i == actuel.y && j == actuel.x)) { // Position du pion qui s'apprete a jouer

                    couleurs(plateau[actuel.y][actuel.x]);
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

void debugDebut(char **plateau, int taille, int mode, int prof, Point bot, Point adversaire, Point depart, Point arrive, int id) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    couleurs(id);
    printf("%C ", 254);
    textcolor(LIGHTGRAY);

    afficheDirection(depart, arrive);
    printf(" prof %d", prof);

    affichePlateauDebug(plateau, taille, mode, prof, bot, adversaire, depart, arrive, id);

}

void debugFin(int prof, int note) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    printf("Ce coup valait = %d\n\n", note);
}
