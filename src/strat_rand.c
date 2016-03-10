#include "strat_rand.h"


struct move place(struct move const * placements, size_t sizes_placements)
{
    update_moves(placements, sizes_placements, &g);

    // randomly choose a free space
    struct move m;
    m.player = get_me(&g);
    m.src = NO_TILE;
    do {
        m.dst = rand() % get_nb_tiles(&g);
    } while((get_tile_penguin(m.dst, &g) != EMPTY) ||
	    (get_tile_nb_fish(m.dst, &g) != 1));

    return m;
}



struct move play(struct move const * moves, size_t size_moves)
{
    update_moves(moves, size_moves, &g);

    // randomly choose a penguin and a neighbour (for now)
    struct move m;
    m.player = get_me(&g);
    do {
	m.src = get_penguin_pos(m.player,
				(rand() % get_nb_penguins(&g)),
				&g);
    } while(!can_move(m.player, get_penguin_number(get_tile_penguin(m.src, &g), &g), &g));
    do {
	int dir = rand() % get_tile_nb_neighbours(m.src, &g);
	int max_distance = get_max_length(m.src, dir, &g);
	int distance = (max_distance == 0 || max_distance == 1)?max_distance:(rand() % max_distance);
        m.dst = get_landing_tile(m.src, dir, distance, &g);
    } while((m.dst == NO_TILE) 
	    || (get_tile_penguin(m.dst, &g) != EMPTY));
    return m;
}
