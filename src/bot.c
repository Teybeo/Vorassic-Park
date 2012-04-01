#include "header/bot.h"


void botCoup(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i, j;
    float debut, temps;
    char valeurTmp;
    Point coup, posTmp;
    Noeud *coups = NULL;
    NoeudCoupNote *coupsNotes = NULL;

    debut = clock();

    char **plateauTemp = malloc(sizeof(char*) * taille);
    for (i=0;i<taille;i++) {
        plateauTemp[i] = malloc(sizeof(char) * taille);
        for (j=0;j<taille;j++)
           plateauTemp[i][j] = plateau[i][j];
    }

    coups = listeCasesBot(coups, bot->position, plateauTemp, taille, mode);

    while (coups != NULL) {

        textcolor(LIGHTRED);
        printf("\nR ");
        textcolor(LIGHTGRAY);
        afficheDirection(bot->position, coups->pos);
        printf(" prof 0");
        affichePlateauDebug(plateauTemp, taille, mode, 0, bot->position, adversaire->position, bot->position, coups->pos, bot->id);

        effectueCoup(plateauTemp, bot, coups->pos, &valeurTmp, &posTmp);

        coupsNotes = ajoutTeteCoupNote(coupsNotes, coups->pos, MinMax(plateauTemp, taille, mode, bot, adversaire, 1, MIN));

        annuleCoup(plateauTemp, bot, coups->pos, &valeurTmp, &posTmp);

        printf("\nCe coup valait = %d\n", coupsNotes->note);

        coups = supprTete(coups);
    }

    if (coupsNotes != NULL) {

        coup = meilleurCoup(coupsNotes);
        if (verifieCoup(plateau, mode, bot->position, coup) == 0) {
            appliqueCoup(plateau, mode, bot, coup);
            bot->position = coup;
        }

    } else {

        printf("\nLe bot est bloque, il passe son tour");
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

    Noeud *coups = NULL, *casesAcquises = NULL;
    NoeudNote *notes = NULL;
    Joueur *J_actuel = NULL;
    Point posTmp;
    int blocage;
    char valeurTmp;

    if (etage == MIN)
        J_actuel = adversaire;
    else
        J_actuel = bot;


    blocage = blocageJoueur(plateau, taille, mode, bot, adversaire);

    if (blocage == 2 || (prof == PROF_MAX && blocage == -1))  { // Si les 2 sont bloqués ou (horizon atteint et aucun joueur bloqué)

        return bot->score - adversaire->score;
    }
    else {

        #ifdef DEBUG
            printf("\n");
        #endif

        if (mode == PIEUVRE)
            casesAcquises = listeCasesAcquises(plateau, taille, J_actuel->id);
        else
            casesAcquises = ajoutTete(casesAcquises, J_actuel->position);

        do {

            coups = listeCasesBot(coups, casesAcquises->pos, plateau, taille, mode);

            while (coups != NULL) {

                 #ifdef DEBUG
                    int i;
                    for (i=0;i<prof;i++)
                        printf("        ");
                    if (etage == MAX) {
                        textcolor(LIGHTRED);printf("%c ", 'R');
                    }else {
                        textcolor(LIGHTCYAN);printf("%C ", 'C');
                    }
                    textcolor(LIGHTGRAY);afficheDirection(casesAcquises->pos, coups->pos);
                    printf(" prof %d", prof);
                    affichePlateauDebug(plateau, taille, mode, prof, bot->position, adversaire->position, casesAcquises->pos, coups->pos, J_actuel->id);
                #endif

                effectueCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posTmp);

                //Si l'autre joueur était ou s'est fait bloquer, on continue solo
                if (blocageJoueur(plateau, taille, mode, bot, adversaire) == !etage )
                    notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, etage));
                else
                    notes = ajoutTeteNote(notes, MinMax(plateau, taille, mode, bot, adversaire, prof+1, !etage));

                annuleCoup(plateau, J_actuel, coups->pos, &valeurTmp, &posTmp);

                coups = supprTete(coups);

                #ifdef DEBUG
                    for (i=0;i<prof;i++)
                        printf("        ");
                    printf("Ce coup valait = %d\n\n", notes->note);
                #endif

            }

            casesAcquises = supprTete(casesAcquises);

        }while (casesAcquises != NULL);

    }

    if (etage == MAX)
        return maxNote(notes);
    else
        return minNote(notes);
}

