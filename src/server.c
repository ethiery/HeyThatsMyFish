#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include <assert.h>

#define FIRST_STRAT 5




// SETTERS AND GETTERS FOR DATA STRUCTURE

void set_serv(struct server *s) {
    srv = s;
}

void set_scores(int *scores) {
    srv->scores = scores;
}

void set_score(int i, int score) {
    srv->scores[i] = score;
}

void set_nb_strats(int nb_strats) {
    srv->nb_strategies = nb_strats;
}

void set_strats(struct strategy* strats) {
    srv->strategies = strats;
}

void set_handle(int i, void* h) {
    srv->strategies[i].lib_handle = h;
}

void set_init(int i, void* init) {
    srv->strategies[i].init = init;
}

void set_place(int i, void* place) {
    srv->strategies[i].place = place;
}

void set_play(int i, void* play) {
    srv->strategies[i].play = play;
}

void set_done(int i, void* done) {
    srv->strategies[i].done = done;
}

void set_graph(struct graph* g) {
    srv->g = g;
}

void set_nb_tiles(int nb_tiles) {
    srv->g->nb_tiles = nb_tiles;
}

void set_tiles(struct tile *tiles) {
    srv->g->tiles = tiles;
}

void set_tile_nb_fish(int i, int nb_fish) {
    srv->g->tiles[i].nb_fish = nb_fish;
}

void set_tile_nb_neighbours(int i, int nb_neighbours) {
    srv->g->tiles[i].nb_neighbours = nb_neighbours;
}

void set_tile_neighbours(int i, int* neighbours) {
    srv->g->tiles[i].neighbours = neighbours;
}

void set_tile_penguin(int i, int penguin) {
    srv->g->tiles[i].penguin = penguin;
}

void set_tile_neighbour(int i, int j, int neighbour) {
    srv->g->tiles[i].neighbours[j] = neighbour;
}

void set_nb_players(int nb_players) {
    srv->g->nb_players = nb_players;
}

void set_players(int **players) {
    srv->g->players = players;
}

void set_player(int i, int* player) {
    srv->g->players[i] = player;
}

void set_penguin_pos(int i, int j, int penguin) {
    srv->g->players[i][j] = penguin;
}

void set_nb_penguins(int nb_penguins) {
    srv->g->nb_penguins = nb_penguins;
}


struct server *get_serv(void) {
    return srv;
}

int* get_scores(void) {
    return srv->scores;
}

int get_score(int i) {
    return srv->scores[i];
}

size_t get_nb_strats(void) {
    return srv->nb_strategies;
}

struct strategy *get_strats(void) {
    return srv->strategies;
}

struct strategy* get_strat(int i) {
    return &(srv->strategies[i]);
}

void *get_handle(int i) {
    return srv->strategies[i].lib_handle;
}

struct graph* get_graph(void) {
    return srv->g;
}

size_t get_nb_players(void) {
    return srv->g->nb_players;
}

int** get_players() {
    return srv->g->players;
}

int* get_player(int i) {
    return srv->g->players[i];
}

int get_penguin_pos(int i, int j) {
    return srv->g->players[i][j];
}

size_t get_nb_tiles(void) {
    return srv->g->nb_tiles;
}

struct tile *get_tiles(void) {
    return srv->g->tiles;
}

struct tile *get_tile(int i) {
    return &(srv->g->tiles[i]);
}

int get_tile_nb_neighbours(int i) {
    return srv->g->tiles[i].nb_neighbours;
}

int* get_tile_neighbours(int i) {
    return srv->g->tiles[i].neighbours;
}

int get_tile_neighbour(int i, int j) {
    return srv->g->tiles[i].neighbours[j];
}

int get_tile_nb_fish(int i) {
    return srv->g->tiles[i].nb_fish;
}

int get_tile_penguin(int i) {
    return srv->g->tiles[i].penguin;
}

size_t get_nb_penguins(void) {
    return srv->g->nb_penguins;
}

void free_srv(void) {
    if (get_graph())
        free_graph();
    if (get_strats())
        free_strats();
    if (get_scores())
        free_scores();
    free(srv);
}

void free_strats(void) {
    for (int i = 0 ; i < get_nb_strats() ; i++)
        if (get_handle(i))
            dlclose(get_handle(i));
    free(get_strats());
}

