#include "test_strat.h"


int test__get_opposite_tile()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 0;
    int nb_penguins = 0;

/********************************
 *  tests for a full-tetra-board * 
 ********************************/    
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);

    // to test all directions
    assert(get_opposite_tile(7, 12, &gr) == 17);
    assert(get_opposite_tile(13, 12, &gr) == 11);
    assert(get_opposite_tile(17, 12, &gr) == 7);
    assert(get_opposite_tile(11, 12, &gr) == 13);
    // to test a same tile case
    assert(get_opposite_tile(12, 12, &gr) == SAME_TILE);
    // to ensure diagonale moves aren't allowed
    printf("Testing diagonale moves on a full-tetra-board of size %dx%d:\n", width, height);
    printf("\t");
    assert(get_opposite_tile(6,12, &gr) == NOT_NEIGHBOURS);
    printf("\t");
    assert(get_opposite_tile(8,12, &gr) == NOT_NEIGHBOURS);
    printf("\t");
    assert(get_opposite_tile(16,12, &gr) == NOT_NEIGHBOURS);
    printf("\t");
    assert(get_opposite_tile(18,12, &gr) == NOT_NEIGHBOURS);
    // to test edges
    assert(get_opposite_tile(1, 0, &gr) == NO_TILE);
    assert(get_opposite_tile(5, 0, &gr) == NO_TILE);
    assert(get_opposite_tile(19, 24, &gr) == NO_TILE);
    assert(get_opposite_tile(23, 24, &gr) == NO_TILE);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);

    // to test all directions
    assert(get_opposite_tile(6, 11, &gr) == 16);
    assert(get_opposite_tile(9, 11, &gr) == 13);
    assert(get_opposite_tile(14, 11, &gr) == 8);
    assert(get_opposite_tile(16, 11, &gr) == 6);
    assert(get_opposite_tile(13, 11, &gr) == 9);
    assert(get_opposite_tile(8, 11, &gr) == 14);
    // to test a same tile case
    assert(get_opposite_tile(11, 11, &gr) == SAME_TILE);
    // to test edges
    assert(get_opposite_tile(3, 0, &gr) == NO_TILE);
    assert(get_opposite_tile(19, 22, &gr) == NO_TILE);

    free_graph(&gr);

/********************************
 *  tests for a octo-tetra-board * 
 ********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins, &gr);

    // to test all directions
    // --> for a square
    assert(get_opposite_tile(2, 7, &gr) == 12);
    assert(get_opposite_tile(8, 7, &gr) == 6);
    assert(get_opposite_tile(12, 7, &gr) == 2);
    assert(get_opposite_tile(6, 7, &gr) == 8);
    // --> for an octogone
    assert(get_opposite_tile(7, 12, &gr) == 17);
    assert(get_opposite_tile(8, 12, &gr) == 16);
    assert(get_opposite_tile(13, 12, &gr) == 11);
    assert(get_opposite_tile(18, 12, &gr) == 6);
    assert(get_opposite_tile(17, 12, &gr) == 7);
    assert(get_opposite_tile(16, 12, &gr) == 8);
    assert(get_opposite_tile(11, 12, &gr) == 13);
    assert(get_opposite_tile(6, 12, &gr) == 18);
    // to test same tile cases
    assert(get_opposite_tile(12, 12, &gr) == SAME_TILE);
    assert(get_opposite_tile(7, 7, &gr) == SAME_TILE);
    // to test edges
    assert(get_opposite_tile(6, 0, &gr) == NO_TILE);
    assert(get_opposite_tile(6, 5, &gr) == NO_TILE);
    assert(get_opposite_tile(11, 10, &gr) == NO_TILE);

    free_graph(&gr);

    return 1;
}

int test__get_max_length()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 2;
    int nb_penguins = 2;
    int size_moves = 3;
    size_t nb_moves;
    struct move moves[size_moves];

/********************************
 *  tests for a full-tetra-board * 
 ********************************/    
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);
    nb_moves = 3;
    moves[0] = build_move(0, NO_TILE, 12);
    moves[1] = build_move(1, NO_TILE, 17);
    moves[2] = build_move(0, NO_TILE, 2);
    update_moves(moves, nb_moves, &gr);

    // to test max length possible
    assert(get_max_length(5, 1, &gr) == 4);
    // to ensure the starting tile isn't counted as occupied
    assert(get_max_length(12, 1, &gr) == 2);
    assert(get_max_length(10, 3, &gr) == 0);
    // to test with a penguin on the trajectory
    assert(get_max_length(12, 2, &gr) == 0);
    assert(get_max_length(12, 0, &gr) == 1);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);
    nb_moves = 2;
    moves[0] = build_move(0, NO_TILE, 8);
    moves[1] = build_move(1, NO_TILE, 16);
    update_moves(moves, nb_moves, &gr);

    // to test max length possible
    assert(get_max_length(0, 2, &gr) == 4);
    // to ensure the starting tile isn't counted as occupied
    assert(get_max_length(10, 4, &gr) == 0);
    assert(get_max_length(10, 3, &gr) == 2);
    // to test with a penguin on the trajectory
    assert(get_max_length(10, 1, &gr) == 0);
    assert(get_max_length(10, 2, &gr) == 1);

    free_graph(&gr);

