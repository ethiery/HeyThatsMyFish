#include "strat.h"


void init(unsigned player,
          size_t nb_tiles,
          struct edge const * edges,
          size_t size_edges,
          int const * fishes,
          size_t nb_players,
          size_t nb_penguins_by_player)
{
    init_pawns(nb_players, nb_penguins_by_player, &g);
    init_tiles(nb_tiles, fishes, &g);
    init_neighbours(size_edges, edges, &g);
    init_me(player, &g);

    // init random generator
    srand(time(NULL));
}

void done(void) {
    printf("Libération de la mémoire par la stratégie du joueur %d \n", get_me(&g));
    free_graph(&g);
    
}

void move_pawn(struct move const move, struct graph *gr)
{
    int penguin_id = 0;
    if(move.src == NO_TILE)
    {
        // find the internal id of the pawn placed by the player
        while(get_penguin_pos(move.player, penguin_id, gr) != NO_TILE
                && penguin_id < get_nb_penguins(gr))
            penguin_id++;
        assert(penguin_id < get_nb_penguins(gr)); // if not, there are more penguins
	                                          // than said at init
	gr->tiles[move.dst].penguin = get_nb_penguins(gr) * move.player + penguin_id;
    }
    else
    {
        // compute the position of pawn in pawns[] array
        penguin_id = gr->tiles[move.src].penguin % gr->nb_penguins;

        // move it on the tiles
        // (we trust the server concerning the validity of the move)
        gr->tiles[move.dst].penguin = gr->tiles[move.src].penguin;
        gr->tiles[move.src].penguin = NO_TILE; // molten
    }
    // move it on the pawns array
    gr->pawns[move.player][penguin_id] = move.dst;
}

void update_moves(struct move const *moves, size_t size_moves, struct graph * gr)
{
    for(int i = 0; i < size_moves; i++)
    {
        move_pawn(moves[i], gr);
    }
}

int get_opposite_tile(int prev, int current, struct graph * gr)
{
    int nb_neighbours = get_tile_nb_neighbours(current, gr);
    for(int v = 0; v < nb_neighbours; v++)
	if(get_tile_neighbour(current, v, gr) == prev)
	    return get_tile_neighbour(current,
				      (v + (nb_neighbours / 2)) % nb_neighbours,
				      gr);
	else if(prev == current)
	    return SAME_TILE;

    printf("ERROR in get_opposite_tile(): %d and %d are not neighbours\n", 
	    prev, 
	    current);

    return NOT_NEIGHBOURS;
}

int get_max_length(int tile, int dir, struct graph * gr)
{
    int l = 0;
    int tmp_tile;
    int next_tile = get_tile_neighbour(tile, dir, gr);
    while(next_tile != NO_TILE 
	  && get_tile_penguin(next_tile, gr) == EMPTY)
    {
	tmp_tile = next_tile;
	next_tile = get_opposite_tile(tile, next_tile, gr);
	tile = tmp_tile;
	l++;
    }
    return l;
}

int get_landing_tile(int start_tile, int dir, int distance, struct graph *gr)
{
    int next_tile = get_tile_neighbour(start_tile, dir, gr);
    int tmp_tile;
    int i = 0;
    while(i < (distance - 1) && next_tile != NO_TILE) 
    {
	tmp_tile = next_tile;
	next_tile = get_opposite_tile(start_tile, next_tile, gr);
	start_tile = tmp_tile;
	i += 1;
    }
    return (distance == 0)?start_tile:next_tile;
}

int is_isolated(int tile, struct graph *gr)
{
    if(tile == NO_TILE)
	return 0;
    int nb_real_neighbours = 0;
    for(int i = 0 ; i < get_tile_nb_neighbours(tile, gr) ; i++)
	nb_real_neighbours += (get_tile_neighbour(tile, i, gr) == NO_TILE)?0:1; 
    return (!nb_real_neighbours)?1:0;
} 

int can_move(int player, int penguin_number, struct graph *gr)
{
    int tile = get_penguin_pos(player, penguin_number, gr);
    if (is_isolated(tile, gr))
	return 0;
    for(int i = 0 ; i < get_tile_nb_neighbours(tile, gr) ; i ++)
    {
	if(get_tile_penguin(get_tile_neighbour(tile, i, gr), gr) == EMPTY)
	    return 1;
    }
    return 0;
}

int aligned_tiles(int tile_a, int tile_b, struct graph *gr)
{
    for(int i = 0 ; i < get_tile_nb_neighbours(tile_a, gr) ; i++)
    {
	int edge = get_landing_tile(tile_a, 
				   i, 
				   get_max_length(tile_a, i, gr) + 1, 
				   gr);
	if(edge != NO_TILE
	   && edge == tile_b)
	    return 1;
    }
    return 0;
}

int aligned_penguins(int penguin_a, int penguin_b, struct graph *gr)
{

    int tile_a = get_penguin_pos(get_penguin_owner(penguin_a, gr)
				 , get_penguin_number(penguin_a, gr)
				 , gr);
    int tile_b = get_penguin_pos(get_penguin_owner(penguin_b, gr)
				 , get_penguin_number(penguin_b, gr)
				 , gr);
    return aligned_tiles(tile_a, tile_b, gr);
}


