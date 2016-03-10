#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "interface.h"

#define EMPTY (-2)


struct tile {
    int nb_fish;               
    int nb_neighbours; // initial number of adjacent tiles 

    int *neighbours;  // int array of size nb_neighbours
    // giving nb of neighbours clockwise (NO_TILE if melted)
    // n and (n + nb_neighbours/2) % nb_neighbours are opposite

    int penguin;
    // nb of the penguin on the tile (EMPTY if none, NO_TILE if molten)
    // player n's mth penguin is penguin n*nb_players + m
};

struct graph {
    int nb_tiles;   // initial number of tiles
    struct tile *tiles;   // array of size nb_tiles, the order of tiles is constant
    int nb_players;       
    int nb_penguins;// initial number of penguins per player
    int **pawns;       // array of array of int
    // pawns[n][m] is the position of player n's mth penguin
    int me; // my player id
};

// INITIALIZERS AND GETTERS FOR DATA STRUCTURE
void init_pawns(int nb_players, int nb_penguins, struct graph *g);
void init_tiles(int nb_tiles, int const *fishes, struct graph *g);
void init_neighbours(int size_edges, struct edge const *edges, struct graph *g);
void init_me(unsigned player, struct graph *g);

int get_nb_players(struct graph *g);
int get_me(struct graph *g);
int get_penguin_pos(int player, int penguin_number, struct graph *g);
int get_nb_tiles(struct graph *g);
struct tile *get_tiles(struct graph *g);
struct tile *get_tile(int tile, struct graph *g);
int get_tile_nb_neighbours(int tile, struct graph *g);
int* get_tile_neighbours(int tile, struct graph *g);
int get_tile_neighbour(int tile, int neighbour, struct graph *gr);
int get_tile_nb_fish(int tile, struct graph *g);
int get_tile_penguin(int tile, struct graph *g);
int get_nb_penguins(struct graph *g);
int get_penguin_owner(int penguin_id, struct graph *g);
int get_penguin_number(int penguin_id, struct graph *g);

void free_graph(struct graph *g);

#endif // _GRAPH_H_