/*********************************
 *  tests for a octo-tetra-board * 
 *********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins,  &gr);
    nb_moves = 2;
    moves[0] = build_move(0, NO_TILE, 22);
    moves[1] = build_move(1, NO_TILE, 23);
    update_moves(moves, nb_moves, &gr);
    
    // to test max length possible
    assert(get_max_length(0, 3, &gr) == 4); // only octogones
    assert(get_max_length(1, 2, &gr) == 4);  // with squares
    // to ensure the starting tile isn't counted as occupied
    assert(get_max_length(5, 3, &gr) == 0); // tile 5 is a square
    assert(get_max_length(10, 6, &gr) == 0); // tile 10 is an octogone
    // to test with a penguin on the trajectory
    assert(get_max_length(2, 4, &gr) == 3); // penguin at 23 (square)
    assert(get_max_length(3, 2, &gr) == 3); // penguin at 22 (octogone)
    
    free_graph(&gr);
    
    return 1;
}

int test__get_landing_tile()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 2;
    int nb_penguins = 2;

/********************************
 *  tests for a full-tetra-board * 
 ********************************/    
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);

    // to test some normal cases
    assert(get_landing_tile(1, 2, 3, &gr) == 16);
    assert(get_landing_tile(14, 3, 4, &gr) == 10);
    // to test edges
    assert(get_landing_tile(10, 1, 5, &gr) == NO_TILE);
    assert(get_landing_tile(12, 2, 0, &gr) == 12);
    assert(get_landing_tile(12, 0, 3, &gr) == NO_TILE);
    assert(get_landing_tile(22, 0, 1, &gr) == 17);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);

    // to test some normal cases
    assert(get_landing_tile(8, 2, 2, &gr) == 14);
    assert(get_landing_tile(9, 4, 3, &gr) == 15);
    // to test edges
    assert(get_landing_tile(10, 1, 5, &gr) == NO_TILE);
    assert(get_landing_tile(11, 2, 0, &gr) == 11);
    assert(get_landing_tile(1, 0, 3, &gr) == NO_TILE);
    assert(get_landing_tile(16, 0, 1, &gr) == 11);

    free_graph(&gr);

