#ifndef H_GENERAL
#define H_GENERAL

#define TAILLE_NOM (10+1)

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