void free_scores(void) {
    free(get_scores());
}

void free_graph(void)
{
    if (get_tiles()) {
        for (int i = 0 ; i < get_nb_tiles() ; i++)
            if (get_tile_neighbours(i))
                free(get_tile_neighbours(i));
        free(get_tiles());
    }

    if (get_players()) {
        for (int i = 0 ; i < get_nb_players() ; i++)
            if (get_player(i))
                free(get_player(i));
        free(get_players());
    }

    free(get_graph());
}


int penguin_owner(int p) {
    return p / get_nb_penguins();
}


int penguin_number(int p) {
    return p % get_nb_penguins();
}



// MAIN FUNCTIONS


int server_init(int argc, char** argv)
{
    // checking arguments passed by the user
    if (!correct_main_args(argc, argv))
        return 0;


    // allocating server structure
    set_serv(calloc(1, sizeof(*srv)));

    if (!get_serv()) {
        printf("server_init : could not allocate server\n");
        return 0;
    }

    // allocating strats array
    set_nb_strats(argc - FIRST_STRAT);
    int N = get_nb_strats();
    set_strats(calloc(N,sizeof(struct strategy)));
    if (!get_strats()) {
        printf("server_init : could not allocate strats array\n");
        free_srv();
        return 0;
    }

    // loading strategy libraries
    if (!server_load_strats(argv + FIRST_STRAT)) {
        free_srv();
        return 0;
    }

    // allocating and initializing scores array
    set_scores(calloc(N, sizeof(int)));


    // initializing the graph
    int peng_per_player = atoi(argv[1]);
    char* config = argv[2];
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    if(!graph_init(config, width, height, N, peng_per_player)) {
        free_srv();
        return 0;
    }


    return 1;
}


int strategies_init(void)
{
    // Getting data needed for strategy initializations :
    size_t size;
    struct edge *edges = NULL;
    int *fishes = NULL;
    if (! create_init_data(&edges, &size, &fishes)) {
        printf("strategies_init : could not get init data\n");
        return 0;
    }

    // Initializing strategies
    for (int i = 0 ; i < get_nb_players() ; i++)
        get_strat(i)->init(i, get_nb_tiles(), edges, size, fishes,
                           get_nb_players(), get_nb_penguins());

    // Freeing init data
    free_init_data(&edges, &fishes);
    return 1;
}





int place_penguins(struct move *prev_moves) {
    struct move mv;
    int size = 0;

    static int turn = 0; // turn counter, one turn is nb_players placements
    static int p = 0; // p contains the id of the player who has to play now

    // if placement turns are over, 2 is returned else
    if(turn < get_nb_penguins()) {
        if(p < get_nb_players())
        {
            size  = (turn < 1) ? p : get_nb_players();

	    // getting the placement of the strat who has to play now
            mv = get_strat(p)->place(prev_moves, size);
	    // checking it and applying it
            if (!check_n_place(p, &mv, turn))
                return 0;

	    // updating previous moves
            update_previous_moves(prev_moves, mv, turn, get_nb_players());

	    // updating p and turn
            p++;
            if(p >= get_nb_players()) {
                p = 0;
                turn++;
            }

            return 1;
        }
    }
    return 2;
}


int play_game(struct move *prev_moves) {
    struct move mv;

    static int i = 0;
    static int first = 1;
    static int turn;
    static int nb_players_in_game;
    static int* players_in_game;

    if(first)
    {
        turn = get_nb_penguins();
        // initializing the array of players still in game
        nb_players_in_game = get_nb_players();
        players_in_game = malloc(nb_players_in_game * sizeof(int));
        for (int p = 0 ; p < get_nb_players() ; ++p)
            players_in_game[p] = p;

        first = 0;
    }


    if(nb_players_in_game > 0) {
        if(i < nb_players_in_game)
        {
            // getting, checking and applying player's move
            int p = players_in_game[i];

            if (! is_blocked(p))
            {
                mv = get_strat(p)->play(prev_moves, nb_players_in_game);
                if(!check_n_play(p, &mv))
                {
                    free(players_in_game);
                    return 0;
                }
                update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
                i++;
            }
            // if the player can't move, we fill prev_moves with a null move
            // and update players_in_game
            else {
                mv.player = p;
                mv.src = NO_TILE;
                mv.dst = NO_TILE;
                update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
                nb_players_in_game--;
                for (int j = i ; j < nb_players_in_game ; j++)
                    players_in_game[j] = players_in_game[j+1];
            }

            if(i >= nb_players_in_game) {
                i = 0;
                turn ++;
            }
        }
        return 1;
    }
    free(players_in_game);
    return 2;
}