// ------------- begin TAD moveset ---------------
struct moveSet * moveSet__reset(struct moveSet * ms)
{
    // if ms is NULL, it's an init
    if(ms == NULL)
    {
	ms = malloc(sizeof(struct moveSet));
	assert(ms != NULL);
	ms->moves = malloc(sizeof(struct move));
	assert(ms->moves != NULL);
	ms->max_size = 1;
    }
    ms->nb_moves = 0;

    return ms;
}

// add a move to moveset
void moveSet__add(struct moveSet * ms, struct move m)
{
    if(ms->nb_moves == ms->max_size)
    {
	ms->moves = realloc(ms->moves, (ms->max_size+1) * sizeof(struct move));
	ms->max_size += 1;
    }
    ms->moves[ms->nb_moves] = m;
    ms->nb_moves++;
}

void moveSet__pop(struct moveSet * ms)
{
    if(ms->nb_moves > 0)
	ms->nb_moves -= 1;
}

int moveSet__size(struct moveSet * ms)
{
    return ms->nb_moves;
}

struct move moveSet__get(struct moveSet * ms, int i)
{
    assert(i >= 0 && i < ms->nb_moves);
    return ms->moves[i];
}

void moveSet__free(struct moveSet * ms)
{
    free(ms->moves);
    ms->moves = NULL;
    ms->max_size = 0;
    ms->nb_moves = 0;
}
// ------- end TAD moveSet ------------


// visited should be of size at least nb_tiles()
void explore_connex(int current, struct graph * g, int * nb_found, int * visited,int * nb_fishes)
{
    for(int v = 0; v < get_tile_nb_neighbours(current, g); v++)
    {
	if(visited[get_tile_neighbour(current, v, g)] == 0
	   && get_tile_penguin(get_tile_neighbour(current, v, g), g) == EMPTY)
	{
	    visited[get_tile_neighbour(current, v, g)] = 1;
	    *nb_found += 1;
	    *nb_fishes += get_tile_nb_fish(current, g);
	    explore_connex(get_tile_neighbour(current, v, g), g,
			   nb_found, visited, nb_fishes);
	}
    }
}

// returns the size of the connex composant
int connex_size(int tile, struct graph * g)
{
    int visited[get_nb_tiles(g)];
    for(int i = 0; i < get_nb_tiles(g); i++)
	visited[i] = 0;
    visited[tile] = 1;
    int nb_found = 1;
    int nb_fishes = 1;
    explore_connex(tile, g, &nb_found, visited, &nb_fishes);
    return nb_found;
}


// ---------- tests to eat all remaining fishes -------------

void path_explore(int current_tile, struct moveSet * current_path,
		  struct graph * g, int * best_length,
		  struct moveSet * best_ms, int * visited,
		  clock_t start_time, int max_time)
{
    int is_end = 1;
    if((float)(clock() - start_time) / CLOCKS_PER_SEC < max_time)
    {
	// explore neighbours
	for(int v = 0; v < get_tile_nb_neighbours(current_tile, g); v++)
	{
	    int neighbour = get_tile_neighbour(current_tile, v, g);
	    if(neighbour != NO_TILE && get_tile_penguin(neighbour,g) == EMPTY && visited[neighbour] == 0)
	    {
		is_end = 0;
		visited[neighbour] = 1;
		struct move m;
		m.player = get_me(g);
		m.src = current_tile;
		m.dst = neighbour;
		moveSet__add(current_path, m);
		path_explore(neighbour, current_path, g, best_length, best_ms, visited, start_time, max_time);
		moveSet__pop(current_path);
		visited[neighbour] = 0;
	    }
	}
    }

    // if no free neighbour was found, it's the end of a path
    if(is_end && moveSet__size(current_path) > *best_length)
    {
	*best_length = moveSet__size(current_path);
	moveSet__reset(best_ms);
	for(int i = 0; i < *best_length; i++)
	    moveSet__add(best_ms, moveSet__get(current_path, i));
    }
}

// find one of the longuest paths starting from penguin number 'pengin' of
// player 'player' in graph 'g'.
// 'bound' is a boundary for the length of the path, to avoid looking for
// too long paths (it takes too much time)
// the result is dynamicly allocated. Free it with moveSet__free.
struct moveSet * eat_them_all(int player, int penguin, struct graph * g, int max_time)
{
    // store if a tile was visited in the path
    int visited[get_nb_tiles(g)];
    for(int i = 0; i < get_nb_tiles(g); i++)
	visited[i] = 0;
    
    int max_length = 0; // longuest path found
    struct moveSet * best_path = moveSet__reset(NULL);
    struct moveSet * current_path = moveSet__reset(NULL);

    int starting_tile = get_penguin_pos(player, penguin, g);
    visited[starting_tile] = 1;
    clock_t start_time = clock();
    path_explore(starting_tile, current_path, g, &max_length, best_path, visited, start_time, max_time);
    moveSet__free(current_path);
    return best_path;
}



