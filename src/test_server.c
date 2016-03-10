#include "server.c"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int test_correct_main_args(void)
{
    printf("Tests of function correct_main_args\n");
    printf("-----------------------------------\n");
    // missing arguments
    int argc = 5;
    char* argv[argc];
    argv[0] = "./launcher";
    argv[1] = "4";
    argv[2] = "full-hexa";
    argv[3] = "10";
    argv[4] = "10";
    printf("\nCalling correct_main_args with missing arguments\n");
    int test1 = ! correct_main_args(argc, argv);

    // incorrect number of penguins per player
    argc = 7;
    char* argv2[argc];
    argv2[0] = "./launcher";
    argv2[1] = "-4";
    argv2[2] = "full-hexa";
    argv2[3] = "10";
    argv2[4] = "10";
    argv2[5] = "./strat1.so";
    argv2[6] = "./strat2.so";
    printf("\nCalling correct_main_args with incorrect number of penguins per player\n");
    int test2 = ! correct_main_args(argc, argv2);

    // incorrect board configuration
    argv2[1] = "4";
    argv2[2] = "penrose";
    printf("\nCalling correct_main_args with unknown board configuration");
    int test3 = ! correct_main_args(argc, argv2);

    // too small board
    argv2[2] = "full-hexa";
    argv2[3] = "3";
    argv2[4] = "2";
    printf("\nCalling correct_main_args with a board of size too small for the number of penguin\n");
    int test4 = ! correct_main_args(argc, argv2);

    // correct arguments
    argv2[3] = "10";
    argv2[4] = "10";
    printf("\nCalling correct_main_args with correct arguments\n");
    int test5 = correct_main_args(argc, argv2);

    if (test1 && test2 && test3 && test4 && test5)
        printf("\nCORRECT_MAIN_ARGS : TEST OK");
    else
        printf("\nCORRECT_MAIN_ARGS : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3 && test4 && test5);

}

int test_server_load_strats(void) {
    printf("Test of function server_load_strats\n");
    printf("--------------------------------------\n");

    printf("\nLoading an inexistant strategy\n");
    set_serv(calloc(1, sizeof(*srv)));
    set_nb_strats(1);
    set_strats(calloc(1, sizeof(struct strategy)));
    char* bad_strats[1];
    bad_strats[0] = "inexistant_strat.so";
    int test1 = !server_load_strats(bad_strats);


    printf("\nLoading lib_strat_rand.so\n");
    char* strats[1];
    strats[0] = "./lib_strat_rand.so";
    int test2 = server_load_strats(strats);

    if (test1 && test2)
        printf("\nSERVER_LOAD_STRATS : TEST OK");
    else
        printf("\nSERVER_LOAD_STRATS : TEST FAILED");
    printf("\n\n\n");
    free_srv();

    return (test1 && test2);
}

int test_random_fishes(void) {
    srand(time(NULL));
    printf("Test of function random_fishes\n");
    printf("---------------------------------\n");
    int nb = 1000;
    printf("%d random draws with proportions %d / %d / %d\n", nb, ONE_FISH, TWO_FISHES, THREE_FISHES);
    int one = 0;
    int two = 0;
    int three = 0;
    for (int i = 0 ; i < nb ; i++)
        switch (random_fishes()) {
        case 1:
            one++;
            break;
        case 2:
            two++;
            break;
        case 3:
            three++;
            break;
        default:
            printf("Error : nb of fishes out of [1,3]\n");
            printf("\nRANDOM_FISHES : TEST FAILED");
            return 0;
            break;
        }

    printf("Proportions obtained : %f / %f / %f\n\n", ((float) one)/nb,
           ((float) two) / nb, ((float) three) / nb);

    int sum = ONE_FISH + TWO_FISHES + THREE_FISHES;
    int test1 = (fabsf(((float) one)/nb - ((float) ONE_FISH)/sum) < 0.1);
    int test2 = (fabsf(((float) two)/nb - ((float) TWO_FISHES)/sum) < 0.1);
    int test3 = (fabsf(((float) three)/nb - ((float) THREE_FISHES)/sum) < 0.1);

    if (test1 && test2 && test3)
        printf("\nRANDOM_FISHES : TEST OK");
    else
        printf("\nRANDOM_FISHES : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3);
}


int test_init_full_hexa(void)
{
    printf("Test of function init_full_hexa\n");
    printf("-------------------------------\n");

    printf("\nInitialization of a 3*3 graph\n");
    set_serv(calloc(1, sizeof(*srv)));
    set_graph(calloc(1, sizeof(struct graph)));

    int test1 = init_full_hexa(3,3);

    int test2 = (get_nb_tiles() == 3*3);

    int test3 = 1;
    for (int i = 0; i < get_nb_tiles(); i++)
        if ((get_tile_nb_fish(i) > 3) ||
                (get_tile_nb_fish(i) < 1) ||
                (get_tile_nb_neighbours(i) != 6) ||
                (get_tile_penguin(i) != EMPTY))
            test3 = 0;

    int test4 = ((get_tile_neighbour(0,0) == NO_TILE) &&
                 (get_tile_neighbour(0,1) == NO_TILE) &&
                 (get_tile_neighbour(0,2) == 2) &&
                 (get_tile_neighbour(0,3) == 3) &&
                 (get_tile_neighbour(0,4) == NO_TILE) &&
                 (get_tile_neighbour(0,5) == NO_TILE) &&
                 (get_tile_neighbour(1,0) == NO_TILE) &&
                 (get_tile_neighbour(1,1) == NO_TILE) &&
                 (get_tile_neighbour(1,2) == NO_TILE) &&
                 (get_tile_neighbour(1,3) == 4) &&
                 (get_tile_neighbour(1,4) == 2) &&
                 (get_tile_neighbour(1,5) == NO_TILE) &&
                 (get_tile_neighbour(2,0) == NO_TILE) &&
                 (get_tile_neighbour(2,1) == 1) &&
                 (get_tile_neighbour(2,2) == 4) &&
                 (get_tile_neighbour(2,3) == 5) &&
                 (get_tile_neighbour(2,4) == 3) &&
                 (get_tile_neighbour(2,5) == 0) &&
                 (get_tile_neighbour(3,0) == 0) &&
                 (get_tile_neighbour(3,1) == 2) &&
                 (get_tile_neighbour(3,2) == 5) &&
                 (get_tile_neighbour(3,3) == 6) &&
                 (get_tile_neighbour(3,4) == NO_TILE) &&
                 (get_tile_neighbour(3,5) == NO_TILE) &&
                 (get_tile_neighbour(4,0) == 1) &&
                 (get_tile_neighbour(4,1) == NO_TILE) &&
                 (get_tile_neighbour(4,2) == NO_TILE) &&
                 (get_tile_neighbour(4,3) == 7) &&
                 (get_tile_neighbour(4,4) == 5) &&
                 (get_tile_neighbour(4,5) == 2) &&
                 (get_tile_neighbour(5,0) == 2) &&
                 (get_tile_neighbour(5,1) == 4) &&
                 (get_tile_neighbour(5,2) == 7) &&
                 (get_tile_neighbour(5,3) == 8) &&
                 (get_tile_neighbour(5,4) == 6) &&
                 (get_tile_neighbour(5,5) == 3) &&
                 (get_tile_neighbour(6,0) == 3) &&
                 (get_tile_neighbour(6,1) == 5) &&
                 (get_tile_neighbour(6,2) == 8) &&
                 (get_tile_neighbour(6,3) == NO_TILE) &&
                 (get_tile_neighbour(6,4) == NO_TILE) &&
                 (get_tile_neighbour(6,5) == NO_TILE) &&
                 (get_tile_neighbour(7,0) == 4) &&
                 (get_tile_neighbour(7,1) == NO_TILE) &&
                 (get_tile_neighbour(7,2) == NO_TILE) &&
                 (get_tile_neighbour(7,3) == NO_TILE) &&
                 (get_tile_neighbour(7,4) == 8) &&
                 (get_tile_neighbour(7,5) == 5) &&
                 (get_tile_neighbour(8,0) == 5) &&
                 (get_tile_neighbour(8,1) == 7) &&
                 (get_tile_neighbour(8,2) == NO_TILE) &&
                 (get_tile_neighbour(8,3) == NO_TILE) &&
                 (get_tile_neighbour(8,4) == NO_TILE) &&
                 (get_tile_neighbour(8,5) == 6));

    free_srv();

    printf("\nInitialization of a 4*4 graph\n");
    set_serv(calloc(1, sizeof(*srv)));
    set_graph(calloc(1, sizeof(struct graph)));


    int test5 = init_full_hexa(4,4);

    int test6 = (get_nb_tiles() == 4*4);

    int test7 = 1;
    for (int i = 0; i < get_nb_tiles(); i++)
        if ((get_tile_nb_fish(i) > 3) ||
                (get_tile_nb_fish(i) < 1) ||
                (get_tile_nb_neighbours(i) != 6) ||
                (get_tile_penguin(i) != EMPTY))
            test7 = 0;

    int test8 = ((get_tile_neighbour(0,0) == NO_TILE) &&
                 (get_tile_neighbour(0,1) == NO_TILE) &&
                 (get_tile_neighbour(0,2) == 1) &&
                 (get_tile_neighbour(0,3) == 4) &&
                 (get_tile_neighbour(0,4) == NO_TILE) &&
                 (get_tile_neighbour(0,5) == NO_TILE) &&
                 (get_tile_neighbour(1,0) == NO_TILE) &&
                 (get_tile_neighbour(1,1) == 2) &&
                 (get_tile_neighbour(1,2) == 6) &&
                 (get_tile_neighbour(1,3) == 5) &&
                 (get_tile_neighbour(1,4) == 4) &&
                 (get_tile_neighbour(1,5) == 0) &&
                 (get_tile_neighbour(2,0) == NO_TILE) &&
                 (get_tile_neighbour(2,1) == NO_TILE) &&
                 (get_tile_neighbour(2,2) == 3) &&
                 (get_tile_neighbour(2,3) == 6) &&
                 (get_tile_neighbour(2,4) == 1) &&
                 (get_tile_neighbour(2,5) == NO_TILE) &&
                 (get_tile_neighbour(3,0) == NO_TILE) &&
                 (get_tile_neighbour(3,1) == NO_TILE) &&
                 (get_tile_neighbour(3,2) == NO_TILE) &&
                 (get_tile_neighbour(3,3) == 7) &&
                 (get_tile_neighbour(3,4) == 6) &&
                 (get_tile_neighbour(3,5) == 2) &&
                 (get_tile_neighbour(4,0) == 0) &&
                 (get_tile_neighbour(4,1) == 1) &&
                 (get_tile_neighbour(4,2) == 5) &&
                 (get_tile_neighbour(4,3) == 8) &&
                 (get_tile_neighbour(4,4) == NO_TILE) &&
                 (get_tile_neighbour(4,5) == NO_TILE) &&
                 (get_tile_neighbour(5,0) == 1) &&
                 (get_tile_neighbour(5,1) == 6) &&
                 (get_tile_neighbour(5,2) == 10) &&
                 (get_tile_neighbour(5,3) == 9) &&
                 (get_tile_neighbour(5,4) == 8) &&
                 (get_tile_neighbour(5,5) == 4) &&
                 (get_tile_neighbour(6,0) == 2) &&
                 (get_tile_neighbour(6,1) == 3) &&
                 (get_tile_neighbour(6,2) == 7) &&
                 (get_tile_neighbour(6,3) == 10) &&
                 (get_tile_neighbour(6,4) == 5) &&
                 (get_tile_neighbour(6,5) == 1) &&
                 (get_tile_neighbour(7,0) == 3) &&
                 (get_tile_neighbour(7,1) == NO_TILE) &&
                 (get_tile_neighbour(7,2) == NO_TILE) &&
                 (get_tile_neighbour(7,3) == 11) &&
                 (get_tile_neighbour(7,4) == 10) &&
                 (get_tile_neighbour(7,5) == 6) &&
                 (get_tile_neighbour(8,0) == 4) &&
                 (get_tile_neighbour(8,1) == 5) &&
                 (get_tile_neighbour(8,2) == 9) &&
                 (get_tile_neighbour(8,3) == 12) &&
                 (get_tile_neighbour(8,4) == NO_TILE) &&
                 (get_tile_neighbour(8,5) == NO_TILE) &&
                 (get_tile_neighbour(9,0) == 5) &&
                 (get_tile_neighbour(9,1) == 10) &&
                 (get_tile_neighbour(9,2) == 14) &&
                 (get_tile_neighbour(9,3) == 13) &&
                 (get_tile_neighbour(9,4) == 12) &&
                 (get_tile_neighbour(9,5) == 8) &&
                 (get_tile_neighbour(10,0) == 6) &&
                 (get_tile_neighbour(10,1) == 7) &&
                 (get_tile_neighbour(10,2) == 11) &&
                 (get_tile_neighbour(10,3) == 14) &&
                 (get_tile_neighbour(10,4) == 9) &&
                 (get_tile_neighbour(10,5) == 5) &&
                 (get_tile_neighbour(11,0) == 7) &&
                 (get_tile_neighbour(11,1) == NO_TILE) &&
                 (get_tile_neighbour(11,2) == NO_TILE) &&
                 (get_tile_neighbour(11,3) == 15) &&
                 (get_tile_neighbour(11,4) == 14) &&
                 (get_tile_neighbour(11,5) == 10) &&
                 (get_tile_neighbour(12,0) == 8) &&
                 (get_tile_neighbour(12,1) == 9) &&
                 (get_tile_neighbour(12,2) == 13) &&
                 (get_tile_neighbour(12,3) == NO_TILE) &&
                 (get_tile_neighbour(12,4) == NO_TILE) &&
                 (get_tile_neighbour(12,5) == NO_TILE) &&
                 (get_tile_neighbour(13,0) == 9) &&
                 (get_tile_neighbour(13,1) == 14) &&
                 (get_tile_neighbour(13,2) == NO_TILE) &&
                 (get_tile_neighbour(13,3) == NO_TILE) &&
                 (get_tile_neighbour(13,4) == NO_TILE) &&
                 (get_tile_neighbour(13,5) == 12) &&
                 (get_tile_neighbour(14,0) == 10) &&
                 (get_tile_neighbour(14,1) == 11) &&
                 (get_tile_neighbour(14,2) == 15) &&
                 (get_tile_neighbour(14,3) == NO_TILE) &&
                 (get_tile_neighbour(14,4) == 13) &&
                 (get_tile_neighbour(14,5) == 9) &&
                 (get_tile_neighbour(15,0) == 11) &&
                 (get_tile_neighbour(15,1) == NO_TILE) &&
                 (get_tile_neighbour(15,2) == NO_TILE) &&
                 (get_tile_neighbour(15,3) == NO_TILE) &&
                 (get_tile_neighbour(15,4) == NO_TILE) &&
                 (get_tile_neighbour(15,5) == 14));

    free_srv();

    if (test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8)
        printf("\nINIT_FULL_HEXA : TEST OK");
    else
        printf("\nINIT_FULL_HEXA : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8);



}


int test_init_full_tetra(void)
{
    printf("Test of function init_full_tetra\n");
    printf("-------------------------------\n");

    printf("\nInitialization of a 3*3 graph\n");
    set_serv(calloc(1, sizeof(*srv)));
    set_graph(calloc(1, sizeof(struct graph)));

    int test1 = init_full_tetra(3,3);

    int test2 = (get_nb_tiles() == 3*3);

    int test3 = 1;
    for (int i = 0; i < get_nb_tiles(); i++)
        if ((get_tile_nb_fish(i) > 3) ||
                (get_tile_nb_fish(i) < 1) ||
                (get_tile_nb_neighbours(i) != 4) ||
                (get_tile_penguin(i) != EMPTY))
            test3 = 0;

    int test4 = ((get_tile_neighbour(0,0) == NO_TILE) &&
                 (get_tile_neighbour(0,1) == 1) &&
                 (get_tile_neighbour(0,2) == 3) &&
                 (get_tile_neighbour(0,3) == NO_TILE) &&
                 (get_tile_neighbour(1,0) == NO_TILE) &&
                 (get_tile_neighbour(1,1) == 2) &&
                 (get_tile_neighbour(1,2) == 4) &&
                 (get_tile_neighbour(1,3) == 0) &&
                 (get_tile_neighbour(2,0) == NO_TILE) &&
                 (get_tile_neighbour(2,1) == NO_TILE) &&
                 (get_tile_neighbour(2,2) == 5) &&
                 (get_tile_neighbour(2,3) == 1) &&
                 (get_tile_neighbour(3,0) == 0) &&
                 (get_tile_neighbour(3,1) == 4) &&
                 (get_tile_neighbour(3,2) == 6) &&
                 (get_tile_neighbour(3,3) == NO_TILE) &&
                 (get_tile_neighbour(4,0) == 1) &&
                 (get_tile_neighbour(4,1) == 5) &&
                 (get_tile_neighbour(4,2) == 7) &&
                 (get_tile_neighbour(4,3) == 3) &&
                 (get_tile_neighbour(5,0) == 2) &&
                 (get_tile_neighbour(5,1) == NO_TILE) &&
                 (get_tile_neighbour(5,2) == 8) &&
                 (get_tile_neighbour(5,3) == 4) &&
                 (get_tile_neighbour(6,0) == 3) &&
                 (get_tile_neighbour(6,1) == 7) &&
                 (get_tile_neighbour(6,2) == NO_TILE) &&
                 (get_tile_neighbour(6,3) == NO_TILE) &&
                 (get_tile_neighbour(7,0) == 4) &&
                 (get_tile_neighbour(7,1) == 8) &&
                 (get_tile_neighbour(7,2) == NO_TILE) &&
                 (get_tile_neighbour(7,3) == 6) &&
                 (get_tile_neighbour(8,0) == 5) &&
                 (get_tile_neighbour(8,1) == NO_TILE) &&
                 (get_tile_neighbour(8,2) == NO_TILE) &&
                 (get_tile_neighbour(8,3) == 7));

    free_srv();

    if (test1 && test2 && test3 && test4)
        printf("\nINIT_FULL_TETRA : TEST OK");
    else
        printf("\nINIT_FULL_TETRA : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3 && test4);
}


int test_init_octo_tetra(void)
{
    printf("Test of function init_octo_tetra\n");
    printf("-------------------------------\n");

    printf("\nInitialization of a 3*3 graph\n");
    set_serv(calloc(1, sizeof(*srv)));
    set_graph(calloc(1, sizeof(struct graph)));

    int test1 = init_octo_tetra(3,3);

    int test2 = (get_nb_tiles() == 3*3);

    int test3 = 1;
    for (int i = 0; i < get_nb_tiles(); i++)
        if ((get_tile_nb_fish(i) > 3) ||
                (get_tile_nb_fish(i) < 1) ||
                (((i/3)%2 == (i%3)%2) && (get_tile_nb_neighbours(i) != 8)) ||
                (((i/3)%2 != (i%3)%2) && (get_tile_nb_neighbours(i) != 4)) ||
                (get_tile_penguin(i) != EMPTY))
            test3 = 0;

    int test4 = ((get_tile_neighbour(0,0) == NO_TILE) &&
                 (get_tile_neighbour(0,1) == NO_TILE) &&
                 (get_tile_neighbour(0,2) == 1) &&
                 (get_tile_neighbour(0,3) == 4) &&
                 (get_tile_neighbour(0,4) == 3) &&
                 (get_tile_neighbour(0,5) == NO_TILE) &&
                 (get_tile_neighbour(0,6) == NO_TILE) &&
                 (get_tile_neighbour(0,7) == NO_TILE) &&
                 (get_tile_neighbour(1,0) == NO_TILE) &&
                 (get_tile_neighbour(1,1) == 2) &&
                 (get_tile_neighbour(1,2) == 4) &&
                 (get_tile_neighbour(1,3) == 0) &&
                 (get_tile_neighbour(2,0) == NO_TILE) &&
                 (get_tile_neighbour(2,1) == NO_TILE) &&
                 (get_tile_neighbour(2,2) == NO_TILE) &&
                 (get_tile_neighbour(2,3) == NO_TILE) &&
                 (get_tile_neighbour(2,4) == 5) &&
                 (get_tile_neighbour(2,5) == 4) &&
                 (get_tile_neighbour(2,6) == 1) &&
                 (get_tile_neighbour(2,7) == NO_TILE) &&
                 (get_tile_neighbour(3,0) == 0) &&
                 (get_tile_neighbour(3,1) == 4) &&
                 (get_tile_neighbour(3,2) == 6) &&
                 (get_tile_neighbour(3,3) == NO_TILE) &&
                 (get_tile_neighbour(4,0) == 1) &&
                 (get_tile_neighbour(4,1) == 2) &&
                 (get_tile_neighbour(4,2) == 5) &&
                 (get_tile_neighbour(4,3) == 8) &&
                 (get_tile_neighbour(4,4) == 7) &&
                 (get_tile_neighbour(4,5) == 6) &&
                 (get_tile_neighbour(4,6) == 3) &&
                 (get_tile_neighbour(4,7) == 0) &&
                 (get_tile_neighbour(5,0) == 2) &&
                 (get_tile_neighbour(5,1) == NO_TILE) &&
                 (get_tile_neighbour(5,2) == 8) &&
                 (get_tile_neighbour(5,3) == 4) &&
                 (get_tile_neighbour(6,0) == 3) &&
                 (get_tile_neighbour(6,1) == 4) &&
                 (get_tile_neighbour(6,2) == 7) &&
                 (get_tile_neighbour(6,3) == NO_TILE) &&
                 (get_tile_neighbour(6,4) == NO_TILE) &&
                 (get_tile_neighbour(6,5) == NO_TILE) &&
                 (get_tile_neighbour(6,6) == NO_TILE) &&
                 (get_tile_neighbour(6,7) == NO_TILE) &&
                 (get_tile_neighbour(7,0) == 4) &&
                 (get_tile_neighbour(7,1) == 8) &&
                 (get_tile_neighbour(7,2) == NO_TILE) &&
                 (get_tile_neighbour(7,3) == 6) &&
                 (get_tile_neighbour(8,0) == 5) &&
                 (get_tile_neighbour(8,1) == NO_TILE) &&
                 (get_tile_neighbour(8,2) == NO_TILE) &&
                 (get_tile_neighbour(8,3) == NO_TILE) &&
                 (get_tile_neighbour(8,4) == NO_TILE) &&
                 (get_tile_neighbour(8,5) == NO_TILE) &&
                 (get_tile_neighbour(8,6) == 7) &&
                 (get_tile_neighbour(8,7) == 4));

    free_srv();

    if (test1 && test2 && test3 && test4)
        printf("\nINIT_OCTO_TETRA : TEST OK");
    else
        printf("\nINIT_OCTO_TETRA : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3 && test4);
}

int test_graph_init(void)
{
    printf("Test of function graph_init\n");
    printf("---------------------------\n");

    // getting in right situation
    set_serv(calloc(1, sizeof(*srv)));
    set_nb_strats(2);
    set_strats(calloc(2,sizeof(struct strategy)));
    char* strats[] = { "./lib_strat_rand.so", "./lib_strat_rand.so" };
    server_load_strats(strats);
    set_scores(calloc(2, sizeof(int)));

    printf("\nInitialization of a 7*8 graph, 2 players, 4 penguins per player\n");

    int test1 = graph_init("full-hexa", 7, 8, 2, 4);

    int test2 = (get_nb_players() == 2);

    int test3 = (get_nb_penguins() == 4);

    int test4 = (get_graph() != NULL);

    int test5 = 1;
    for (int i = 0 ; i < get_nb_players() ; i++)
        for (int j = 0 ; j < get_nb_penguins() ; j++)
            if (get_penguin_pos(i, j) != NO_TILE)
                test5 = 0;

    if (test1 && test2 && test3 && test4 && test5)
        printf("\nGRAPH_INIT : TEST OK");
    else
        printf("\nGRAPH_INIT : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3 && test4 && test5);
}

int test_server_init(void)
{
    printf("Test of function server_init\n");
    printf("---------------------------\n");

    printf("\nInitialization of the server, for a 2 player game, between rand_strat and rand_strat2\n");

    int argc = 7;
    char* argv[7] = { "./banquise", "4", "full-hexa", "7", "8", "./lib_strat_rand.so", "./lib_strat_rand.so" };

    int test1 = server_init(argc, argv);

    int test2 = (get_serv() != NULL);

    int test3 = (get_nb_strats() == 2);

    int test4 = (get_strats() != NULL);

    int test5 = (get_scores() != NULL);

    int test6 = (get_graph() != NULL);

    if (test1 && test2 && test3 && test4 && test5 && test6)
        printf("\nSERVER_INIT : TEST OK");
    else
        printf("\nSERVER_INIT : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3 && test4 && test5 && test6);
}

int test_create_init_data(void)
{
    printf("Test of function create_init_data\n");
    printf("---------------------------------\n");

    // getting in conditions
    int argc = 7;
    char *argv[] = { "./banquise", "2", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Creating_init_data corresponding to a 3*3 full tetra");
    struct edge *edges;
    int *fishes;
    size_t size;

    int test1 = create_init_data(&edges, &size, &fishes);

    int test2 = ((edges != NULL) && (fishes != NULL));

    int test3 = (size == 3*3*4);

    int test4 = ((edges[0].src == 0) &&
                 (edges[1].src == 0) &&
                 (edges[2].src == 0) &&
                 (edges[3].src == 0) &&
                 (edges[0].dst == NO_TILE) &&
                 (edges[1].dst == 1) &&
                 (edges[2].dst == 3) &&
                 (edges[3].dst == NO_TILE) &&
                 (edges[4].src == 1) &&
                 (edges[5].src == 1) &&
                 (edges[6].src == 1) &&
                 (edges[7].src == 1) &&
                 (edges[4].dst == NO_TILE) &&
                 (edges[5].dst == 2) &&
                 (edges[6].dst == 4) &&
                 (edges[7].dst == 0) &&
                 (edges[8].src == 2) &&
                 (edges[9].src == 2) &&
                 (edges[10].src == 2) &&
                 (edges[11].src == 2) &&
                 (edges[8].dst == NO_TILE) &&
                 (edges[9].dst == NO_TILE) &&
                 (edges[10].dst == 5) &&
                 (edges[11].dst == 1) &&
                 (edges[12].src == 3) &&
                 (edges[13].src == 3) &&
                 (edges[14].src == 3) &&
                 (edges[15].src == 3) &&
                 (edges[12].dst == 0) &&
                 (edges[13].dst == 4) &&
                 (edges[14].dst == 6) &&
                 (edges[15].dst == NO_TILE) &&
                 (edges[16].src == 4) &&
                 (edges[17].src == 4) &&
                 (edges[18].src == 4) &&
                 (edges[19].src == 4) &&
                 (edges[16].dst == 1) &&
                 (edges[17].dst == 5) &&
                 (edges[18].dst == 7) &&
                 (edges[19].dst == 3) &&
                 (edges[20].src == 5) &&
                 (edges[21].src == 5) &&
                 (edges[22].src == 5) &&
                 (edges[23].src == 5) &&
                 (edges[20].dst == 2) &&
                 (edges[21].dst == NO_TILE) &&
                 (edges[22].dst == 8) &&
                 (edges[23].dst == 4) &&
                 (edges[24].src == 6) &&
                 (edges[25].src == 6) &&
                 (edges[26].src == 6) &&
                 (edges[27].src == 6) &&
                 (edges[24].dst == 3) &&
                 (edges[25].dst == 7) &&
                 (edges[26].dst == NO_TILE) &&
                 (edges[27].dst == NO_TILE) &&
                 (edges[28].src == 7) &&
                 (edges[29].src == 7) &&
                 (edges[30].src == 7) &&
                 (edges[31].src == 7) &&
                 (edges[28].dst == 4) &&
                 (edges[29].dst == 8) &&
                 (edges[30].dst == NO_TILE) &&
                 (edges[31].dst == 6) &&
                 (edges[32].src == 8) &&
                 (edges[33].src == 8) &&
                 (edges[34].src == 8) &&
                 (edges[35].src == 8) &&
                 (edges[32].dst == 5) &&
                 (edges[33].dst == NO_TILE) &&
                 (edges[34].dst == NO_TILE) &&
                 (edges[35].dst == 7));

    int test5 = 1;
    for (int i = 0 ; i < get_nb_tiles() ; i++)
        if ((fishes[i] > 3 ) || (fishes[i] < 1))
            test5 = 0;

    if (test1 && test2 && test3 && test4 && test5)
        printf("\nCREATE_INIT_DATA : TEST OK");
    else
        printf("\nCREATE_INIT_DATA : TEST FAILED");
    printf("\n\n\n");

    free_srv();
    free(fishes);
    free(edges);

    return (test1 && test2 && test3 && test4 && test5);
}

int test_strategies_init(void)
{
    printf("Test of function strategies_init\n");
    printf("--------------------------------\n");

    // getting in conditions
    int argc = 7;
    char *argv[] = { "./banquise", "2", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("\nInitializing lib_strat_rand and lib_strat_rand2\n");
    int test1 = strategies_init();

    if (test1)
        printf("\nSTRATEGIES_INIT : TEST OK");
    else
        printf("\nSTRATEGIES_INIT : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return test1;
}

int test_update_previous_moves(void)
{
    printf("Test of function update_previous_moves\n");
    printf("--------------------------------------\n");

    struct move prev_moves[2];
    int turn = 0;
    int nb_players_in_game = 2;

    printf("Simulating a game with 2 players and 1 penguin per player\n");

    struct move mv;
    // player 0 places
    mv.player = 0;
    mv.src = -1;
    mv.dst = 0;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    int test1 = ((prev_moves[0].player == 0) &&
                 (prev_moves[0].src == -1) &&
                 (prev_moves[0].dst == 0));

    // player 1 places
    mv.player = 1;
    mv.src = -1;
    mv.dst = 1;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    int test2 = ((prev_moves[0].player == 0) &&
                 (prev_moves[0].src == -1) &&
                 (prev_moves[0].dst == 0) &&
                 (prev_moves[1].player == 1) &&
                 (prev_moves[1].src == -1) &&
                 (prev_moves[1].dst == 1));

    turn++;

    // player 0 moves
    mv.player = 0;
    mv.src = 0;
    mv.dst = 2;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    int test3 = ((prev_moves[0].player == 1) &&
                 (prev_moves[0].src == -1) &&
                 (prev_moves[0].dst == 1) &&
                 (prev_moves[1].player == 0) &&
                 (prev_moves[1].src == 0) &&
                 (prev_moves[1].dst == 2));

    // player 1 moves
    mv.player = 1;
    mv.src = 1;
    mv.dst = 3;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    int test4 = ((prev_moves[0].player == 0) &&
                 (prev_moves[0].src == 0) &&
                 (prev_moves[0].dst == 2) &&
                 (prev_moves[1].player == 1) &&
                 (prev_moves[1].src == 1) &&
                 (prev_moves[1].dst == 3));

    turn++;

    // player 0 has no moves available :
    mv.player = 0;
    mv.src = NO_TILE;
    mv.dst = NO_TILE;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    nb_players_in_game--;
    int test5 = ((prev_moves[0].player == 1) &&
                 (prev_moves[0].src == 1) &&
                 (prev_moves[0].dst == 3) &&
                 (prev_moves[1].player == 0) &&
                 (prev_moves[1].src == NO_TILE) &&
                 (prev_moves[1].dst == NO_TILE));

    // player 1 moves
    mv.player = 1;
    mv.src = 3;
    mv.dst = 4;
    update_previous_moves(prev_moves, mv, turn, nb_players_in_game);
    int test6 = ((prev_moves[0].player == 1) &&
                 (prev_moves[0].src == 3) &&
                 (prev_moves[0].dst == 4) &&
                 (prev_moves[1].player == 0) &&
                 (prev_moves[1].src == NO_TILE) &&
                 (prev_moves[1].dst == NO_TILE));


    if (test1 && test2 && test3 && test4 && test5 && test6)
        printf("\nUPDATE_PREVIOUS_MOVES : TEST OK");
    else
        printf("\nUPDATE_PREVIOUS_MOVES : TEST FAILED");
    printf("\n\n\n");

    return (test1 && test2 && test3 && test4 && test5 && test6);
}

int test_check_n_place(void)
{
    printf("Test of function check_n_place\n");
    printf("------------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra, 2 players, 1 penguin per player\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Trying to place player 0 penguin on available tile 0\n");
    struct move mv;
    mv.player = 0;
    mv.src = NO_TILE;
    mv.dst = 0;
    int test1 = check_n_place(0, &mv, 0);
    int test2 = ((get_penguin_pos(0, 0) == 0) && (get_tile_penguin(0) == 0));

    printf("Trying to place player 1 penguin on occupied tile 0\n");
    mv.player = 1;
    int test3 = ! check_n_place(1, &mv, 0);
    int test4 = ((get_penguin_pos(0, 0) == 0) && (get_tile_penguin(0) == 0) &&
                 (get_penguin_pos(1, 0) == NO_TILE));

    printf("Trying to place player 1 penguin on available tile 1 as player 0\n");
    mv.player = 0;
    mv.dst = 1;
    int test5 = !check_n_place(1, &mv, 0);

    if (test1 && test2 && test3 && test4 && test5)
        printf("\nCHECK_N_PLACE : TEST OK");
    else
        printf("\nCHECK_N_PLACE : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3 && test4 && test5);
}


int test_opposite(void)
{
    printf("Test of function opposite\n");
    printf("-------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Asking opposite regarding a non adjacent tile\n");
    int test1 = (opposite(0,2) == NO_TILE);

    printf("Checking results of opposite\n");
    int test2 = ((opposite(4, 1) == 7) &&
		 (opposite(4, 3) == 5) &&
		 (opposite(4, 7) == 1) &&
		 (opposite(4,5) == 3));

    if (test1 && test2)
        printf("\nOPPOSITE : TEST OK");
    else
        printf("\nOPPOSITE : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2);
}

int test_is_reachable(void)
{
    printf("Test of function is_reachable\n");
    printf("-----------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra, 2 players, 1 penguin\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Placing penguins\n");
    struct move mv;
    mv.player = 0;
    mv.src = NO_TILE;
    mv.dst = 0;
    check_n_place(0, &mv, 0);
    mv.player = 1;
    mv.dst = 1;
    check_n_place(1, &mv, 0);
    
    int test1;
    printf("Calling is_reachable with src and dst not aligned : %d\n",
	   !(test1 = !is_reachable(0, 8)));
    
    int test2;
    printf("Calling is_reachable with an obstacle between src and dst : %d\n",
	   !(test2 = !is_reachable(0, 2)));

    int test3;
    printf("Calling is_reachable with a reachable dst : %d\n",
	   test3 = is_reachable(0, 3));

    if (test1 && test2 && test3)
        printf("\nIS_REACHABLE : TEST OK");
    else
        printf("\nIS_REACHABLE : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3);
}

int test_disconnect_tile(void)
{
    printf("Test of function disconnect_tile\n");
    printf("------------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("disconnecting tile 4 and checking it's properly disconnected\n");
    disconnect_tile(4);
    int test1 = ((get_tile_neighbour(1, 2) == NO_TILE) &&
                 (get_tile_neighbour(3, 1) == NO_TILE) &&
                 (get_tile_neighbour(5, 3) == NO_TILE) &&
                 (get_tile_neighbour(7, 0) == NO_TILE));
    int test2 = 1;
    for (int i = 0 ; i < 4 ; i++)
        if (get_tile_neighbour(4, i) != NO_TILE)
            test2 = 0;

    printf("disconnecting tile 0 and checking it's properly disconnected\n");
    disconnect_tile(0);
    int test3 = ((get_tile_neighbour(1, 3) == NO_TILE) &&
                 (get_tile_neighbour(3, 0) == NO_TILE));
    int test4 = 1;
    for (int i = 0 ; i < 4 ; i++)
        if (get_tile_neighbour(0, i) != NO_TILE)
            test4 = 0;

    if (test1 && test2 && test3 && test4)
        printf("\nDISCONNECT_TILE : TEST OK");
    else
        printf("\nDISCONNECT_TILE : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3 && test4);
}

int test_is_blocked(void)
{
    printf("Test of function is_blocked\n");
    printf("------------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra, 2 players, 1 penguin per player\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Placing the penguins of player 0 and player 1\n");
    struct move mv;
    mv.player = 0;
    mv.src = NO_TILE;
    mv.dst = 0;
    check_n_place(0, &mv, 0);
    mv.player = 1;
    mv.dst = 4;
    check_n_place(1, &mv, 0);

    printf("Then removing tiles around player 0\n");
    disconnect_tile(1);
    disconnect_tile(3);
    printf("Checking that player 0 is consider blocked, and 1 not blocked\n");
    int test1 = is_blocked(0);
    int test2 = !is_blocked(1);

    if (test1 && test2)
        printf("\nIS_BLOCKED : TEST OK");
    else
        printf("\nIS_BLOCKED : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2);
}


int test_check_n_play(void)
{
    printf("Test of function check_n_play\n");
    printf("------------------------------\n");

    printf("\nInitializing a game with 3*3 full-tetra, 2 players, 1 penguin per player\n");
    int argc = 7;
    char *argv[] = { "./banquise", "1", "full-tetra", "3", "3", "./lib_strat_rand.so", "./lib_strat_rand2.so" };
    server_init(argc, argv);

    printf("Placing the penguins\n");
    struct move mv;
    mv.player = 0; mv.src = NO_TILE; mv.dst = 0;
    check_n_place(0, &mv, 0);
    mv.player = 1; mv.dst = 1;
    check_n_place(1, &mv, 0);

    printf("Trying to move a penguin from a tile outside the board\n");
    mv.player = 0; mv.src = 10; mv.dst = 2;
    int test1 = !check_n_play(0, &mv);
    
    printf("Trying to move a penguin from an empty tile\n");
    mv.player = 0; mv.src = 2; mv.dst = 3;
    int test2 = !check_n_play(0, &mv);

    printf("Trying to move a penguin to a tile where another penguin is\n");
    mv.player = 0; mv.src = 0; mv.dst = 1;
    int test3 = !check_n_play(0, &mv);

    printf("Trying to move another player's penguin\n");
    mv.player = 0; mv.src = 1; mv.dst = 2;
    int test4 = !check_n_play(0, &mv);
    
    printf("Trying to move a penguin as someone else\n");
    mv.player = 1; mv.src = 1; mv.dst = 2;
    int test5 = !check_n_play(0, &mv);

    printf("Trying to move on a tile not aligned with the src\n");
    mv.player = 0; mv.src = 0; mv.dst = 8;
    int test6 = !check_n_play(0, &mv);

    printf("Trying to move past another player\n");
    mv.player = 0; mv.src = 0; mv.dst = 2;
    int test7 = !check_n_play(0, &mv);

    printf("Trying a valid move\n");
    int fishes = get_tile_nb_fish(0);
    mv.player = 0; mv.src = 0; mv.dst = 3;
    int test8 = check_n_play(0, &mv);
    int test9 = ((get_tile_penguin(3) == 0) &&
		 (get_tile_penguin(0) == NO_TILE) &&
		 (get_penguin_pos(0, 0) == 3) &&
		 (get_score(0) == fishes));
    int test10 = ((get_tile_neighbour(1, 3) == NO_TILE) &&
		  (get_tile_neighbour(3, 0) == NO_TILE));
    for (int i = 0; i < get_tile_nb_neighbours(0) ; i++)
	if (get_tile_neighbour(0, i) != NO_TILE)
	    test10 = 0;
    
    printf("Trying to move to a melted tile\n");
    mv.player = 1; mv.src = 1; mv.dst = 0;
    int test11 = !check_n_play(1, &mv);

    if (test1 && test2 && test3 && test4 && test5 && test6 && test7 &&
	test8 && test9 && test10 && test11)
	printf("\nCHECK_N_PLAY : TEST OK");
    else
        printf("\nCHECK_N_PLAY : TEST FAILED");
    printf("\n\n\n");

    free_srv();

    return (test1 && test2 && test3 && test4 && test5 && test6 && test7 &&
	    test8 && test9 && test10 && test11);
}


int main (int argc, char** argv)
{
    if (test_correct_main_args() &&
	test_server_load_strats() &&
	test_random_fishes() &&
	test_init_full_hexa() &&
	test_init_full_tetra() &&
	test_init_octo_tetra() &&
	test_graph_init() &&
	test_server_init() &&
	test_create_init_data() &&
	test_strategies_init() &&
	test_update_previous_moves() &&
	test_check_n_place() &&
	test_opposite() &&
	test_is_reachable() &&
	test_disconnect_tile() &&
	test_is_blocked() &&
	test_check_n_play())
    {
        printf("server.c : All tests cleared\n");
        return EXIT_SUCCESS;
    }
    else
    {
        printf("server.c : something went wrong\n");
        return EXIT_FAILURE;
    }

}

