#include "strat_dissekator.h"

int test_graph_to_network() {
    struct graph g;
    init_pawns(2, 1, &g);
    int fishes[] = { 1, 1, 1, 1};
    init_tiles(4, fishes, &g);
    struct edge edges[] = { (struct edge) { 0, NO_TILE },
			    (struct edge) { 0, 1 },
			    (struct edge) { 0, 2 },
			    (struct edge) { 0, NO_TILE },
			    (struct edge) { 1, NO_TILE },
			    (struct edge) { 1, NO_TILE },
			    (struct edge) { 1, 3 },
			    (struct edge) { 1, 0 },
			    (struct edge) { 2, 0 },
			    (struct edge) { 2, 3 },
			    (struct edge) { 2, NO_TILE },
			    (struct edge) { 2, NO_TILE },
			    (struct edge) { 3, 1 },
			    (struct edge) { 3, NO_TILE },
			    (struct edge) { 3, 2 },
			    (struct edge) { 3, NO_TILE },
    };

    init_neighbours(16, edges, &g);
    init_me(0, &g);
    
    struct network *n = graph_to_network(&g, 0, 3);

    int v0 = ((net_degree(n, 0) == 3) &&
	      (arc_src(net_arc(n, 0, 0)) == 0) &&
	      (arc_dst(net_arc(n, 0, 0)) == 1) &&
	      (arc_weight(net_arc(n, 0, 0)) == 1) &&
	      (arc_src(net_arc(n, 0, 1)) == 0) &&
	      (arc_dst(net_arc(n, 0, 1)) == 3) &&
	      (arc_weight(net_arc(n, 0, 1)) == 0) &&
	      (arc_src(net_arc(n, 0, 2)) == 0) &&
	      (arc_dst(net_arc(n, 0, 2)) == 5) &&
	      (arc_weight(net_arc(n, 0, 2)) == 0));
	
    int v1 = ((net_degree(n, 1) == 3) &&
	      (arc_src(net_arc(n, 1, 0)) == 1) &&
	      (arc_dst(net_arc(n, 1, 0)) == 0) &&
	      (arc_weight(net_arc(n, 1, 0)) == 0) &&
	      (arc_src(net_arc(n, 1, 1)) == 1) &&
	      (arc_dst(net_arc(n, 1, 1)) == 2) &&
	      (arc_weight(net_arc(n, 1, 1)) == INT_MAX) &&
	      (arc_src(net_arc(n, 1, 2)) == 1) &&
	      (arc_dst(net_arc(n, 1, 2)) == 4) &&
	      (arc_weight(net_arc(n, 1, 1)) == INT_MAX));

    int v2 = ((net_degree(n, 2) == 3) &&
	      (arc_src(net_arc(n, 2, 0)) == 2) &&
	      (arc_dst(net_arc(n, 2, 0)) == 3) &&
	      (arc_weight(net_arc(n, 2, 0)) == 1) &&
	      (arc_src(net_arc(n, 2, 1)) == 2) &&
	      (arc_dst(net_arc(n, 2, 1)) == 7) &&
	      (arc_weight(net_arc(n, 2, 1)) == 0) &&
	      (arc_src(net_arc(n, 2, 2)) == 2) &&
	      (arc_dst(net_arc(n, 2, 2)) == 1) &&
	      (arc_weight(net_arc(n, 2, 2)) == 0));

    int v3 = ((net_degree(n, 3) == 3) &&
	      (arc_src(net_arc(n, 3, 0)) == 3) &&
	      (arc_dst(net_arc(n, 3, 0)) == 2) &&
	      (arc_weight(net_arc(n, 3, 0)) == 0) &&
	      (arc_src(net_arc(n, 3, 1)) == 3) &&
	      (arc_dst(net_arc(n, 3, 1)) == 6) &&
	      (arc_weight(net_arc(n, 3, 1)) == INT_MAX) &&
	      (arc_src(net_arc(n, 3, 2)) == 3) &&
	      (arc_dst(net_arc(n, 3, 2)) == 0) &&
	      (arc_weight(net_arc(n, 3, 2)) == INT_MAX));

    int v4 = ((net_degree(n, 4) == 3) &&
	      (arc_src(net_arc(n, 4, 0)) == 4) &&
	      (arc_dst(net_arc(n, 4, 0)) == 5) &&
	      (arc_weight(net_arc(n, 4, 0)) == 1) &&
	      (arc_src(net_arc(n, 4, 1)) == 4) &&
	      (arc_dst(net_arc(n, 4, 1)) == 1) &&
	      (arc_weight(net_arc(n, 4, 1)) == 0) &&
	      (arc_src(net_arc(n, 4, 2)) == 4) &&
	      (arc_dst(net_arc(n, 4, 2)) == 7) &&
	      (arc_weight(net_arc(n, 4, 2)) == 0));

    int v5 = ((net_degree(n, 5) == 3) &&
	      (arc_src(net_arc(n, 5, 0)) == 5) &&
	      (arc_dst(net_arc(n, 5, 0)) == 4) &&
	      (arc_weight(net_arc(n, 5, 0)) == 0) &&
	      (arc_src(net_arc(n, 5, 1)) == 5) &&
	      (arc_dst(net_arc(n, 5, 1)) == 0) &&
	      (arc_weight(net_arc(n, 5, 1)) == INT_MAX) &&
	      (arc_src(net_arc(n, 5, 2)) == 5) &&
	      (arc_dst(net_arc(n, 5, 2)) == 6) &&
	      (arc_weight(net_arc(n, 5, 2)) == INT_MAX));

    int v6 = ((net_degree(n, 6) == 3) &&
	      (arc_src(net_arc(n, 6, 0)) == 6) &&
	      (arc_dst(net_arc(n, 6, 0)) == 7) &&
	      (arc_weight(net_arc(n, 6, 0)) == 1) &&
	      (arc_src(net_arc(n, 6, 1)) == 6) &&
	      (arc_dst(net_arc(n, 6, 1)) == 3) &&
	      (arc_weight(net_arc(n, 6, 1)) == 0) &&
	      (arc_src(net_arc(n, 6, 2)) == 6) &&
	      (arc_dst(net_arc(n, 6, 2)) == 5) &&
	      (arc_weight(net_arc(n, 6, 2)) == 0));

    int v7 = ((net_degree(n, 7) == 3) &&
	      (arc_src(net_arc(n, 7, 0)) == 7) &&
	      (arc_dst(net_arc(n, 7, 0)) == 6) &&
	      (arc_weight(net_arc(n, 7, 0)) == 0) &&
	      (arc_src(net_arc(n, 7, 1)) == 7) &&
	      (arc_dst(net_arc(n, 7, 1)) == 2) &&
	      (arc_weight(net_arc(n, 7, 1)) == INT_MAX) &&
	      (arc_src(net_arc(n, 7, 2)) == 7) &&
	      (arc_dst(net_arc(n, 7, 2)) == 4) &&
	      (arc_weight(net_arc(n, 7, 2)) == INT_MAX));

    free_graph(&g);
    net_free(n);
    return (v0 && v1 && v2 && v3 && v4 && v5 && v6 && v6 && v7);
}

