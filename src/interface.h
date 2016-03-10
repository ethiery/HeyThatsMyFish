#ifndef _PENGUINS_INTERFACE_H
#define _PENGUINS_INTERFACE_H

#include <stdlib.h>

// PAD    https://mensuel.framapad.org/p/pingouins1

/*** Algo Serveur ***/

/*
créer plateau de jeu
pour chaque joueur
  init(...);
fin
pour chaque pingouin
  pour chaque joueur
    place = placer(...);
    vérifier place; // éliminer le joueur si place invalide
fin
tant que le jeu n'est pas fini
  pour chaque joueur
    coup = play(...);
    vérifier coup;  // éliminer le joueur si coup invalide
  fin
fin
proclamer vainqueur
*/



/*** Graphe ***/

/* Les clients et le serveur doivent disposer de représentations compatibles
   du plateau de jeu. Celui-ci est constitué de tuiles polygonales à nombre
   pair de côtés.
   Le plateau de jeu est représenté par un graphe dont les sommets
   correspondent aux tuiles. Les sommets sont identifiés par des nombres
   supérieurs ou égaux à 0.
   Les arêtes du graphe représentent la relation d'adjacence des tuiles.
 */

/* Arête du graphe
   src : identifiant de la tuile de départ
   dst : identifiant de la tuile de destination
*/
struct edge {
    int src;
    int dst;
};



/*** Coup ***/

/* Description d'un coup joué
   player : identifiant du joueur
   src : identifiant de la tuile de départ
   dst : identifiant de la tuile de destination
   CONVENTION:
   src=NO_TILE s'il s'agit du placement d'un pingouin sur la tuile dst
 */

#define NO_TILE (-1)

struct move {
  int player;
  int src;
  int dst;
};



/*** communication client-serveur ***/

/* Toute stratégie doit implémenter:
   - une fonction d'initialisation 'init'
   - une fonction de placement d'un pingouin 'place'
   - une fonction de déplacement d'un pingouin 'play'
   - une fonction de terminaison 'done'
*/


/* Fonction d'initialisation du jeu
   player : identifiant du joueur
   nb_tiles : nombre de tuiles dans le jeu
   edges : liste des arêtes du graphe de jeu
   size_edges : nombre d'arêtes
   fishes : associe un nombre de poissons à chaque tuile
   nb_players : nombre de joueurs
   nb_penguins_by_players : nombre de pingouins par joueur
   PRECONDITIONS:
   - l'ensemble des identifiants valides de tuiles est {0,...,nb-tiles-1}
   - l'ensemble des identifiants valides de joueurs est {0,..,nb_players-1}
   - player est un identifiant valide de joueur
   - edges est une liste d'arêtes telle que toutes les arêtes sortantes
   d'un nœud v sont données dans l'ordre chronologique et à partir d'une
   direction de référence commune à toutes les tuiles. Un nœud v représentant
   une tuile à N côtés a N arêtes sortantes. Chaque côté de la tuile qui ne
   touche pas une autre tuile est représenté par une arête (v, NO_TILE). Les
   autres côtés sont représentés par une arête (v,v') où v' est la tuile
   adjacente.
   - fishes est un tableau de taille nb_tiles à valeur >= 0
   - nb_players * nb_penguins_by_player <= nb_tiles
   NOTE:
   - donner les arêtes issues d'un nœud consécutivement permet de détecter
   les arêtes opposées
   - pour les autres fonctions, les ensembles d'identifiants valides sont
   définis à partir de ceux reçus par cette fonction
   - pour les autres fonction, l'identifiant du joueur est la valeur de player
 */
void init(unsigned player,
    size_t nb_tiles,
    struct edge const * edges,
    size_t size_edges,
    int const * fishes,
    size_t nb_players,
    size_t nb_penguins_by_player);


/* Fonction de placement d'un pingouin
   placements : liste des derniers pingouins placés
   size_placements : nombre de placements
   PRECONDITIONS:
   - placements est de taille size_placements
   - pour 0 <= i < sizes_placements, placements[i].player est un identifiant
   de joueur valide qui identifie le joueur qui place un pingouin,
   .src = NO_TILE et .dst est un identifiant de tuile valide qui identifie
   la tuile sur laquelle le pingouin est placé
   - placements contient tous les coups joués depuis le dernier appel à place,
   dans l'ordre dans lequel ils ont été joués, et incluant le dernier coup
   joué par le joueur
   - le joueur possède (au moins) qui peut être placé
   RETOURNE:
   - un coup tel .player est l'identifiant du joueur, .src=NO_TILE et .dst
   est un identifiant de tuile valide
 */
struct move place(struct move const * placements, size_t sizes_placements);


/* Fonction de jeu
   moves : liste des derniers coups joués
   size_moves : nombre de coups
   PRECONDITIONS:
   - moves est de taille size_moves
   - pour 0 <= i < size_moves, moves[i] est valide: .player est un identifiant
   valide de joueur, .src et .dst sont des identifiants valides de tuiles
   - moves contient tous les coups joués depuis le dernier appel à play,
   dans l'ordre dans lequel les coups ont été joués, et incluant le dernier
   coup joué par le joueur
   - le joueur possède (au moins) un pingouin qui peut être déplacé
   RETOURNE:
   - un coup tel que .player est l'identifiant du joueur, .src et .dst sont
   des identifiants valides de tuiles
 */
struct move play(struct move const * moves, size_t size_moves);


/* Fonction de terminaison du jeu
   NOTE:
   - cette fonction doit être appelée lorsque le jeu est terminé pour permettre
   au client de libérer les ressources allouées (mémoire, etc)
 */
void done(void);


#endif // _PENGUINS_INTERFACE_H_