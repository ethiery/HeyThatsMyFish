#include "strat_line.h"


struct move place(struct move const * placements, size_t sizes_placements)
{
    update_moves(placements, sizes_placements, &g);

    // init the movement
    struct move m;
    m.player = get_me(&g);
    m.src = NO_TILE;

    // find tiles with 1 fish
    int max_fishes = 0;
    struct moveSet * best_moves = moveSet__reset(NULL);
    struct moveSet * valid_moves = moveSet__reset(NULL);
    for(int i = 0 ; i < get_nb_tiles(&g) ; i++)
	if((get_tile_nb_fish(i, &g) == 1) && (get_tile_penguin(i, &g) == EMPTY))
	{
	    m.dst = i;
	    moveSet__add(valid_moves, m);
	    for(int j = 0 ; j < get_tile_nb_neighbours(i, &g) ; j++)
	    {
		int neighbour = get_tile_neighbour(i, j, &g); 
		if(neighbour != NO_TILE)
		{
		    if(get_tile_nb_fish(neighbour, &g) > max_fishes)
		    {
			best_moves = moveSet__reset(best_moves);
			max_fishes = get_tile_nb_fish(neighbour, &g);
		    }
		    if(get_tile_nb_fish(neighbour, &g) >= max_fishes)
		    {
			m.dst = i;
			moveSet__add(best_moves, m);
		    }
		}
	    }
	}
    // chose randomly a tile with 1 fish
    if (moveSet__size(valid_moves) == 0)
	printf("no available tiles for placing\n");
    
    m = moveSet__size(best_moves) ? 
	moveSet__get(best_moves, rand() % moveSet__size(best_moves)) : 
	moveSet__get(valid_moves, rand() % moveSet__size(valid_moves));
    moveSet__free(best_moves);
    moveSet__free(valid_moves);
    return m;
}


struct move play(struct move const * moves, size_t size_moves)
{
    update_moves(moves, size_moves, &g);

    // init the movement
    struct move m;
    m.player = get_me(&g);
    
    
    int max_fishes = 0; // max number of fishes reachable by our penguins
    struct moveSet * max_moves = moveSet__reset(NULL); // movements to get that max number of fishes

    for(int i = 0 ; i < get_nb_penguins(&g) ; i++) // for each pengin ...
	if(can_move(m.player, i, &g)) // ... which is able to move ...
	    for(int j = 0 ; j < get_tile_nb_neighbours(get_penguin_pos(m.player, i, &g), &g) ; j++) // ... for each direction ...
		for(int k = 0 ; k < get_max_length(get_penguin_pos(m.player, i, &g), j, &g) ; k++) // ... and each straight line distance
		{
		    int landing_tile = get_landing_tile(get_penguin_pos(m.player, i, &g), j, k + 1, &g);
		    if(get_tile_nb_fish(landing_tile, &g) > max_fishes)
		    {
			moveSet__reset(max_moves);
			max_fishes = get_tile_nb_fish(landing_tile, &g);
		    }
		    if(get_tile_nb_fish(landing_tile, &g) >= max_fishes)
		    {
			m.src = get_penguin_pos(m.player, i, &g);
			m.dst = landing_tile;
			moveSet__add(max_moves, m);
		    }
		}
    
    m = moveSet__get(max_moves, rand() % moveSet__size(max_moves));
    moveSet__free(max_moves);
    return m;
}
