#include "header/bot.h"


void botCoup(char **plateau, int taille, int mode, int profMax, Joueur *bot, Joueur *adversaire) {

    float debut, temps;
    char valeurTmp;
    int blocageTmpBot;
    int blocageTmpAdversaire;
    PointNote maxActuel;
    ElemPoint *coups = NULL;
    ElemPoint *pions = NULL;
    PointNote coupNote = {{-1, -1}, 0};

    debut = clock();

    if (mode == PIEUVRE)
        pions = creerPilePions(plateau, taille, bot->id);
    else
        pions = empiler(pions, bot->position);

    maxActuel.note = EVAL_MIN;

    do {

        coups = creerPileCoupsPossibles(coups, plateau, taille, mode, pions->pos);

        while (coups != NULL) {

            #if DEBUG >= 1
                debugDebut(plateau, taille, mode, 0, profMax, bot->position, adversaire->position, pions->pos, coups->pos, bot->id);
            #endif

            effectueCoup(plateau, bot, coups->pos, &valeurTmp);
            blocageTmpBot = bot->blocage;
            blocageTmpAdversaire = adversaire->blocage;

            chercheBlocage(plateau, taille, mode, bot);
            chercheBlocage(plateau, taille, mode, adversaire); // Si l'autre joueur était ou s'est fait bloquer, on continue solo

            coupNote.pos = coups->pos;
            // Si le joueur actuel ne s'est pas fait bloquer mais qu'un joueur est bloqué, il continue solo
            if ((bot->blocage == FAUX && adversaire->blocage == VRAI) || (bot->blocage && adversaire->blocage))
                coupNote.note = AlphaBeta(plateau, taille, mode, 1, profMax, bot, adversaire, MAX, maxActuel.note, EVAL_MAX);
            else
                coupNote.note = AlphaBeta(plateau, taille, mode, 1, profMax, bot, adversaire, MIN, maxActuel.note, EVAL_MAX);

            maxActuel = maxPointNote(maxActuel, coupNote);

            annuleCoup(plateau, bot, coups->pos, &valeurTmp, pions->pos);
            bot->blocage = blocageTmpBot;
            adversaire->blocage = blocageTmpAdversaire;

            coups = depiler(coups);

            #if DEBUG >= 1
                debugFin(0, coupNote.note, 0, EVAL_MIN, EVAL_MAX, 0, NULL);
            #endif

            }

        pions = depiler(pions);

    } while (pions != NULL);

    if (coupNote.pos.x != -1 && coupNote.pos.y != -1) {

        appliqueCoup(plateau, bot, maxActuel.pos);
        bot->position = maxActuel.pos;

    } else {

        printf("\nLe bot est bloque, il passe son tour\n");
        bot->blocage = 1;
    }

    temps = (clock() - debut)/CLOCKS_PER_SEC;
    printf("Temps de calcul: %f\n", temps);

    getchar();

}

