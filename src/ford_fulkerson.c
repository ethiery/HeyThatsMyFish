#include "ford_fulkerson.h"

// setters and getters for struct w_arc

void set_arc_src(struct w_arc* a, int src) {
    a->src = src;}

void set_arc_dst(struct w_arc* a, int dst) {
    a->dst = dst;}

void set_arc_weight(struct w_arc* a, int weight) {
    a->weight = weight;}

int arc_src(struct w_arc* a) {
    return a->src;}

int arc_dst(struct w_arc* a) {
    return a->dst;}

int arc_weight(struct w_arc* a) {
    return a->weight;}

void arc_add_weight(struct w_arc *a, int w) {
    a->weight += w;}

// setters and getters for struct network

void set_net_nb_vertices(struct network* n, int nb) {
    n->nb_vertices = nb;}

void set_net_vertices(struct network* n, int* vertices) {
    n->vertices = vertices;}

void set_net_vertex(struct network* n, int v, int val) {
    n->vertices[v] = val;}

void set_net_src(struct network* n, int src) {
    n->src = src;}

void set_net_dst(struct network* n, int dst) {
    n->dst = dst;}

void set_net_degrees(struct network* n, int* degrees) {
    n->degrees = degrees;}

void set_net_degree(struct network* n, int v, int deg) {
    n->degrees[v] = deg;}


void set_net_arcs(struct network* n, struct w_arc **arcs) {
    n->arcs = arcs;}

void set_net_arcs_array(struct network* n, int v, struct w_arc *a_array) {
    n->arcs[v] = a_array;}

void set_net_arc(struct network* n, int v, int nb, struct w_arc * a) {
    struct w_arc *tmp = net_arc(n, v, nb);
    set_arc_src(tmp, arc_src(a));
    set_arc_dst(tmp, arc_dst(a));
    set_arc_weight(tmp, arc_weight(a));
}

int net_nb_vertices(struct network* n) {
    return n->nb_vertices;}

int *net_vertices(struct network* n) {
    return n->vertices;}

int *net_vertex(struct network* n, int v) {
    return &(n->vertices[v]);}

int net_src(struct network* n) {
    return n->src;}

int net_dst(struct network* n) {
    return n->dst;}

int *net_degrees(struct network* n) {
    return n->degrees;}

int net_degree(struct network* n, int v) {
    return n->degrees[v];}

struct w_arc *net_arcs_array(struct network* n, int v) {
    return n->arcs[v];}

struct w_arc *net_arc(struct network* n, int v, int nb) {
    return &(n->arcs[v][nb]);}

void net_free(struct network* n) {
    free(n->vertices);
    free(n->degrees);
    for (int i = 0 ; i < n->nb_vertices ; i++)
	free(n->arcs[i]);
    free(n->arcs);
    free(n);
}

// bonus handy functions
struct w_arc *arc_between(struct network* n, int v, int w)
{
    int i;
    for (i = 0 ; (i < net_degree(n,v)) && (next_vertex(n, v, i) != w) ; i++);
    if (i < net_degree(n,v))
	return net_arc(n, v, i);
    else
	return NULL;
}

int next_vertex(struct network* n, int v, int i) {
    return arc_dst(net_arc(n, v, i));}

void print_network(struct network *n)
{
    for (int i = 0 ; i < net_nb_vertices(n) ; i++) {
	for (int j = 0 ; j < net_degree(n,i) ; j++) {
	    printf(" %d --- %d : %d\n", 
		   i, 
		   arc_dst(net_arc(n, i, j)),
		   arc_weight(net_arc(n, i, j)));
	}
    }
    printf("src = %d\n", net_src(n));
    printf("dst = %d\n", net_dst(n));
}


// setters and getters for struct path

struct queue *path_arcs(struct path *p) {
    return p->arcs;}

int path_min_capacity(struct path *p) {
    return p->min_capacity;}

void set_path_min_capacity(struct path *p, int c) {
    p->min_capacity = c;}

void set_path_arcs(struct path *p, struct queue *q) {
    p->arcs = q;}

void path_free(struct path* p) {
    while (!queue_is_empty(p->arcs))
	dequeue(p->arcs);
    queue_free(p->arcs);
    free(p);
}

// setters and getters for struct cut

int cut_nb_arcs(struct cut *c) {
    return c->nb_arcs;}

struct w_arc **cut_arcs(struct cut* c) {
    return c->arcs;}

struct w_arc *cut_arc(struct cut *c, int i) {
    return c->arcs[i];}

void set_cut_nb_arcs(struct cut *c, int nb) {
    c->nb_arcs = nb;}

void set_cut_arcs(struct cut *c, struct w_arc **arcs) {
    c->arcs = arcs;}

void set_cut_arc(struct cut *c, int i, struct w_arc *a) {
    c->arcs[i] = a;}

void cut_free(struct cut *c) {
    free(c->arcs);
    free(c);
}

// setters and getters for struct v_cut

int v_cut_nb_vertices(struct v_cut *c) {
    return c->nb_vertices;}

int *v_cut_vertices(struct v_cut* c) {
   return c->vertices;}

int v_cut_vertex(struct v_cut *c, int i) {
    return c->vertices[i];}

void set_v_cut_nb_vertices(struct v_cut *c, int nb) {
    c->nb_vertices = nb;}

void set_v_cut_vertices(struct v_cut *c, int *vertices) {
    c->vertices = vertices;}

void set_v_cut_vertex(struct v_cut *c, int i, int v) {
    c->vertices[i] = v;}

void v_cut_free(struct v_cut *c) {
    free(c->vertices);
    free(c);}



