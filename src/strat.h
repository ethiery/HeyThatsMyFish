#ifndef _STRAT_H_
#define _STRAT_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "interface.h"
#include "graph.h"

#define SAME_TILE (-3)
#define NOT_NEIGHBOURS (-12)

struct graph g;

/*** communication client-serveur ***/

/* Toute stratégie doit implémenter:
   - une fonction d'initialisation 'init'
   - une fonction de placement d'un pingouin 'place'
   - une fonction de déplacement d'un pingouin 'play'
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
   - le joueur possède encore (au moins) un pingouin à placer
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

// makes the move "move" (placement or real move) in gr
void move_pawn(struct move const move, struct graph *gr);

// apply all moves of *moves
void update_moves(struct move const *moves, size_t size_moves, struct graph * gr);

// returns the next tile in the sequence [prev,current] in gr
int get_opposite_tile(int prev, int current, struct graph * gr);
int get_max_length(int tile, int dir, struct graph * gr);
int get_landing_tile(int start_tile, int dir, int distance, struct graph *gr);
int is_isolated(int tile, struct graph *gr);
int can_move(int player, int penguin_number, struct graph *gr);
int aligned_tiles(int tile_a,int tile_b, struct graph *gr);
int aligned_penguins(int penguin_a,int penguin_b, struct graph *gr);


// ------------- begin TAD moveset ---------------
struct moveSet {
    struct move * moves;
    int nb_moves; // number of slots used in the evious arrays
    int max_size; // number of slots currently allocated (>= nb_moves)
};

// if ms is NULL, it's an init
struct moveSet * moveSet__reset(struct moveSet * ms);
// add a move to moveset
void moveSet__add(struct moveSet * ms, struct move m);
// removes last move added to set
void moveSet__pop(struct moveSet * ms);
// get size of moveset
int moveSet__size(struct moveSet * ms);
// get the i-th move added to set
struct move moveSet__get(struct moveSet * ms, int i);
// free memory
void moveSet__free(struct moveSet * ms);
// ------- end TAD moveSet ------------


// -------- tests to eat all remaining fishes -----------
void explore_connex(int current, struct graph * g, int * nb_found, int * visited, int * nb_fishes);

int connex_size(int tile, struct graph * g);

void path_explore(int current_tile, struct moveSet * current_path,
		  struct graph * g, int * best_length, struct moveSet * best_ms,
		  int * visited, clock_t start_time, int max_time);
struct moveSet * eat_them_all(int player, int penguin, struct graph * g, int max_time);


#endif // _STRAT_H_