int test_cut_to_v_cut(void) 
{
    // initializing a cut
    struct cut *c = malloc(sizeof(*c));
    set_cut_nb_arcs(c, 2);
    struct w_arc** arcs = malloc(2*sizeof(*arcs));
    set_cut_arcs(c, arcs);
    struct w_arc a1 = {0, 1, 1};
    struct w_arc a2 = {4, 5, 1};
    set_cut_arc(c, 0, &a1);
    set_cut_arc(c, 1, &a2);
    
    struct v_cut *vc = cut_to_v_cut(c);
    int res = ((v_cut_nb_vertices(vc) == 2) &&
	   (v_cut_vertex(vc, 0) == 0) &&
	   (v_cut_vertex(vc, 1) == 2));

    cut_free(c);
    v_cut_free(vc);
    return res;
}

int test_v_min_cut(void)
{
    struct graph g;
    init_pawns(2, 1, &g);
    int fishes[] = { 1, 1, 1, 1};
    init_tiles(4, fishes, &g);
    struct edge edges[] = { (struct edge) { 0, NO_TILE },
			    (struct edge) { 0, 1 },
			    (struct edge) { 0, 2 },
			    (struct edge) { 0, NO_TILE },
			    (struct edge) { 1, NO_TILE },
			    (struct edge) { 1, NO_TILE },
			    (struct edge) { 1, 3 },
			    (struct edge) { 1, 0 },
			    (struct edge) { 2, 0 },
			    (struct edge) { 2, 3 },
			    (struct edge) { 2, NO_TILE },
			    (struct edge) { 2, NO_TILE },
			    (struct edge) { 3, 1 },
			    (struct edge) { 3, NO_TILE },
			    (struct edge) { 3, 2 },
			    (struct edge) { 3, NO_TILE },
    };

    init_neighbours(16, edges, &g);
    init_me(0, &g);
    
    struct v_cut *vc = v_min_cut(&g, 0, 3);
    int res = ((v_cut_vertex(vc, 0) == 1) &&
	       (v_cut_vertex(vc, 1) == 2));;

    v_cut_free(vc);
    free_graph(&g);
    return res;
}


int main (int argc, char** argv) {
    if (test_graph_to_network() &&
	test_cut_to_v_cut() &&
	test_v_min_cut())
	printf("tests for strat_dissekator passed successfully\n");


}
