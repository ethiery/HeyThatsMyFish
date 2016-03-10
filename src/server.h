#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdlib.h>
#include "interface.h"

struct move;
struct edge;

#define EMPTY -2
// fishes proportions (3/2/1 in official game)
#define ONE_FISH 3
#define TWO_FISHES 2
#define THREE_FISHES 1

struct tile {
    size_t nb_fish;
    size_t nb_neighbours;   // initial number of adjacent tiles

    int* neighbours;     // int array of size nb_neighbours
    // giving nb of neighbours clockwise (NO_TILE if melted)
    // n and (n + nb_neighbours/2) % nb_neighbours are opposite

    int penguin;        // nb of the penguin on the tile (EMPTY if none)
                        // (NO_TILE if the tile is melted)
    // player n's mth penguin is penguin n*nb_players + m
};


struct graph {
    size_t nb_tiles;   // initial number of tiles
    struct tile *tiles;   // array of size nb_tiles, the order of tiles is constant
    size_t nb_players;
    size_t nb_penguins;// initial number of penguins per player
    int **players;       // array of array of int
};


struct strategy
{
    void *lib_handle;
    void (*init)(int,
                 size_t,
                 struct edge const*,
                 size_t,
                 int const *,
                 size_t,
                 size_t);
    struct move (*place)(struct move const*,
                         size_t);
    struct move (*play)(struct move const*,
                        size_t);
    void (*done)(void);
};


struct server {
    struct graph* g;
    struct strategy* strategies;
    int* scores;
    size_t nb_strategies;
    
};

struct server* srv;

// SETTERS, GETTERS AND FREEERS FOR DATA STRUCTURE

void set_serv(struct server *s);
void set_scores(int *scores);
void set_score(int i, int score);
void set_nb_strats(int nb_strats);
void set_strats(struct strategy* strats);
void set_handle(int i, void* h);
void set_init(int i, void* init);
void set_place(int i, void* place);
void set_play(int i, void* play);
void set_done(int i, void* done);
void set_graph(struct graph* g); 
void set_nb_tiles(int nb_tiles);
void set_tiles(struct tile *tiles);
void set_tile_nb_fish(int i, int nb_fish);
void set_tile_nb_neighbours(int i, int nb_neighbours);
void set_tile_neighbours(int i, int* neighbours);
void set_tile_penguin(int i, int penguin);
void set_tile_neighbour(int i, int j, int neighbour);
void set_nb_players(int nb_players);
void set_players(int **players);
void set_player(int i, int* player);
void set_penguin_pos(int i, int j, int penguin);
void set_nb_penguins(int nb_penguins);

struct server *get_serv(void);
int* get_scores(void);
int get_score(int i);
size_t get_nb_strats(void);
struct strategy *get_strats(void);
struct strategy *get_strat(int i);
void *get_handle(int i);
struct graph* get_graph(void);
size_t get_nb_players(void);
int** get_players(); 
int* get_player(int i);
int get_penguin_pos(int i, int j);
size_t get_nb_tiles(void);
struct tile *get_tiles(void);
struct tile *get_tile(int i);
int get_tile_nb_neighbours(int i);
int* get_tile_neighbours(int i);
int get_tile_neighbour(int i, int j);
int get_tile_nb_fish(int i);
int get_tile_penguin(int i);
size_t get_nb_penguins(void);

void free_srv(void);
void free_scores(void);
void free_strats(void);
void free_graph(void);

// returns the number of the owner of the penguin p;
int penguin_owner(int p);
//
// returns the number of p for its owner
int penguin_number(int p);



// MAIN FUNCTIONS
//
// There are 5 main functions used one after another in the main
//
// server_init initializes the data structure of the server,
// thanks to the arguments passed by the user
// returns 0 in case of error, 1 else
int server_init(int argc, char** argv);
//
// strategies_init calls the function "init" of each strategy
int strategies_init(void);
//
// place_penguins is in charge of the placement turns
// previous moves passed to each strat are stored in prev_moves
// it calls the function "place" of the strategy which it is the turn 
// to playone after another
// returns 0 in case of error, 2 if it is no longer placement turns,
// 1 else
int place_penguins(struct move *prev_moves);
//
// play_game is in charge of all the other turns
// previous moves passed to each strat are still stored in prev_moves
// it calls the function "play" of the strategy it is the turn to play
// returns 0 in case of error, 2 if it is no longer move turns 
// (the game is over), 1 else
int play_game(struct move *prev_moves); 
//
// end_game frees all strategies, removes all the penguins 
// from the board, counts scores, and declares the winner
void end_game(void);


// SERVER_INIT AUXILIARY FUNCTIONS
//
// correct_main_args checks arguments passed by the user
// returns 1 if valid, 0 else
int correct_main_args(int argc, char** argv);
// 
// server_load_strats opens the strategy libraries dynamically with DLopen
// strats is an array of string paths of those libraries
// returns 0 in case of error, 1 else
int server_load_strats(char** strats);
//
// graph_init initialized the struct graph
// with caracteristics given in arguments
// returns 0 in case of error, 1 else
int graph_init(char* layout, int width, int height, int nb_players, int penguins_by_player);
//
// The following 3 functions initialize the graph with different configurations
// they return 0 in case of error, 1 else
int init_full_hexa(int width, int height);
int init_full_tetra(int width, int height);
int init_octo_tetra(int width, int height);
//
// This function is used to draw a random number of fish
int random_fishes(void);


// STRATEGIES_INIT AUXILIARY FUNCTIONS
// 
// create_init_data browses the graph to create the data strategies need for their initialization:
// the array of edges, and the array of number of fishes
// returns 0 in case of error, 1 else
int create_init_data(struct edge** edges, size_t * size_edges, int ** fishes);
//
// free_init_data frees those data
void free_init_data(struct edge** edges, int ** fishes);



// PLACE_PENGUINS AUXILIARY FUNCTIONS
// given the move of player p at turn t
// update previous_moves
void update_previous_moves(struct move *prev_moves, struct move move, 
			   int turn, int nb_players_in_game);
//
// checks if the placement is valid 
// if yes applies it on the server and returns 1
// else returns 0
int check_n_place(int player_id, struct move* place, int turn_nb);



// PLAY_GAME AUXILIARY FUNCTIONS
//
// returns 1 if player p has no move available
// 0 else
int is_blocked(int p);
//
// checks if the move is valid 
// if yes applies it on the server and returns 1
// else returns 0
int check_n_play(int player_id, struct move* play);
//
// returns 1 if tiles dst is reachable from tile src
// <=> if src and dst are aligned and there is no hole or penguin in between 
int is_reachable(int src, int dst);
//
// disconnects the tile : removes it from the neighbours of all its neighbours
// then deprives it from neighbours
void disconnect_tile(int tile);
// 
// returns the opposite tile of tile prec for tile t
// t and prec must be neighbours
int opposite(int t, int prec);

// END_GAME AUXILIARY FUNCTIONS
//
void stategies_done(void);



// Debugging functions
//
// prints the graph
void print_tiles(void);

void print_server(struct server* s);






#endif // _SERVER_H_