void end_game(void)
{

    // removing all the penguins from the board, updating scores
    for (int p = 0 ; p < get_nb_players() ; p++) {

        for (int peng = 0 ; peng < get_nb_penguins() ; peng++)
        {
            int tile = get_penguin_pos(p, peng);
            if(tile !=  NO_TILE) 
	    {
                set_penguin_pos(p, peng, NO_TILE);
                set_score(p, get_score(p) + get_tile_nb_fish(tile));
                set_tile_penguin(tile, NO_TILE);
                disconnect_tile(tile);
            }
        }
    }

    // printing scores
    int winner = -1;
    int max = 0;
    printf("\nFINAL SCORES :\n-------------\n\n");
    for (int p = 0 ; p < get_nb_players() ; p++) {
        printf("Joueur %d : %d\n", p, get_score(p));
        if (get_score(p) > max) {
            max = get_score(p);
            winner = p;
        }
    }
    printf("The winner is player %d\n", winner);
    
    // calling done functions of strategies
    stategies_done();
    free_srv();

}



// SERVER_INIT AUXILIARY FUNCTIONS

int correct_main_args(int argc, char** argv)
{
    int nb_strats = argc - FIRST_STRAT;
    if (nb_strats < 2) {
        printf("Usage : %s <number of penguins per player>", argv[0]);
        printf("<board configuration> <board width> <board length> <strat1>");
        printf("<strat2> ... \n <number of penguins per player> >0\n");
        printf("<board configuration> = full-hexa | octo-tetra | full-tetra\n");
        printf("<board width> > 0\n<board length> > 0\n");
        return 0;
    }

    int nb_penguins = atoi(argv[1]);
    if (nb_penguins < 1) {
        printf("Serv_init : number of penguins per player must be > 0\n");
        return 0;
    }

    char* board_cfg = argv[2];
    if ((strcmp(board_cfg, "full-hexa") != 0) &&
            (strcmp(board_cfg, "octo-tetra") != 0) &&
            (strcmp(board_cfg, "full-tetra") != 0)) {
        printf("Serv_init : unknown board configuration.\n");
        printf("Try full-hexa, octa-tetra or full-tetra\n");
        return 0;
    }

    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    if (width * height < 2 * nb_strats * nb_penguins) {
        printf("Server_init : as penguins can be placed only on 1-fish tiles\n");
	printf(" and it represents half the tiles, width * height must be \n");
	printf("greater than to 2 * nb_strats * nb_penguins\n");
        return 0;
    }

    return 1;
}


int server_load_strats(char** strats)
{
    char *error;

    for (int i = 0 ; i < get_nb_strats() ; i++) {
        // opening the strategy
        set_handle(i, dlopen(strats[i], RTLD_NOW));
        if ((error = dlerror()) != NULL) {
            printf("server_init : can't load the strategy %s\n", strats[i]);
            printf("dlopen : %s\n", error);
            return 0;
        }

        // loading init function
        set_init(i, dlsym(get_handle(i), "init"));
        if ((error = dlerror()) != NULL) {
            printf("server_init : can't load init func %s\n", strats[i]);
            printf("dlopen : %s\n", error);
            return 0;
        }

        //loading place function
        set_place(i, dlsym(get_handle(i), "place"));
        if ((error = dlerror()) != NULL) {
            printf("server_init : can't load place func%s\n", strats[i]);
            printf("dlopen : %s\n", error);
            return 0;
        }

        //loading play function
        set_play(i, dlsym(get_handle(i), "play"));
        if ((error = dlerror()) != NULL) {
            printf("server_init : can't load play func %s\n", strats[i]);
            printf("dlopen : %s\n", error);
            return 0;
        }

	//loading done function
	set_done(i, dlsym(get_handle(i), "done"));
	if ((error = dlerror()) != NULL) {
	    printf("server_init : can't load done func %s\n", strats[i]);
	    printf("dlopen : %s\n", error);
	    return 0;
	}
    }
    return 1;
}