// read the table of predecessors pred, to backtrack from n's dst to n's src
// and returns the path obtained (arcs are given in reverse order)
struct path *backtrack(struct network *n, int* pred)
{
    // allocating and initializing result path
    struct path *p = malloc(sizeof(*p));
    p->min_capacity = INT_MAX;
    p->arcs = queue_init();
	
    // bactracing from dst to src, storing arcs, and updating min capacity
    int v = net_dst(n);
    while (v != net_src(n))
    {
	struct w_arc *a = arc_between(n, pred[v], v);

	if (arc_weight(a) < path_min_capacity(p))
	    set_path_min_capacity(p, arc_weight(a));

	enqueue(path_arcs(p), a);
	v = arc_src(a);
    }        
    return p;
}


struct path *augmenting_path(struct network *n) 
{
    // initializing array of predecessors to -1
    int pred[net_nb_vertices(n)];
    for (int i = 0 ; i < net_nb_vertices(n) ; i++)
	pred[i] = -1;

    // initializing the queue of arcs to visit
    struct queue *q = queue_init();
    enqueue(q, net_vertex(n, net_src(n)));
    pred[net_src(n)] = -2;

    // breadth-first searching  dst
    int found = 0;
    while (!found && !queue_is_empty(q))
    {
	int current = *(int *)dequeue(q);
	if (current == net_dst(n))
	    found = 1;
	else 
	    for (int i = 0 ; i < net_degree(n, current) ; i++) {
		if (arc_weight(net_arc(n, current, i)) > 0) 
		{
		    int *next = net_vertex(n, next_vertex(n, current, i));
		    if (pred[*next] == -1) 
		    {
			pred[*next] = current;
			enqueue(q, next);
		    }  
		}
	    }
	
    }

    queue_free(q);    
	    
    // if found, creating the path 
    if (!found)
	return NULL;
    else
	return backtrack(n, pred);
}
	
void update_capacities(struct network *n, struct path* p) 
{
    struct queue *q = path_arcs(p);

    int min_cap = path_min_capacity(p);
    while (!queue_is_empty(q))
    {
	struct w_arc *a = (struct w_arc*)dequeue(q);
	arc_add_weight(arc_between(n, arc_src(a), arc_dst(a)), -min_cap);
	arc_add_weight(arc_between(n, arc_dst(a), arc_src(a)), min_cap);
    }
}


int ford_fulkerson(struct network *n)
{
    int flow = 0;
    struct path *p;
    int turn = 0;

    while ((p = augmenting_path(n)) != NULL)
    {
	turn ++;
	update_capacities(n, p);
	flow += path_min_capacity(p);
	path_free(p);
    }
    
    return flow;
}


struct network * net_copy(struct network *n) 
{
    struct network* wn = malloc(sizeof(*n));

    set_net_nb_vertices(wn, net_nb_vertices(n));
    set_net_vertices(wn, malloc(net_nb_vertices(wn) * sizeof(int)));
    memcpy(net_vertices(wn), net_vertices(n), net_nb_vertices(wn) * sizeof(int));
    set_net_src(wn, net_src(n));
    set_net_dst(wn, net_dst(n));
    set_net_degrees(wn, malloc(net_nb_vertices(wn) * sizeof(int)));
    memcpy(net_degrees(wn), net_degrees(n), net_nb_vertices(wn) * sizeof(int));
    set_net_arcs(wn, malloc(net_nb_vertices(wn) * sizeof(struct w_arc*)));
    for (int i = 0 ; i < net_nb_vertices(wn) ; i++) 
    {
	set_net_arcs_array(wn, i, 
			    malloc(net_degree(wn, i) * sizeof(struct w_arc)));
	memcpy(net_arcs_array(wn, i), net_arcs_array(n, i),
	       net_degree(wn, i) * sizeof(struct w_arc));
    }
    return wn;
}

struct cut* min_cut(struct network *n) 
{
    // copying network, and computing residual graph of a maximum flow
    struct network* wn = net_copy(n);
    ford_fulkerson(wn);

    // flood filling the graph from the src

    int *marks = calloc(net_nb_vertices(wn), sizeof(int));

    struct queue *q = queue_init();
    enqueue(q, net_vertex(wn, net_src(wn)));
    marks[net_src(wn)] = 1;

    while (!queue_is_empty(q))
    {
	int current = *(int *)dequeue(q);
	
	for (int i = 0 ; i < net_degree(wn, current) ; i++)
	    if ((arc_weight(net_arc(wn, current,i)) > 0) &&
		(marks[next_vertex(wn, current, i)] == 0)) 
	    {
		marks[next_vertex(wn, current, i)] = 1;
		enqueue(q, net_vertex(wn, next_vertex(wn, current, i)));
	    }
    }

    // now the min cut is the arcs with src marked 1 and dst marked 2
    struct cut *res = malloc(sizeof(struct cut));
    set_cut_nb_arcs(res, 0);
    set_cut_arcs(res, NULL);
    
    for (int i = 0; i < net_nb_vertices(wn); i++) 
	if (marks[i] == 1)
	    for (int j = 0 ; j < net_degree(wn, i) ; j++) 
	    {
		int next = next_vertex(wn, i, j);
		if ((marks[next] == 0) && (arc_weight(net_arc(n, i, j)) > 0)) 
		{
		    set_cut_nb_arcs(res, cut_nb_arcs(res)+1);
		    set_cut_arcs(res, 
				  realloc(cut_arcs(res), 
					  cut_nb_arcs(res)*sizeof(struct w_arc*)
				      ));
		    set_cut_arc(res, cut_nb_arcs(res)-1, 
				 arc_between(n, i, next));
		}
	    }
    
    free(marks);
    queue_free(q);
    net_free(wn);
    return res;
}
