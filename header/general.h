#ifndef H_GENERAL
#define H_GENERAL

/** \file general.h
 *  \brief Constantes et structures utilisées par le programme
 */

/** \def TAILLE_NOM
 *
 * \brief Taille du tableau de caractères d'un nom de joueur
 *
 *  On doit rajouter 2 au nombre de caractères car le tableau doit contenir '\0' et .. ?
 *
*/
#define TAILLE_NOM (10+1+1)

/** \struct Point
 *
 * \brief Stocke 2 coordonnées entières x, y
 *
 */
typedef struct Point {

    int x; /**< Colonne */
    int y; /**< Ligne */

} Point;

/** \struct Joueur
 *
 * \brief Stocke les infos relatives à un joueur
 *
 */
typedef struct Joueur {

    Point position; /**< Position actuelle */
    int score; /**< Score actuel */
    int blocage; /**< Permet de savoir si le joueur doit passer son tour */
    int estBot; /**< Permet de savoir si le joueur est géré par l'ia */
    char nom[TAILLE_NOM]; /**< Le nom du joueur */

} Joueur;

#endif
