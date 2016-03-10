#ifndef _STRAT_DISS_H_
#define _STRAT_DISS_H_

#include <stdlib.h>
#include "strat.h"
#include "ford_fulkerson.h"

#define NB_TILES_TO_RUPTURE 3

struct moveSet moves_to_rupture_tile;

// transform a graph in a network
// each tile of the graph is split in 2 vertices linked by 2 edges of capacity 1,
// one going one way, the other going the other way (in both ways). 
// Edges are going in one of those 2 vertices, and out of the other
// Each edge of the graph is split in two weighted edges in the network 
// of capacity INT_MAX and going one way and the other
//
// for more information on the transformation of a max flow problem 
// with vertex capacities in a simple max flow problem
// check the maximum flow problem wikipedia page
struct network *graph_to_network (struct graph* g, int src, int dst);

// after transforming a graph on a network,
// it is possible to use it as an argument for min_cut (in ford_fulkerson.c)
// to obtain a cut
// cut_to_v_cut returns the corresponding minimal vertex cut, 
// which is the smallest array of vertices to delete to disconnect the graph

struct v_cut *cut_to_v_cut (struct cut* c);

// returns the same move than play() in strat_line
struct move play_as_line(struct move m, struct graph *g);    

#endif // _STRAT_DISS_H_