/*********************************
 *  tests for a octo-tetra-board * 
 *********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins,  &gr);

    // to test some normal cases
    assert(get_landing_tile(1, 2, 3, &gr) == 16);
    assert(get_landing_tile(14, 6, 3, &gr) == 11);
    // to test edges
    assert(get_landing_tile(15, 1, 5, &gr) == NO_TILE);
    assert(get_landing_tile(12, 6, 0, &gr) == 12);
    assert(get_landing_tile(11, 0, 3, &gr) == NO_TILE);
    assert(get_landing_tile(12, 1, 1, &gr) == 8);    

    free_graph(&gr);

    return 1;
}

int test__is_isolated()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 2;
    int nb_penguins = 2;

/********************************
 *  tests for a full-tetra-board * 
 ********************************/    
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(11, &gr);
    sink_tile(15, &gr);
    sink_tile(21, &gr);

    // to test some normal cases
    assert(is_isolated(7, &gr) == 0);
    assert(is_isolated(13, &gr) == 0);
    assert(is_isolated(17, &gr) == 0);
    // to test a tile close to empty tiles
    assert(is_isolated(10, &gr) == 0);
    assert(is_isolated(12, &gr) == 0);
    assert(is_isolated(16, &gr) == 0);
    assert(is_isolated(22, &gr) == 0);
    // to test isolated tiles
    assert(is_isolated(20, &gr) == 1);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);
    sink_tile(15, &gr);
    sink_tile(16, &gr);
    sink_tile(18, &gr);
    sink_tile(19, &gr);
    sink_tile(23, &gr);

    // to test some normal cases
    assert(is_isolated(6, &gr) == 0);
    assert(is_isolated(8, &gr) == 0);
    assert(is_isolated(9, &gr) == 0);
    // to test a tile close to empty tiles
    assert(is_isolated(17, &gr) == 0);
    assert(is_isolated(21, &gr) == 0);
    assert(is_isolated(22, &gr) == 0);
    assert(is_isolated(24, &gr) == 0);
    // to test isolated tiles
    assert(is_isolated(20, &gr) == 1);

    free_graph(&gr);

/*********************************
 *  tests for a octo-tetra-board * 
 *********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(15, &gr);
    sink_tile(16, &gr);
    sink_tile(18, &gr);
    sink_tile(21, &gr);
    sink_tile(22, &gr);
    sink_tile(24, &gr);

    // to test some normal cases
    assert(is_isolated(6, &gr) == 0);
    assert(is_isolated(7, &gr) == 0);
    assert(is_isolated(8, &gr) == 0);
    // to test a tile close to empty tiles
    assert(is_isolated(10, &gr) == 0);
    assert(is_isolated(14, &gr) == 0);
    assert(is_isolated(17, &gr) == 0);
    assert(is_isolated(19, &gr) == 0);
    // to test isolated tiles
    assert(is_isolated(20, &gr) == 1); // octogonal tile
    assert(is_isolated(23, &gr) == 1); // squared tile

    free_graph(&gr);

    return 1;
}

int test__can_move()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 2;
    int nb_penguins = 3;

/********************************
 *  tests for a full-tetra-board * 
 ********************************/
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(11, &gr);
    sink_tile(15, &gr);
    sink_tile(21, &gr);
    set_penguin_pos(0, 0, 16, &gr);
    set_penguin_pos(1, 0, 7, &gr);
    set_penguin_pos(0, 1, 17, &gr);
    set_penguin_pos(1, 1, 20, &gr);
    set_penguin_pos(0, 2, 22, &gr);
    set_penguin_pos(1, 2, 23, &gr);

    // to test some normal cases
    assert(can_move(1, 0, &gr) == 1);
    // to test penguins with limited moves
    assert(can_move(0, 1, &gr) == 1);
    assert(can_move(1, 2, &gr) == 1);
    // to test isolated penguins
    assert(can_move(0, 0, &gr) == 0);
    assert(can_move(1, 1, &gr) == 0);
    assert(can_move(0, 2, &gr) == 0);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);
    sink_tile(15, &gr);
    sink_tile(18, &gr);
    sink_tile(23, &gr);
    set_penguin_pos(0, 0, 6, &gr);
    set_penguin_pos(1, 0, 20, &gr);
    set_penguin_pos(0, 1, 16, &gr);
    set_penguin_pos(1, 1, 19, &gr);
    set_penguin_pos(0, 2, 24, &gr);
    set_penguin_pos(1, 2, 21, &gr);

    // to test some normal cases
    assert(can_move(0, 0, &gr) == 1);
    // to test penguins with limited moves
    assert(can_move(0, 1, &gr) == 1);
    assert(can_move(1, 1, &gr) == 1);
    assert(can_move(0, 2, &gr) == 1);
    // to test isolated penguins
    assert(can_move(1, 2, &gr) == 0);
    assert(can_move(1, 0, &gr) == 0);

    free_graph(&gr);

