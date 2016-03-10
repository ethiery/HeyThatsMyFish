#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>

struct elmt;
struct elmt {
    void *val;
    struct elmt *next;
};

struct queue {
    int size;
    struct elmt *head;
    struct elmt *tail;
};

// return an empty queue
struct queue *queue_init(void);

// provided q is valid, frees q
// be careful, if q is not empty, void* elements contained in q will not be freed
void queue_free(struct queue* q);

// provided q is valid, returns the size of q
int queue_size (struct queue* q);

// provided q is valid, return 0 if q is not empty, another value else
int queue_is_empty (struct queue* q);

// provided q is valid, enqueue e in q
void enqueue(struct queue* q, void* e);

// provided q is valid and not empty,  dequeue the head of q and returns its value
void *dequeue(struct queue* q);


#endif //_QUEUE_H_
