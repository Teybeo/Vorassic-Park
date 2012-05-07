#ifndef H_GENERAL
#define H_GENERAL

/* --- Constantes et structures utilisées par le programme --- */

#define FAUX 0
#define VRAI 1

#define SERPENT 0
#define PIEUVRE 1

#define CASEVIDE(x) \
    (x < 100 || x > 103)

/*  Taille du tableau de caractères d'un nom de joueur
    On doit rajouter 2 au nombre de caractères car le tableau doit contenir '\0' et .. ?
*/
#define TAILLE_NOM (20+1+1)


/*  Stocke 2 coordonnées entières x, y */

typedef struct Point {

    int x;
    int y;

} Point;

/*  Stocke un point et une note associée */

typedef struct PointNote {

    Point pos;
    int note;

} PointNote;

/*  Structure pour pile de points */

typedef struct ElemPoint {

    Point pos;
    struct ElemPoint *suivant;

} ElemPoint;


/*  Stocke les infos relatives à un joueur */

typedef struct Joueur {

    ElemPoint *pion; /* Les pions du joueur */
    int score; /* Score actuel */
    int blocage; /* Permet de savoir si le joueur doit passer son tour */
    int id; /* Identifiant du joueurr */
    int estBot; /* Permet de savoir si le joueur est géré par l'ia */
    char nom[TAILLE_NOM]; /* Le nom du joueur */

} Joueur;

#endif