int graph_init(char* layout, int width, int height,
               int nb_players, int penguins_by_player)
{
    srand(time(NULL));

    // allocating tiles
    set_graph(calloc(1,sizeof(struct graph)));
    if (!get_graph()) {
        printf("graph_init : could not allocate graph\n");
        return 0;
    }

    // filling data
    set_nb_players(nb_players);
    set_nb_penguins(penguins_by_player);

    // allocating and initializing tiles
    int ret = 0;
    if (strcmp(layout, "full-tetra") == 0)
        ret = init_full_tetra(width, height);
    else if (strcmp(layout, "full-hexa") == 0)
        ret = init_full_hexa(width, height);
    else if (strcmp(layout, "octo-tetra") == 0)
        ret = init_octo_tetra(width, height);
    else
        printf("graph_init : unknown configuration\n");

    if (! ret) {
        free_graph();
        return 0;
    }

    // allocating players (which is the array of penguins positions)
    set_players(calloc(get_nb_players(), sizeof(int*)));
    if (! get_players())
    {
        printf("graph_init : couldn't initialize players\n");
        free_graph();
        return 0;
    }

    // initializing all penguins positions to NO_TILE
    for(int i = 0; i < get_nb_players(); ++i)
    {
        set_player(i, calloc(get_nb_penguins(), sizeof(int)));
        if (! get_player(i)) {
            printf("graph_init : couldn't initialize players[%d]\n", i);
            free_graph();
            return 0;
        }

        for(int j = 0; j < get_nb_penguins(); ++j)
            set_penguin_pos(i, j, NO_TILE);
    }

    return 1;
}

int init_full_hexa(int width, int height)
{
    set_nb_tiles(height * width);
    set_tiles(calloc(get_nb_tiles(), sizeof(struct tile)));
    if (! get_tiles()) {
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
        memcpy(even_diff, ((int[]) {
            -width, -width/2, (width/2)+1,
            width, width/2, -(width/2)-1
        }), sizeof even_diff);
        memcpy(odd_diff, ((int[]) {
            -width, -width/2, (width/2)+1,
            width, width/2, -(width/2)-1
        }), sizeof odd_diff);
    } else
    {
        memcpy(even_diff, ((int[]) {
            -width, -width+1, 1, width, -1, -width-1
        }),
        sizeof even_diff);
        memcpy(odd_diff, ((int[]) {
            -width, 1, width+1, width, width-1, -1
        }) ,
        sizeof odd_diff);
    }

    int r;
    int nb_one_fish = 0;
    // initializing and linking each tile
    for (int i = 0 ; i < get_nb_tiles() ; i ++)
    {
        set_tile_nb_fish(i, r = random_fishes());
	if (r == 1)
	    nb_one_fish ++;
        set_tile_nb_neighbours(i, 6);
        set_tile_penguin(i, EMPTY);
        set_tile_neighbours(i, calloc(6, sizeof(int)));
        if (! get_tile_neighbours(i)) {
            printf("init_full_hexa : could not allocate neighb of tile %d\n", i);
            return 0;
        }

        for (int j = 0 ; j < 6 ; j++)
        {
            if ((i % 2) == 0)
                set_tile_neighbour(i, j, i + even_diff[j]);
            else
                set_tile_neighbour(i, j, i + odd_diff[j]);

            // placing water on 2 sides
            if ((get_tile_neighbour(i,j) < 0) ||
                    (get_tile_neighbour(i,j) >= get_nb_tiles()))
                set_tile_neighbour(i, j, NO_TILE);
        }

        // and on the other 2 sides
        if ((i % width) == 0)
        {
            set_tile_neighbour(i, 4, NO_TILE);
            set_tile_neighbour(i, 5, NO_TILE);
        }
        if ((((width % 2) == 1) && (i % width) == (width/2)) ||
                (((width % 2) == 0) && (i % width) == (width-1)))
        {
            set_tile_neighbour(i, 1, NO_TILE);
            set_tile_neighbour(i, 2, NO_TILE);
        }
    }

    while (nb_one_fish < get_nb_players() * get_nb_penguins()) {
	int t = rand() % get_nb_tiles();
	if (get_tile_nb_fish(t) != 1) {
	    set_tile_nb_fish(t, 1);
	    nb_one_fish++;
	}
    }
    

    return 1;
}