/* Stocke la valeur de la case prise
   Stocke la position de la tete
   Place la tete sur la case prise
   Passe la valeur de l'ancienne tete en minuscule
   Met a jour la position du joueur, désormais égale à celle de la case prise
   Met a jour le score du joueur en fonction de la case prise*/
void effectueCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp) {

    *valeurTmp = plateau[arrivee.y][arrivee.x];
    *posTmp = joueur->position;

    plateau[arrivee.y][arrivee.x] = plateau[joueur->position.y][joueur->position.x];

    joueur->position = arrivee;
    joueur->score += *valeurTmp;
}

/*  Restaure les coordonnées de la tete
    Replace la tete sur l'ancienne tete
    Restaure la valeur de la case prise
    Restaure le score du joueur avant la prise de la case */
void annuleCoup(char **plateau, Joueur *joueur, Point arrivee, char *valeurTmp, Point *posTmp) {

    joueur->position = *posTmp;

    plateau[joueur->position.y][joueur->position.x] = plateau[arrivee.y][arrivee.x];
    plateau[arrivee.y][arrivee.x] = *valeurTmp;

    joueur->score -= *valeurTmp;
}

int blocageJoueur(char **plateau, int taille, int mode, Joueur *bot, Joueur *adversaire) {

    int i, blocage[2] = {VRAI, VRAI};
    Joueur *joueur = bot;
    Noeud *coups = NULL;
    Noeud *casesAcquises = NULL;

    for (i=0;i<2;i++) {

        if (mode == PIEUVRE)
            casesAcquises = listeCasesAcquises(plateau, taille, joueur->id);
        else
            casesAcquises = ajoutTete(casesAcquises, joueur->position);


        do { // Pour chaque case du joueur, on cree une liste des coups possibles

            coups = listeCasesBot(coups, casesAcquises->pos, plateau, taille, mode);

            if (coups != NULL) { // Si une liste existe, alors le joueur n'est pas bloqué
                blocage[joueur->id-100] = FAUX;
                coups = libereListe(coups);
                casesAcquises = libereListe(casesAcquises);
                break;
            }

            casesAcquises = supprTete(casesAcquises);

        } while (casesAcquises != NULL);

        joueur = adversaire;
    }

    if (blocage[100-100] && blocage[101-100])

       return 2; // Les 2 joueurs sont bloqués

    else if (blocage[101-100])

        return MAX; // Si le bot ne peut plus joueur

    else if (blocage[100-100])

        return MIN; // Idem pour l'adversaire

    else

        return -1; // Il reste des pions a placer

}

Noeud* listeCasesBot(Noeud *liste,  Point depart, char **plateau, int taille, int mode) {

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

    int i, j, k;
    printf("\n");
    for (i=0;i<prof;i++)
        printf("        ");
    printf("   |");

    for (i=0 ; i < taille ; i++)
        printf("%2c ", 'A' + i);
    printf("\n");

    for (i=0;i<prof;i++)
        printf("        ");

    for (i=0 ; i < taille+1 ; i++)
        printf("---");
    printf("\n");

    for (i=0 ; i < taille ; i++)
    {
        for (k=0;k<prof;k++)
            printf("        ");

        for (j=0 ; j < taille ; j++)
        {
            if (j == 0)
                printf("%2d |", i);


            if (CASEVIDE(plateau[i][j]) == FAUX) {

                if (mode == SERPENT && ((i == bot.y && j == bot.x) || (i == adversaire.y && j == adversaire.x))) { // Position des tetes des joueurs

                    couleurs(plateau, plateau[i][j]);
                    highvideo();
                    printf("%2c ", 254);

                } else if (mode == PIEUVRE && (i == actuel.y && j == actuel.x)) { // Position du pion qui s'apprete a jouer

                    couleurs(plateau, plateau[actuel.y][actuel.x]);
                    highvideo();
                    printf("%2c ", 254);

                } else { // Anciennes positions

                    couleurs(plateau, plateau[i][j]);

                    printf("%2c ", 254);
                }

            } else if (i == arrivee.y && j == arrivee.x) { // Position d'arrivée

                couleurs(plateau, id);
                highvideo();

                printf("%2c ", 'x');
            }
            else
                printf("%2d ", plateau[i][j]);
                //printf("   ");

                textcolor(LIGHTGRAY);
                textbackground(BLACK);

        }

        printf("\n");
    }

}
