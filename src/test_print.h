#ifndef _TEST_PRINT_H_
#define _TEST_PRINT_H_


#include <stdio.h>
#include <stdlib.h>

#include "strat.h"
#include "test_strat.h"
#include "graph.h"

void print_consts(struct graph * gr);
void print_tiles(struct graph * gr);
void print_edges(struct graph *gr);
void print_players(struct graph * gr);
void init_edge(struct edge *edges, int *actual_edge, int i, int j);
void print_graph(struct graph * gr);



#endif // _TEST_PRINT_H_
 