int init_full_tetra(int width, int height)
{
    set_nb_tiles(height * width);
    set_tiles(calloc(get_nb_tiles(), sizeof(struct tile)));
    if (! get_tiles()) {
        printf("init_full_tetra : could not allocate tiles\n");
        return 0;
    }

    int diff[4] = { -width, 1, width, -1 };

    // initialize and link each tile
    int r;
    int nb_one_fish = 0;

    for (int i = 0 ; i < get_nb_tiles() ; i ++)
    {
	set_tile_nb_fish(i, r = random_fishes());
	if (r == 1)
	    nb_one_fish ++;
        set_tile_nb_neighbours(i, 4);
        set_tile_penguin(i, EMPTY);
        set_tile_neighbours(i, calloc(4, sizeof(int)));
        if (! get_tile_neighbours(i)) {
            printf("init_full_tetra : could not allocate neighb of tile %d\n", i);
            return 0;
        }

        for (int j = 0 ; j < 4 ; j++)
            set_tile_neighbour(i, j, i + diff[j]);

        // placing water on top, bottom, left and right
        if (i < width)
            set_tile_neighbour(i, 0, NO_TILE);
        if (i >= (get_nb_tiles() - width))
            set_tile_neighbour(i, 2, NO_TILE);
        if ((i % width) == 0)
            set_tile_neighbour(i, 3, NO_TILE);
        if ((i % width) == (width-1))
            set_tile_neighbour(i, 1, NO_TILE);
    }
    
    while (nb_one_fish < get_nb_players() * get_nb_penguins()) {
	int t = rand() % get_nb_tiles();
	if (get_tile_nb_fish(t) != 1) {
	    set_tile_nb_fish(t, 1);
	    nb_one_fish++;
	}
    }

    return 1;
}

// initializes the tiles of the graph g, in an octo tetra configuration of size width * height
int init_octo_tetra(int width, int height)
{
    set_nb_tiles(height * width);
    set_tiles(calloc(get_nb_tiles(), sizeof(struct tile)));
    if (! get_tiles()) {
        printf("init_octo_tetra : could not allocate tiles\n");
        return 0;
    }

    int tetra_diff[4] = { -width, 1, width, -1 };
    int octo_diff[8] = { -width, -width+1, 1, width+1, width, width-1, -1, -width-1 };

    // initialize and link each tile
    int r;
    int nb_one_fish = 0;
    for (int j = 0 ; j < height ; j++) {
        for (int i = 0 ; i < width ; i ++)
        {
            int nb = j*width + i;
            int nb_sides = (i%2 == j%2) ? 8 : 4;
	    set_tile_nb_fish(nb, r = random_fishes());
	    if (r == 1)
		nb_one_fish ++;
            set_tile_nb_neighbours(nb, nb_sides);
            set_tile_penguin(nb, EMPTY);
            set_tile_neighbours(nb, calloc(nb_sides, sizeof(int)));
            if (! get_tile_neighbours(nb)) {
                printf("init_octo_tetra : cant allocate neighb of tile %d\n", nb);
                return 0;
            }

            for (int k = 0 ; k < nb_sides ; k++)
                if (nb_sides == 8)
                    set_tile_neighbour(nb, k, nb + octo_diff[k]);
                else
                    set_tile_neighbour(nb, k, nb + tetra_diff[k]);

            // placing water on top, bottom, left and right
            if (j == 0) {
                set_tile_neighbour(nb, 0, NO_TILE);
                if (nb_sides == 8) {
                    set_tile_neighbour(nb, 1, NO_TILE);
                    set_tile_neighbour(nb, 7, NO_TILE);
                }
            }
            if (j == height-1) {
                if (nb_sides == 8) {
                    set_tile_neighbour(nb, 3, NO_TILE);
                    set_tile_neighbour(nb, 4, NO_TILE);
                    set_tile_neighbour(nb, 5, NO_TILE);
                } else {
                    set_tile_neighbour(nb, 2, NO_TILE);
                }
            }
            if (i == 0) {
                if (nb_sides == 8) {
                    set_tile_neighbour(nb, 5, NO_TILE);
                    set_tile_neighbour(nb, 6, NO_TILE);
                    set_tile_neighbour(nb, 7, NO_TILE);
                } else {
                    set_tile_neighbour(nb, 3, NO_TILE);
                }
            }
            if (i == width-1) {
                set_tile_neighbour(nb, 1, NO_TILE);
                if (nb_sides == 8) {
                    set_tile_neighbour(nb, 2, NO_TILE);
                    set_tile_neighbour(nb, 3, NO_TILE);
                }
            }
        }
    }
    
    while (nb_one_fish < get_nb_players() * get_nb_penguins()) {
	int t = rand() % get_nb_tiles();
	if (get_tile_nb_fish(t) != 1) {
	    set_tile_nb_fish(t, 1);
	    nb_one_fish++;
	}
    }

    return 1;
}


