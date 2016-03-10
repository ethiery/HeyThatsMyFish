#include "strat_diss.h"

// ABOUT GRAPH TRANSFORMATION
//
// the game graph is undirected but a network must be
// it involves a first transformation : 
// replacing each edge by 2 arcs going one way, and the other
//
// ford fulkerson algorithm's result is a set of arcs
// in the game Penguins, player's withdraw vertices, not arcs
// it involves transforming the max flow/ min cut problem
// in a max vertices flow / min vertices cut
// where each vertex have a capacity of 1
// to do so, each vertex v is split in two vertices v_in and v_out
// all arcs that were directed on v are directed on v_in
// all arcs that went out of v go out from v_out
// those arcs have an infinite capacity (represented by INT_MAX)
// one arc of capacity 1 goes from v_in to v_out
// IN vertices are pair vertices, OUT vertices are odd vertices
//
// finally, each arc obtained at this point must be doubled
// by an arc of capacity 0, going in the inverse way
// so that the network can be transformed by ford_fulkerson function
// in the residual network for the maximum flow
//
// if |V| and |E| are the initial numbers of vertices and edges
// the obtained network has 2|V| vertices and 2*(2|E|+|V|) arcs
//
// the src of the network is the OUT-vertex of the src in the graph
// the dst of the network is the IN-vertex of the dst in the graph
//
// if v is the k-th vertex of the graph
// v_in is the 2k-h vertex of the network
// and v_out is the 2k+1-th vertex of the network  



struct network *graph_to_network (struct graph* g, int src, int dst) 
{
    
    struct network *n = malloc(sizeof(*n));

    // setting src, dst and number of vertices
    set_net_nb_vertices(n, 2*get_nb_tiles(g));
    set_net_src(n, 2*src+1);
    set_net_dst(n, 2*dst);

    // allocating and setting vertices
    set_net_vertices(n, malloc(net_nb_vertices(n) * sizeof(int)));
    for (int i = 0 ; i < net_nb_vertices(n) ; i++) 
	set_net_vertex(n, i, i);

    // allocating and setting degrees by counting non melted neighbours
    set_net_degrees(n, malloc(net_nb_vertices(n) * sizeof(int)));
    for (int i = 0 ; i < get_nb_tiles(g) ; i++) 
    {
	int nb_non_melted = 0;
	for (int j = 0 ; j < get_tile_nb_neighbours(i, g) ; j++)
	    if (get_tile_neighbours(i, g)[j] != NO_TILE)
		nb_non_melted++;
	set_net_degree(n, 2*i, nb_non_melted+1);
	set_net_degree(n, 2*i+1, nb_non_melted+1);
    }
	   
    // allocating and setting arcs
    set_net_arcs(n, malloc(net_nb_vertices(n) * sizeof(struct w_arc*)));
    for (int i = 0 ; i < get_nb_tiles(g) ; i++)
    {
	// allocating and setting arcs out of IN-vertices
	set_net_arcs_array(n, 2*i, 
			    malloc(net_degree(n, 2*i)*sizeof(struct w_arc)));
	
	set_arc_src(net_arc(n, 2*i, 0), 2*i);
	set_arc_dst(net_arc(n, 2*i, 0), 2*i+1);
	set_arc_weight(net_arc(n, 2*i, 0), 1);
	int count = 0;
	for (int j = 1 ; j < net_degree(n, i) ; j++)
	{
	    while (get_tile_neighbours(i, g)[count] == NO_TILE)
		count++;
	    int dst2 = 2 * get_tile_neighbours(i, g)[count] + 1;
	    set_arc_src(net_arc(n, 2*i, j), 2*i);
	    set_arc_dst(net_arc(n, 2*i, j), dst2);
	    set_arc_weight(net_arc(n, 2*i, j), 0);
	    count++;
	}

	// allocating and setting arcs out of OUT-vertices 
	set_net_arcs_array(n, 2*i+1, 
			    malloc(net_degree(n, 2*i+1)*sizeof(struct w_arc)));
	set_arc_src(net_arc(n, 2*i+1, 0), 2*i+1);
	set_arc_dst(net_arc(n, 2*i+1, 0), 2*i);
	set_arc_weight(net_arc(n, 2*i+1, 0), 0);
	count = 0;
	for (int j = 1 ; j < net_degree(n, i) ; j++)
	{
	    while (get_tile_neighbours(i, g)[count] == NO_TILE)
		count++;		
	    int dst2 = 2 * get_tile_neighbours(i, g)[count];
	    set_arc_src(net_arc(n, 2*i+1, j), 2*i+1);
	    set_arc_dst(net_arc(n, 2*i+1, j), dst2);
	    set_arc_weight(net_arc(n, 2*i+1, j), INT_MAX);		
	    count++;
	}	
    }