/*********************************
 *  tests for a octo-tetra-board * 
 *********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(15, &gr);
    sink_tile(16, &gr);
    sink_tile(17, &gr);
    sink_tile(18, &gr);
    sink_tile(21, &gr);
    sink_tile(23, &gr);
    set_penguin_pos(0, 0, 6, &gr);
    set_penguin_pos(1, 0, 8, &gr);
    set_penguin_pos(0, 1, 19, &gr);
    set_penguin_pos(1, 1, 14, &gr);
    set_penguin_pos(0, 2, 22, &gr);
    set_penguin_pos(1, 2, 20, &gr);

    // to test some normal cases
    assert(can_move(0, 0, &gr) == 1);
    // to test penguins with limited moves
    assert(can_move(0, 1, &gr) == 1);
    assert(can_move(1, 1, &gr) == 1);
    assert(can_move(1, 0, &gr) == 1);
    // to test isolated penguins
    assert(can_move(1, 2, &gr) == 0);
    assert(can_move(0, 2, &gr) == 0);

    free_graph(&gr);
    
    return 1;
}

int test__aligned_penguins()
{
    struct graph gr;
    int width = 5;
    int height = 5;
    int nb_players = 2;
    int nb_penguins = 6;

/********************************
 *  tests for a full-tetra-board * 
 ********************************/
    init_full_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(13, &gr);
    set_penguin_pos(0, 0, 2, &gr);
    set_penguin_pos(0, 1, 8, &gr);
    set_penguin_pos(0, 2, 10, &gr);
    set_penguin_pos(1, 0, 9, &gr);
    set_penguin_pos(1, 1, 12, &gr);
    set_penguin_pos(1, 2, 18, &gr);

    // to test really unaligned penguins
    assert(aligned_penguins(get_tile_penguin(2, &gr), get_tile_penguin(18, &gr), &gr) == 0);
    assert(aligned_penguins(get_tile_penguin(2, &gr), get_tile_penguin(9, &gr), &gr) == 0);
    // to test penguins separated by a penguin
    assert(aligned_penguins(get_tile_penguin(8, &gr), get_tile_penguin(10, &gr), &gr) == 0);
    // to test penguins separated by a melted tile
    assert(aligned_penguins(get_tile_penguin(8, &gr), get_tile_penguin(18, &gr), &gr) == 0);
    // to test aligned tiles
    assert(aligned_penguins(get_tile_penguin(2, &gr), get_tile_penguin(12, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(8, &gr), get_tile_penguin(9, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(9, &gr), get_tile_penguin(10, &gr), &gr) == 1);

    free_graph(&gr);

/********************************
 *  tests for a full-hexa-board * 
 ********************************/
    init_full_hexa(width, height, nb_players, nb_penguins, &gr);
    sink_tile(12, &gr);
    set_penguin_pos(0, 0, 2, &gr);
    set_penguin_pos(0, 1, 13, &gr);
    set_penguin_pos(0, 2, 17, &gr);
    set_penguin_pos(1, 0, 3, &gr);
    set_penguin_pos(1, 1, 7, &gr);
    set_penguin_pos(1, 2, 15, &gr);

    // to test really unaligned penguins
    assert(aligned_penguins(get_tile_penguin(2, &gr), get_tile_penguin(13, &gr), &gr) == 0);
    assert(aligned_penguins(get_tile_penguin(7, &gr), get_tile_penguin(15, &gr), &gr) == 0);
    // to test penguins separated by a penguin
    assert(aligned_penguins(get_tile_penguin(13, &gr), get_tile_penguin(17, &gr), &gr) == 0);
    // to test penguins separated by a melted tile
    assert(aligned_penguins(get_tile_penguin(7, &gr), get_tile_penguin(17, &gr), &gr) == 0);
    // to test aligned tiles
    assert(aligned_penguins(get_tile_penguin(3, &gr), get_tile_penguin(7, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(7, &gr), get_tile_penguin(13, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(13, &gr), get_tile_penguin(15, &gr), &gr) == 1);

    free_graph(&gr);

/*********************************
 *  tests for a octo-tetra-board * 
 *********************************/
    init_octo_tetra(width, height, nb_players, nb_penguins, &gr);
    sink_tile(13, &gr);
    set_penguin_pos(0, 0, 1, &gr);
    set_penguin_pos(0, 1, 5, &gr);
    set_penguin_pos(0, 2, 20, &gr);
    set_penguin_pos(1, 0, 3, &gr);
    set_penguin_pos(1, 1, 10, &gr);
    set_penguin_pos(1, 2, 18, &gr);

    // to test really unaligned penguins
    assert(aligned_penguins(get_tile_penguin(1, &gr), get_tile_penguin(10, &gr), &gr) == 0);
    assert(aligned_penguins(get_tile_penguin(3, &gr), get_tile_penguin(20, &gr), &gr) == 0);
    // to test penguins separated by a penguin
    assert(aligned_penguins(get_tile_penguin(5, &gr), get_tile_penguin(20, &gr), &gr) == 0);
    // to test penguins separated by a melted tile
    assert(aligned_penguins(get_tile_penguin(3, &gr), get_tile_penguin(18, &gr), &gr) == 0);
    // to test aligned tiles
    assert(aligned_penguins(get_tile_penguin(1, &gr), get_tile_penguin(3, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(10, &gr), get_tile_penguin(20, &gr), &gr) == 1);
    assert(aligned_penguins(get_tile_penguin(5, &gr), get_tile_penguin(10, &gr), &gr) == 1);

    free_graph(&gr);
    
    return 1;
}

struct move build_move(int player, int src, int dst)
{
    struct move move;
    move.player = player;
    move.src = src;
    move.dst = dst;
    return move;
}

void sink_tile(int tile, struct graph *gr)
{
    // for each neighbour n of tile
    for (int i = 0 ; i < get_tile_nb_neighbours(tile, gr) ; i ++)
    {
        int n = get_tile_neighbour(tile, i, gr);
        if (n != NO_TILE)
        {
            // looking for tile in the neighbours of n
            int j = 0;
            while (get_tile_neighbour(n, j, gr) != tile)
                j++;
            // removing it
            set_tile_neighbour(n, j, NO_TILE, gr);
            // removing n from tile neighbours
            set_tile_neighbour(tile, i, NO_TILE, gr);
        }
    }
}

int init_full_tetra(int width, int height, int nb_players, int nb_penguins, struct graph *gr) 
{
//    printf("nb_players : %d\nnb_penguins : %d\n",nb_players,nb_penguins);
    init_pawns(nb_players, nb_penguins, gr);

    set_nb_tiles(height * width, gr);
    set_tiles(calloc(get_nb_tiles(gr), sizeof(struct tile)), gr);
    if (! get_tiles(gr)) {
        printf("init_full_tetra : could not allocate tiles\n");
        return 0;
    }
    
    int diff[4] = { -width, 1, width, -1 };
    
    // initialize and link each tile
    for (int i = 0 ; i < get_nb_tiles(gr) ; i ++) 
    {
        set_tile_nb_fish(i, (i % 3) + 1, gr);
        set_tile_nb_neighbours(i, 4, gr);
        set_tile_penguin(i, EMPTY, gr);
        set_tile_neighbours(i, calloc(4, sizeof(int)), gr);
        if (! get_tile_neighbours(i, gr)) {
            printf("init_full_tetra : could not allocate neighb of tile %d\n", i);
            return 0;
        }

        for (int j = 0 ; j < 4 ; j++)
            set_tile_neighbour(i, j, i + diff[j], gr);

        // placing water on top, bottom, left and right
        if (i < width)
            set_tile_neighbour(i, 0, NO_TILE, gr);
        if (i >= (get_nb_tiles(gr) - width))
            set_tile_neighbour(i, 2, NO_TILE, gr);
        if ((i % width) == 0) 
            set_tile_neighbour(i, 3, NO_TILE, gr);
        if ((i % width) == (width-1))
            set_tile_neighbour(i, 1, NO_TILE, gr);
    }
    return 1;
} 

int init_full_hexa(int width, int height, int nb_players, int nb_penguins, struct graph *gr) 
{
    init_pawns(nb_players, nb_penguins, gr);

    set_nb_tiles(height * width, gr);
    set_tiles(calloc(get_nb_tiles(gr), sizeof(struct tile)), gr);
    if (! get_tiles(gr)) {
        printf("init_full_hexa : could not allocate tiles\n");
        return 0;
    }

    // permuting width and height to have an odd width if possible
    // because and odd width results in simpler calculations
    if (((width % 2) == 1) && ((height % 2) == 0)) {
        int tmp = width;
        width = height;
        height = tmp;
    }

    int even_diff[6];
    int odd_diff[6];

    // now, according to width parity, there are 2 different rules linking the 
    // number of the tiles
    if ((width % 2) == 1) 
    {
        memcpy(even_diff, ((int[]) { -width, -width/2, (width/2)+1, 
                        width, width/2, -(width/2)-1 }), sizeof even_diff);
        memcpy(odd_diff, ((int[]) { -width, -width/2, (width/2)+1, 
                        width, width/2, -(width/2)-1 }), sizeof odd_diff);
    } else 
    {
        memcpy(even_diff, ((int[]) { -width, -width+1, 1, width, -1, -width-1 }),
               sizeof even_diff);
        memcpy(odd_diff, ((int[]) { -width, 1, width+1, width, width-1, -1}) , 
               sizeof odd_diff);
    }

    // initializing and linking each tile
    for (int i = 0 ; i < get_nb_tiles(gr) ; i ++) 
    {
        set_tile_nb_fish(i, (i % 3) + 1, gr);
        set_tile_nb_neighbours(i, 6, gr);
        set_tile_penguin(i, EMPTY, gr);
        set_tile_neighbours(i, calloc(6, sizeof(int)), gr);
    if (! get_tile_neighbours(i, gr)) {
            printf("init_full_hexa : could not allocate neighb of tile %d\n", i);
            return 0;
        }

        for (int j = 0 ; j < 6 ; j++)
        {
            if ((i % 2) == 0)
                set_tile_neighbour(i, j, i + even_diff[j], gr);
            else
                set_tile_neighbour(i, j, i + odd_diff[j], gr);
	    
            // placing water on 2 sides
            if ((get_tile_neighbour(i,j, gr) < 0) || 
                (get_tile_neighbour(i,j, gr) >= get_nb_tiles(gr)))
                set_tile_neighbour(i, j, NO_TILE, gr);
        }

        // and on the other 2 sides
        if ((i % width) == 0) 
        {
	    set_tile_neighbour(i, 4, NO_TILE, gr);
	    set_tile_neighbour(i, 5, NO_TILE, gr);
        }
        if ((((width % 2) == 1) && (i % width) == (width/2)) ||
            (((width % 2) == 0) && (i % width) == (width-1)))
        {
	    set_tile_neighbour(i, 1, NO_TILE, gr);
	    set_tile_neighbour(i, 2, NO_TILE, gr);
        }
    }         
    return 1;
} 

int init_octo_tetra(int width, int height, int nb_players, int nb_penguins, struct graph *gr) 
{
    init_pawns(nb_players, nb_penguins, gr);

    set_nb_tiles(height * width, gr);
    set_tiles(calloc(get_nb_tiles(gr), sizeof(struct tile)), gr);
    if (! get_tiles(gr)) {
        printf("init_octo_tetra : could not allocate tiles\n");
        return 0;
    }
    
    int tetra_diff[4] = { -width, 1, width, -1 };
    int octo_diff[8] = { -width, -width+1, 1, width+1, width, width-1, -1, -width-1 };
    
    // initialize and link each tile
    for (int j = 0 ; j < height ; j++) {
        for (int i = 0 ; i < width ; i ++)
        {
            int nb = j*width + i;
            int nb_sides = (i%2 == j%2) ? 8 : 4;
            set_tile_nb_fish(nb, (i % 3) + 1, gr);
            set_tile_nb_neighbours(nb, nb_sides, gr);
            set_tile_penguin(nb, EMPTY, gr);
            set_tile_neighbours(nb, calloc(nb_sides, sizeof(int)), gr);
	if (! get_tile_neighbours(nb, gr)) {
                printf("init_octo_tetra : cant allocate neighb of tile %d\n", nb);
                return 0;
            }

            for (int k = 0 ; k < nb_sides ; k++)
                if (nb_sides == 8)
		    set_tile_neighbour(nb, k, nb + octo_diff[k], gr);
		    else
		    set_tile_neighbour(nb, k, nb + tetra_diff[k], gr);

        // placing water on top, bottom, left and right
            if (j == 0) {
		set_tile_neighbour(nb, 0, NO_TILE, gr);
                if (nb_sides == 8) {
		set_tile_neighbour(nb, 1, NO_TILE, gr);
		set_tile_neighbour(nb, 7, NO_TILE, gr);
                }
            }
            if (j == height-1) {
                if (nb_sides == 8) {
		set_tile_neighbour(nb, 3, NO_TILE, gr);
		set_tile_neighbour(nb, 4, NO_TILE, gr);
		set_tile_neighbour(nb, 5, NO_TILE, gr);
                } else {
		set_tile_neighbour(nb, 2, NO_TILE, gr);
                }
            }
            if (i == 0) {
                if (nb_sides == 8) {
		set_tile_neighbour(nb, 5, NO_TILE, gr);
		set_tile_neighbour(nb, 6, NO_TILE, gr);
		set_tile_neighbour(nb, 7, NO_TILE, gr);
                } else {
		set_tile_neighbour(nb, 3, NO_TILE, gr);
                }
            }
            if (i == width-1) {
		set_tile_neighbour(nb, 1, NO_TILE, gr);
                if (nb_sides == 8) {
		set_tile_neighbour(nb, 2, NO_TILE, gr);
		set_tile_neighbour(nb, 3, NO_TILE, gr);
                }
            }
        }
    }
    return 1;
}

void set_nb_tiles(int nb_tiles, struct graph *gr) {
    gr->nb_tiles = nb_tiles;
}

void set_tiles(struct tile *tiles, struct graph *gr) {
    gr->tiles = tiles;
}

void set_tile_nb_fish(int tile, int nb_fish, struct graph *gr) {
    gr->tiles[tile].nb_fish = nb_fish;
}

void set_tile_nb_neighbours(int tile, int nb_neighbours, struct graph *gr) {
    gr->tiles[tile].nb_neighbours = nb_neighbours;
}

void set_tile_neighbours(int tile, int* neighbours, struct graph *gr) {
    gr->tiles[tile].neighbours = neighbours;
}

void set_tile_penguin(int tile, int penguin, struct graph *gr) {
    gr->tiles[tile].penguin = penguin;
}

void set_tile_neighbour(int tile, int neighbour_number, int neighbour_tile, struct graph *gr) {
    gr->tiles[tile].neighbours[neighbour_number] = neighbour_tile;
}

void set_penguin_pos(int player, int penguin_number, int tile, struct graph *gr) {
    set_tile_penguin(tile, penguin_number + (player * get_nb_penguins(gr)), gr);
    gr->pawns[player][penguin_number] = tile;
}

int main()
{
    if(test__get_opposite_tile()
       && test__get_max_length()
       && test__get_landing_tile()
       && test__is_isolated()
       && test__can_move()
       && test__aligned_penguins())
    {
	printf("Tous les tests sont passés\n");
	return 1;
    }
    
    return 0;
}
