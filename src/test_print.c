#include "test_print.h"


void print_consts(struct graph * gr)
{
    printf("****** Constants ******\n\n");
    printf("me -> %d\n",get_me(gr));
    printf("nb_tiles -> %d\n",get_nb_tiles(gr));
    printf("nb_players -> %d\n",get_nb_players(gr));
    printf("nb_penguins -> %d (initially, by player)\n",get_nb_penguins(gr));
    printf("\n\n");
}

void print_tiles(struct graph * gr)
{
    printf("****** Tiles ******\n\n");
    printf("ID |  nb_fish | nb_neighbours  | penguin\n");
    printf("----------------------------------------\n");
    for(int i = 0; i < get_nb_tiles(gr) ; i++)
    {
        printf("%d |   %d      |      %d         |    %d\n",
               i,
               get_tile_nb_fish(i, gr),
               get_tile_nb_neighbours(i, gr),
               get_tile_penguin(i, gr));
    }
    printf("\n\n");
}

void print_edges(struct graph *gr)
{
    printf("****** Edges ******\n\n");
    printf("src | dst\n");
    printf("---------\n");
    for(int i=0 ; i < get_nb_tiles(gr) ; i++)
    {
        printf("%d | ",i);
        for(int j=0 ; j < get_tile_nb_neighbours(i, gr) ; j++)
            printf("%d ", get_tile_neighbour(i, j, gr));
        printf("\n");
    }
    printf("\n");
}

void print_players(struct graph * gr)
{
    printf("****** Players ******\n\n");
    printf("Player | penguins (position)\n");
    printf("----------------------------\n");
    for(int i=0 ; i < get_nb_players(gr) ; i++)
    {
        printf("%d      | ",i);
        for(int j=0 ; j < get_nb_penguins(gr) ; j++)
            printf("%d\t",get_penguin_pos(i, j, gr));
        printf("\n");
    }
    printf("\n\n");
}

void print_graph(struct graph * gr)
{
    print_consts(gr);
    print_tiles(gr);
    print_edges(gr);
    print_players(gr);
}
/*
int main(int argc, char **argv)
{
    int width = 5;
    int height = 5;
    struct graph gr;

    init_full_tetra(width, height, &gr);
//    init_full_hexa(width, height, &gr);
//    init_octo_tetra(width, height, &gr);    

    print_graph(&gr);
 
    free(&gr);
    return 0;
}
*/