int random_fishes(void)
{
    int draw = rand() % (ONE_FISH + TWO_FISHES + THREE_FISHES);
    if (draw < ONE_FISH)
        return 1;
    else if (draw < (ONE_FISH + TWO_FISHES))
        return 2;
    else
        return 3;
}


// STRATEGIES_INIT AUXILIARY FUNCTIONS


int create_init_data(struct edge **edges, size_t *size_edges, int ** fishes)
{
    // allocating memory
    *fishes = calloc(get_nb_tiles(), sizeof(int));
    if (! *fishes) {
        printf("create_init_data : could not allocate fishes\n");
        return 0;
    }

    *size_edges = 0;
    *edges = NULL;
    // browsing graph
    int nb;
    for (int i = 0 ; i < get_nb_tiles() ; i++)
    {
        nb = get_tile_nb_neighbours(i);

        // allocating
        *edges = realloc(*edges, (*size_edges + nb) * sizeof(struct edge));
        if (*edges == NULL) {
            printf("create_init_data : could not allocate edges\n");
            free(* fishes);
            return 0;
        }
        // initializing
        for (int j = 0 ; j < nb ; j++)
        {
            (*edges)[*size_edges].src = i;
            (*edges)[*size_edges].dst = get_tile_neighbour(i, j);
            (*size_edges)++;
        }
        (*fishes)[i] = get_tile_nb_fish(i);
    }

    return 1;
}

void free_init_data(struct edge** edges, int ** fishes) {
    free(*fishes);
    free(*edges);
}


// PLACE_PENGUINS AUXILIARY FUNCTIONS


void update_previous_moves(struct move *prev_moves, struct move mv, int turn,
                           int nb_players_in_game)
{
    if (turn < 1)
        prev_moves[mv.player] = mv;
    else
    {

        // after first turn, the values of the array
        // must be shifted to the left once every move
        // and the new move must be added in the nb_players_in_game - 1 cell
        for (int i = 0; i < nb_players_in_game-1 ; i++)
            prev_moves[i] = prev_moves[i+1];

        prev_moves[nb_players_in_game-1] = mv;
    }
}




int check_n_place(int player_id, struct move* place, int turn_nb)
{
    // checking if tile is available and not melted
    if (get_tile_penguin(place->dst) != EMPTY) {
        printf("Player %d tried to place on non-empty or melted tile %d\n",
               player_id, place->dst);
        return 0;
    }

    // anti identity theft check
    if (place->player != player_id) {
        printf("Player %d tried to place a penguin as player %d\n",
               player_id, place->player);
        return 0;
    }
    
    // checking if there is only one fish on the tile
    if (get_tile_nb_fish(place->dst) != 1) {
	printf("Player %d tried to place on a tile with more than 1 fish on it\n",
	       player_id);
	return 0;
    }


    // placing penguin
    printf("SERVER : Player %d placed on tile %d\n", player_id, place->dst);
    int penguin_id = player_id * get_nb_penguins() + turn_nb;
    set_tile_penguin(place->dst,  penguin_id);
    set_penguin_pos(player_id, turn_nb, place->dst);

    return 1;
}


// PLAY_GAME AUXILIARY FUNCTIONS


int is_blocked(int p) {
    for (int i = 0; i < get_nb_penguins(); i++)
    {
        int tile = get_penguin_pos(p, i);

        for (int j = 0; j < get_tile_nb_neighbours(tile) ; j++)
        {
            int neighbour = get_tile_neighbour(tile, j);
            if ((neighbour != NO_TILE) && (get_tile_penguin(neighbour) == EMPTY))
                return 0;
        }
    }
    return 1;
}



