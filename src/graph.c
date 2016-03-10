#include "graph.h"

void init_pawns(int nb_players, int nb_penguins, struct graph *gr) {
//    printf("%d %d\n", nb_players, nb_penguins);
    gr->nb_players = nb_players;
    gr->nb_penguins = nb_penguins;
    gr->pawns = malloc(nb_players * sizeof(int*));
    for(int i = 0 ; i < nb_players ; i++)
    {   
	gr->pawns[i] = malloc(nb_penguins * sizeof(int));
        for(int j = 0; j < nb_penguins; j++)
            gr->pawns[i][j] = NO_TILE;
    }
}

void init_tiles(int nb_tiles, int const *fishes, struct graph *gr) {
    gr->nb_tiles = nb_tiles;
    gr->tiles = malloc(nb_tiles * sizeof(struct tile));
    for(int i = 0; i < nb_tiles; i++)
    {
        gr->tiles[i].nb_fish = fishes[i];
        gr->tiles[i].penguin = EMPTY;
        gr->tiles[i].nb_neighbours = 0;
        gr->tiles[i].neighbours = NULL;
    }

}

void init_neighbours(int size_edges, struct edge const *edges, struct graph *gr) {
    // compute number of neighbours
    for(int i = 0; i < size_edges; i++)
        gr->tiles[edges[i].src].nb_neighbours += 1;

    // write neighbours
    int next_neighbour[get_nb_tiles(gr)]; // next free space in neighbour array
    for(int i = 0; i < get_nb_tiles(gr); i++)
        next_neighbour[i] = 0;

    for(int i = 0; i < size_edges; i++)
    {
	struct tile *t = &(gr->tiles[edges[i].src]);
	if(t->neighbours == NULL)
            t->neighbours = malloc(t->nb_neighbours * sizeof(int));

        t->neighbours[next_neighbour[edges[i].src]] = edges[i].dst;

        next_neighbour[edges[i].src] += 1;
    }

}

void init_me(unsigned player, struct graph *gr) {
    gr->me = player;
}

/*
 * GETTERS
 */

int get_nb_players(struct graph *gr) {
    return gr->nb_players;
}

int get_me(struct graph *gr) {
    return gr->me;
}

int get_penguin_pos(int player, int penguin_number, struct graph *gr) {
    return gr->pawns[player][penguin_number];
}

int get_nb_tiles(struct graph *gr) {
    return gr->nb_tiles;
}

struct tile *get_tiles(struct graph *gr) {
    return gr->tiles;
}

struct tile *get_tile(int tile, struct graph *gr) {
    assert(tile >= 0 && tile < get_nb_tiles(gr));
    return &(gr->tiles[tile]);
}

int get_tile_nb_neighbours(int tile, struct graph *gr) {
    assert(tile >= 0 && tile < get_nb_tiles(gr));
    return gr->tiles[tile].nb_neighbours;
}

int* get_tile_neighbours(int tile, struct graph *gr) {
    assert(tile >= 0 && tile < get_nb_tiles(gr));
    return gr->tiles[tile].neighbours;
}

int get_tile_neighbour(int tile, int neighbour, struct graph *gr) {
    assert(tile >= 0 && tile < get_nb_tiles(gr) && neighbour >= 0
	   && neighbour < get_tile_nb_neighbours(tile, gr));
    return gr->tiles[tile].neighbours[neighbour];
}

int get_tile_nb_fish(int tile, struct graph *gr) {
    assert(tile >= 0 && tile < get_nb_tiles(gr));
    return gr->tiles[tile].nb_fish;
}

int get_tile_penguin(int tile, struct graph *gr) {
    return (tile == NO_TILE)?NO_TILE:gr->tiles[tile].penguin;
}

int get_nb_penguins(struct graph *gr) {
    return gr->nb_penguins;
}

int get_penguin_owner(int penguin_id, struct graph *gr) {
    return penguin_id / gr->nb_penguins;
}

int get_penguin_number(int penguin_id, struct graph *gr) {
    return penguin_id % gr->nb_penguins;
}

void free_graph(struct graph *gr) {
    if (gr->nb_tiles && gr->tiles) {
        for (int i = 0 ; i < get_nb_tiles(gr) ; i++)
            if (get_tile_neighbours(i,gr))
		free(get_tile_neighbours(i,gr));
	free(gr->tiles);
	gr->tiles = NULL;
    }
    if (gr->nb_players && gr->pawns) {
        for (int i = 0 ; i < get_nb_players(gr) ; i++)
	    if (gr->nb_penguins && gr->pawns[i])
		free(gr->pawns[i]);
        free(gr->pawns);
	gr->pawns = NULL;    
    }
//    free(gr);
}