    return n;
}

struct v_cut *cut_to_v_cut (struct cut* c) 
{
    struct v_cut *res = malloc(sizeof(*res));
    set_v_cut_nb_vertices(res, cut_nb_arcs(c));
    set_v_cut_vertices(res, 
		       malloc(cut_nb_arcs(c) * sizeof(int)));
    for (int i = 0 ; i < cut_nb_arcs(c) ; i++)
	set_v_cut_vertex(res, i, arc_src(cut_arc(c, i))/2);

    return res;
}

struct v_cut *v_min_cut(struct graph *g, int src, int dst)
{
    struct network *n = graph_to_network(g, src, dst);
    struct cut* c = min_cut(n);
    struct v_cut *res = cut_to_v_cut(c);

    cut_free(c);
    net_free(n);
    return res;
}

struct move play(struct move const * moves, size_t size_moves)
{
    update_moves(moves, size_moves, &g);

    // init the movement
    struct move m;
    m.player = get_me(&g);

    
    /*  
     *	sets peng_cpls 
     *	it contains the couples of penguins 
     *  which could be fastly separated 
     */

    struct moveSet *peng_cpls = moveSet__reset(NULL);
    struct move cpl;
    cpl.player = get_me(&g);
 
    int  min_rupture = NB_TILES_TO_RUPTURE + 1;
    // checking all penguins combination
    for(int i = 0 ; i < get_nb_penguins ; i++)
    {
	int my_tile = get_penguin_pos(get_me(&g), i, &g);
	for(int t = 0 ; t < get_nb_players(&g) ; t++)
	    if(t != get_me(&g))
		for(int t_penguin = 0 ; t_penguin < get_nb_penguins(&g) ; t_penguin ++)
		{
		    int their_tile = get_penguin_pos(t, t_penguin, &g);
		    struct v_cut c = v_min_cut(&g, my_tile, their_tile);
		    // if the penguin still can meet 
		    if(v_cut_nb_vertices(&c) != 0)
		    {
			// if a best couple if found
			// peng_cpls is reseted
			if(v_cut_nb_vertices(&c) < min_rupture)
			{
			    moveSet__reset(peng_cpls);
			    min_rupture = v_cut_nb_vertices(&c);
			}
			// if another one with the same rupture limit is found
			// it's added to peng_cpls
			if(v_cut_nb_vertices(&c) <= min_rupture)
			{
			    cpl.src = my_tile;
			    cpl.dst = their_tile;
			    moveSet__add(peng_cpls, cpl);
			}
		    }
		}
    }
    
    // if no rupture is coming soon
    if(min_rupture >= NB_TILES_TO_RUPTURE)
	moveSet__free(peng_cpls);
	return play_as_line(m, &g);
    // ----------------------------

    // if one is, let's position our penguin on the rupture tile
    struct move cpl_to_split = moveSet__get(peng_cpls, rand() % moveSet__size(peng_cpls));
    moveSet__free(peng_cpls);
/*    struct moveSet **moves_to_rupture_tile = compute_best_traj(cpl_to_split.src,
							       cpl_to_split.dst,
*/						       &g);
    return play_as_line(m, &g);
}

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

struct move play_as_line(struct move m, struct graph *g)
{    
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
