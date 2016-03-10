#include "ford_fulkerson.h"

void print_network(struct network *n)
{
    for (int i = 0 ; i < net_nb_vertices(n) ; i++) {
	for (int j = 0 ; j < net_degree(n,i) ; j++) {
	    printf(" %d --- %d : %d\n", 
		   i, 
		   edge_dst(net_edge(n, i, j)),
		   edge_weight(net_edge(n, i, j)));
	}
    }
    printf("src = %d\n", net_src(n));
    printf("dst = %d\n", net_dst(n));
}

int main (int argc, char** argv)
{
    // example from Denis Lapoire's graph lecture p80
    struct network *n = malloc(sizeof(*n));
    set_net_nb_vertices(n, 6);

    set_net_vertices(n, malloc(sizeof(int)* net_nb_vertices(n)));
    for (int i = 0 ; i < net_nb_vertices(n) ; i++)
	set_net_vertex(n, i, i);

    set_net_src(n, 0);
    set_net_dst(n, 5);
    
    set_net_degrees(n, malloc(sizeof(int)* net_nb_vertices(n)));
    set_net_degree(n, 0, 2); set_net_degree(n, 1, 4); set_net_degree(n, 2, 2);
    set_net_degree(n, 3, 3); set_net_degree(n, 4, 3); set_net_degree(n, 5, 2);

    set_net_edges(n, malloc(net_nb_vertices(n)*sizeof(struct w_edge*)));
    for (int i = 0 ; i < net_nb_vertices(n) ; i++)
	set_net_edges_array(n, i, 
			    malloc(net_degree(n, i) * sizeof(struct w_edge)));

    set_edge_src(net_edge(n, 0, 0), 0); 
    set_edge_dst(net_edge(n, 0, 0), 1); 
    set_edge_weight(net_edge(n, 0, 0), 3);

    set_edge_src(net_edge(n, 0, 1), 0); 
    set_edge_dst(net_edge(n, 0, 1), 3); 
    set_edge_weight(net_edge(n, 0, 1), 4);


    set_edge_src(net_edge(n, 1, 0), 1); 
    set_edge_dst(net_edge(n, 1, 0), 2); 
    set_edge_weight(net_edge(n, 1, 0), 1);

    set_edge_src(net_edge(n, 1, 1), 1); 
    set_edge_dst(net_edge(n, 1, 1), 3); 
    set_edge_weight(net_edge(n, 1, 1), 2);

    set_edge_src(net_edge(n, 1, 2), 1); 
    set_edge_dst(net_edge(n, 1, 2), 0); 
    set_edge_weight(net_edge(n, 1, 2), 0);

    set_edge_src(net_edge(n, 1, 3), 1); 
    set_edge_dst(net_edge(n, 1, 3), 4); 
    set_edge_weight(net_edge(n, 1, 3), 0);


    set_edge_src(net_edge(n, 2, 0), 2); 
    set_edge_dst(net_edge(n, 2, 0), 5); 
    set_edge_weight(net_edge(n, 2, 0), 4);

    set_edge_src(net_edge(n, 2, 1), 2); 
    set_edge_dst(net_edge(n, 2, 1), 1); 
    set_edge_weight(net_edge(n, 2, 1), 0);


    set_edge_src(net_edge(n, 3, 0), 3);
    set_edge_dst(net_edge(n, 3, 0), 0); 
    set_edge_weight(net_edge(n, 3, 0), 1);

    set_edge_src(net_edge(n, 3, 1), 3); 
    set_edge_dst(net_edge(n, 3, 1), 4); 
    set_edge_weight(net_edge(n, 3, 1), 4);
    
    set_edge_src(net_edge(n, 3, 2), 3); 
    set_edge_dst(net_edge(n, 3, 2), 1); 
    set_edge_weight(net_edge(n, 3, 2), 0);

    
    set_edge_src(net_edge(n, 4, 0), 4); 
    set_edge_dst(net_edge(n, 4, 0), 1); 
    set_edge_weight(net_edge(n, 4, 0), 3);

    set_edge_src(net_edge(n, 4, 1), 4); 
    set_edge_dst(net_edge(n, 4, 1), 5); 
    set_edge_weight(net_edge(n, 4, 1), 5);

    set_edge_src(net_edge(n, 4, 2), 4); 
    set_edge_dst(net_edge(n, 4, 2), 3); 
    set_edge_weight(net_edge(n, 4, 2), 0);

    set_edge_src(net_edge(n, 5, 0), 5); 
    set_edge_dst(net_edge(n, 5, 0), 2); 
    set_edge_weight(net_edge(n, 5, 0), 0);

    set_edge_src(net_edge(n, 5, 1), 5); 
    set_edge_dst(net_edge(n, 5, 1), 4); 
    set_edge_weight(net_edge(n, 5, 1), 0);

    
    //print_network(n);

    struct cut* mc = min_cut(n);

    if ((edge_src(cut_edge(mc, 0)) == 1) &&
	(edge_dst(cut_edge(mc, 0)) == 2) &&
	(edge_weight(cut_edge(mc, 0)) == 1) &&
	(edge_src(cut_edge(mc, 1)) == 3) &&
	(edge_dst(cut_edge(mc, 1)) == 4) &&
	(edge_weight(cut_edge(mc, 1)) == 4)) {
	printf("Test of ford_fulkerson.c passed successfuly\n");
	cut_free(mc);
	net_free(n);
	return EXIT_SUCCESS;
    } else {
	printf("Test of ford_fulkerson.c failed\n");
	cut_free(mc);
	net_free(n);
	return EXIT_FAILURE;
    }
}