int AlphaBeta(char **plateau, int taille, int mode, int prof, int profMax, Joueur *bot, Joueur *adversaire, int etage, int maxActuel, int minActuel) {

    ElemPoint *coups = NULL, *pions = NULL;
    Joueur *J_actuel = NULL;
    char valeurTmp;
    int blocageTmpBot;
    int blocageTmpAdversaire;
    int note;

    if (etage == MIN)
        J_actuel = adversaire;
    else
        J_actuel = bot;


    chercheBlocage(plateau, taille, mode, bot);
    chercheBlocage(plateau, taille, mode, adversaire);

    if ((bot->blocage && adversaire->blocage) || (prof == profMax && !bot->blocage && !adversaire->blocage))  { // Si les 2 sont bloqués ou horizon atteint

        return bot->score - adversaire->score;
    }
    else {

        #if DEBUG == 2
            printf("\n");
        #endif

        if (mode == PIEUVRE)
            pions = creerPilePions(plateau, taille, J_actuel->id);
        else
            pions = empiler(pions, J_actuel->position);

        if (etage == MIN)
            minActuel = EVAL_MAX;

        else if (etage == MAX)
            maxActuel = EVAL_MIN;

        do {

            coups = creerPileCoupsPossibles(coups, plateau, taille, mode, pions->pos);

            while (coups != NULL) {

                #if DEBUG == 2
                    debugDebut(plateau, taille, mode, prof, profMax, bot->position, adversaire->position, pions->pos, coups->pos, J_actuel->id);
                #endif

                effectueCoup(plateau, J_actuel, coups->pos, &valeurTmp);
                blocageTmpBot = bot->blocage;
                blocageTmpAdversaire = adversaire->blocage;

                chercheBlocage(plateau, taille, mode, bot);
                chercheBlocage(plateau, taille, mode, adversaire);

                if (etage == MAX) {

                    // Si le joueur actuel ne s'est pas fait bloquer mais qu'un joueur est bloqué, il continue solo
                    if ((J_actuel->blocage == FAUX && (bot->blocage || adversaire->blocage)) || (bot->blocage && adversaire->blocage))
                        note = AlphaBeta(plateau, taille, mode, prof+1, profMax, bot, adversaire, etage, maxActuel, minActuel);
                    else
                        note = AlphaBeta(plateau, taille, mode, prof+1, profMax, bot, adversaire, !etage, maxActuel, minActuel);
                    maxActuel = max(maxActuel, note);

                } else if (etage == MIN) {

                    if ((J_actuel->blocage == FAUX && (bot->blocage || adversaire->blocage)) || (bot->blocage && adversaire->blocage))
                        note = AlphaBeta(plateau, taille, mode, prof+1, profMax, bot, adversaire, etage, maxActuel, minActuel);
                    else
                        note = AlphaBeta(plateau, taille, mode, prof+1, profMax, bot, adversaire, !etage, maxActuel, minActuel);

                    minActuel = min(minActuel, note);
                }

                annuleCoup(plateau, J_actuel, coups->pos, &valeurTmp, pions->pos);
                bot->blocage = blocageTmpBot;
                adversaire->blocage = blocageTmpAdversaire;

                coups = depiler(coups);

                #if DEBUG == 2
                    debugFin(prof, note, profMax-prof-1, maxActuel, minActuel, etage, coups);
                #endif

                if (etage == MAX && maxActuel >= minActuel) { // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
                    liberePile(coups);
                    liberePile(pions);
                    return maxActuel;
                }

                else if (etage == MIN && minActuel <= maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
                    liberePile(coups);
                    liberePile(pions);
                    return minActuel;
                }


            }

            pions = depiler(pions);

        } while (pions != NULL);

    }

    if (etage == MAX)
        return maxActuel;
    else
        return minActuel;

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

int max(int a, int b) {

     return (a > b) ? a : b;

}

int min(int a, int b) {

    return (a < b) ? a : b;

}

inline PointNote maxPointNote(PointNote a, PointNote b) {

     return (a.note > b.note) ? a : b;
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

void debugDebut(char **plateau, int taille, int mode, int prof, int profMax, Point bot, Point adversaire, Point depart, Point arrive, int id) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    couleurs(id);
    printf("%C ", 254);
    textcolor(LIGHTGRAY);

    afficheDirection(depart, arrive);
    printf(" prof %d/%d", prof, profMax-1);

    affichePlateauDebug(plateau, taille, mode, prof, bot, adversaire, depart, arrive, id);

}

void debugFin(int prof, int note, int profActuelle, int maxActuel, int minActuel, int etage, ElemPoint *coup) {

    int i;
    for (i=0;i<prof;i++)
        printf("        ");

    printf("Ce coup valait = %d\n", note);
    if (coup != NULL && profActuelle != 0) {

        if (etage == MAX && maxActuel >= minActuel) {  // Si le max actuel est déja supérieur au min actuel de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage maxActuel %d >= minActuel %d, cette branche est deja plus haute ou egale au min precedent", maxActuel, minActuel);
            getchar();
        }
        else if (etage == MIN && minActuel <= maxActuel) { // Si le min actuel est déja inférieur au max de l'étage supérieur, on coupe
            for (i=0;i<prof;i++)
                printf("        ");
            printf("Elagage minActuel %d <= maxActuel %d, cette branche est deja plus basse ou egale au sup precedent", minActuel, maxActuel);
            getchar();
        }
    }

    printf("\n");

}
