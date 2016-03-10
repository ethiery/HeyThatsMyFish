#ifndef _TEST_STRAT_H_
#define _TEST_STRAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "strat.h"

#include "test_print.h"

int test__get_opposite_tile();
int test__get_max_length();
int test__get_landing_tile();
int test__is_isolated();
int test__can_move();
int test__aligned_penguins();

struct move build_move(int player, int src, int dst);
void sink_tile(int tile, struct graph *gr);

int init_full_tetra(int width, int height, int nb_players, int nb_penguins, struct graph *g);
int init_full_hexa(int width, int height,  int nb_players, int nb_penguins, struct graph *g); 
int init_octo_tetra(int width, int height,  int nb_players, int nb_penguins, struct graph *g); 

// SETTERS FOR DATA STRUCTURE

void set_nb_tiles(int nb_tiles, struct graph *gr);
void set_tiles(struct tile *tiles, struct graph *gr);
void set_tile_nb_fish(int tile, int nb_fish, struct graph *gr);
void set_tile_nb_neighbours(int tile, int nb_neighbours, struct graph *gr);
void set_tile_neighbours(int tile, int* neighbours, struct graph *gr);
void set_tile_penguin(int tile, int penguin, struct graph *gr);
void set_tile_neighbour(int tile, int neighbour_number, int neighbour_tile, struct graph *gr);
void set_penguin_pos(int player, int penguin_number, int tile, struct graph *gr);


#endif // _TEST_STRAT_H_
