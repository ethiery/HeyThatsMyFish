#ifndef _FORDFULKERSON_H_
#define _FORDFULKERSON_H_

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "queue.h"
#include <string.h>

//--------STRUCT W_ARC-----------

struct w_arc {
    int src;
    int dst;
    int weight;
};

void set_arc_src(struct w_arc* e, int src);
void set_arc_dst(struct w_arc* e, int dst);
void set_arc_weight(struct w_arc* e, int weight);
int arc_src(struct w_arc* e);
int arc_dst(struct w_arc* e);
int arc_weight(struct w_arc* e);

// bonus
void arc_add_weight(struct w_arc *e, int w);


//---------STRUCT NETWORK-----------

struct network {
    int nb_vertices;
    int *vertices;  // vertices[i] = i,  constant array made to be pointed on
    int src;
    int dst;
    int *degrees;   // degrees[i] is the degree of vertex i
    struct w_arc **arcs;  // arcs[i] is the array of arcs going out of i
};

void set_net_nb_vertices(struct network* n, int nb);
void set_net_vertices(struct network* n, int* vertices);
void set_net_vertex(struct network* n, int v, int val);
void set_net_src(struct network* n, int src);
void set_net_dst(struct network* n, int dst);
void set_net_degrees(struct network* n, int* degrees);
void set_net_degree(struct network* n, int v, int deg);
void set_net_arcs(struct network* n, struct w_arc **arcs);
void set_net_arcs_array(struct network* n, int v, struct w_arc *a_array);
void set_net_arc(struct network* n, int v, int nb, struct w_arc * a);

int net_nb_vertices(struct network* n);
int *net_vertices(struct network* n);
int *net_vertex(struct network* n, int v);
int net_src(struct network* n);
int net_dst(struct network* n);
int *net_degrees(struct network* n);
int net_degree(struct network* n, int v);
struct w_arc *net_arcs_array(struct network* n, int v);
struct w_arc *net_arc(struct network* n, int v, int nb);

void net_free(struct network* n);

// bonus handy functions

// returns the arc from v to w (NULL if it does not exist)
struct w_arc *arc_between(struct network* n, int v, int w);

// returns the i-th vertex adjacent to vertex v in network n
int next_vertex(struct network* n, int v, int i);

void print_network(struct network *n);

// -----------STRUCT PATH-----------

struct path {
    struct queue *arcs;
    int min_capacity;
};

struct queue *path_arcs(struct path *p);
int path_min_capacity(struct path *p);
void set_path_min_capacity(struct path *p, int c);
void set_path_arcs(struct path* p, struct queue *q);
void path_free(struct path* p);

//---------STRUCT CUT-------------

struct cut {
    int nb_arcs;
    struct w_arc **arcs;
};

int cut_nb_arcs(struct cut *c);
struct w_arc **cut_arcs(struct cut* c);
struct w_arc *cut_arc(struct cut *c, int i);
void set_cut_nb_arcs(struct cut *c, int nb);
void set_cut_arcs(struct cut *c, struct w_arc **arcs);
void set_cut_arc(struct cut *c, int i, struct w_arc *a);
void cut_free(struct cut *c);


//--------STRUCT V_CUT------------

// vertex-cut
struct v_cut {
    int nb_vertices;
    int *vertices;
};

int v_cut_nb_vertices(struct v_cut *c);
int *v_cut_vertices(struct v_cut* c);
int v_cut_vertex(struct v_cut *c, int i);
void set_v_cut_nb_vertices(struct v_cut *c, int nb);
void set_v_cut_vertices(struct v_cut *c, int *vertices);
void set_v_cut_vertex(struct v_cut *c, int i, int v);
void v_cut_free(struct v_cut *c);



// returns a path (from src to dst) in the network n from s to t 
// of capacity > 0 (as a queue of w_arc)
// or NULL if there is no path
struct path *augmenting_path(struct network *n);

// given an augmenting path, update n sending the minimal flow along the path
void update_capacities(struct network *n, struct path* p);

// returns the maximum flow of n, and turns n into the residual graph
int ford_fulkerson(struct network *n);

// returns a copy of a net
struct network * net_copy(struct network *n);

// returns a min-cut of the network n
struct cut* min_cut(struct network *n);


#endif //_FORDFULKERSON_H_


