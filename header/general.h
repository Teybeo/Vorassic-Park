#ifndef H_GENERAL
#define H_GENERAL

/* + 1 car commence a 0
    + 1 car \0 */
#define TAILLE_NOM (10+1+1)

typedef struct Point {

    int x;
    int y;

} Point;

typedef struct Joueur {

    Point position;
    int score;
    int blocage;
    char nom[TAILLE_NOM];

} Joueur;

#endif