int check_n_play(int player_id, struct move* play) 
{
    // checking if src tile is on the board
    if ((play->src < 0) ||
            (play->src >= get_nb_tiles())) {
        printf("Player %d tried to move from an inexistant tile : %d\n",
               player_id, play->src);
        return 0;
    }

    // checking if tile is available
    if (get_tile_penguin(play->dst) != EMPTY) {
        printf("Player %d tried to move on a non empty or melted tile\n",
               player_id);
        return 0;
    }

    // checking if moving a penguin
    if (penguin_owner(get_tile_penguin(play->src)) != player_id) {
        printf("Player %d tried to move someone else's / no penguin\n",
               player_id);
        return 0;
    }

    // anti identity theft check
    if (play->player != player_id) {
        printf("Player %d tried to move a penguin as player %d\n",
               player_id, play->player);
        return 0;
    }

    // checking if dst is reachable
    if (! is_reachable(play->src, play->dst)) {
        printf("Player %d tried to move to an inaccessible tile\n", player_id);
        return 0;
    }

    // placing penguin
    int penguin = get_tile_penguin(play->src);
    set_tile_penguin(play->dst, penguin);
    set_penguin_pos(penguin_owner(penguin), penguin_number(penguin), play->dst);

    // updating score
    set_score(player_id, get_score(player_id) + get_tile_nb_fish(play->src));

    // removing src tile
    set_tile_penguin(play->src, NO_TILE);
    disconnect_tile(play->src);

    return 1;
}


int is_reachable(int src, int dst)
{
    // looking for dst in all directions
    int found = 0;
    int direction = 0;
    while (!found && (direction < get_tile_nb_neighbours(src)))
    {
        int prec = src;
        int aux = get_tile_neighbour(src, direction);

        while (!found && (aux != NO_TILE) &&
                (get_tile_penguin(aux) == EMPTY))
        {
            if (aux == dst)
                found = 1;
            else {
                int tmp = aux;
                aux = opposite(aux, prec);
                prec = tmp;
            }
        }
        direction++;
    }

    return found;
}


void disconnect_tile(int tile)
{
    // for each neighbour n of tile
    for (int i = 0 ; i < get_tile_nb_neighbours(tile) ; i ++)
    {
        int n = get_tile_neighbour(tile, i);
        if (n != NO_TILE)
        {
            // looking for tile in the neighbours of n
            int j = 0;
            while (get_tile_neighbour(n, j) != tile)
                j++;
            // removing it
            set_tile_neighbour(n, j, NO_TILE);
            // removing n from tile neighbours
            set_tile_neighbour(tile, i, NO_TILE);
        }
    }
}

int opposite(int t, int prec) {
    int prec_dir = 0;
    int max_dir = get_tile_nb_neighbours(t);
    while ((prec_dir < max_dir) &&
            (get_tile_neighbour(t, prec_dir) != prec))
        prec_dir ++;
    if (prec_dir == max_dir) {
        printf("opposite : non adjacent tiles\n");
        return NO_TILE;
    }
    else {
        int opp_dir = (prec_dir + max_dir/2) % max_dir;
        return get_tile_neighbour(t, opp_dir);
    }
}


// END_GAME AUXILIARY FUNCTIONS
//
void stategies_done(void) 
{
    // calling freeing functions of each strategie
    for (int i = 0 ; i < get_nb_strats() ; i++)
        get_strat(i)->done();
}



// Debugging functions


void print_tiles(void)
{
    printf("###### Tiles ######\n\n");
    printf("ID |  nb_fish | nb_neighbours  | penguin\n");
    printf("----------------------------------------\n");
    for(int i=0; i<get_nb_tiles() ; i++)
    {
        printf("%d |   %d      |      %d         |    %d\n",
               i,
               get_tile_nb_fish(i),
               get_tile_nb_neighbours(i),
               get_tile_penguin(i));
    }
    printf("\n\n");
}


void print_server(struct server* s)
{
    printf("#### Server initialized ####\n");
    printf("nb_strategies : %zu \n", s->nb_strategies);

    printf("nb_tiles : %zu \n", s->g->nb_tiles);
    printf("nb_players : %zu \n", s->g->nb_players);
    printf("nb_penguins by player : %zu \n", s->g->nb_penguins);

    print_tiles();

}
