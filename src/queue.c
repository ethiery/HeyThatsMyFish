#include "queue.h"
#include <stdio.h>

void print_queue(struct queue* q)
{
    printf("\nprint_queue : ");
    struct elmt* current = q->head;
    while (current != NULL) {
	printf("%d ", *(int *)current->val);
	current = current->next;
    }
    printf("\n");
}

struct queue *queue_init(void) 
{
    struct queue *q = malloc(sizeof(*q));
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void queue_free(struct queue* q) 
{
    while (!queue_is_empty(q))
	dequeue(q);
    free(q);
}
    
int queue_size (struct queue* q) 
{
    return q->size;
}

int queue_is_empty (struct queue* q) 
{
    return (q->size == 0);
}

void enqueue(struct queue* q, void* e)
{
    struct elmt *new = malloc(sizeof(*new));
    new->next = NULL;
    new->val = e;

    if (queue_is_empty(q)) 
    {
	q->head = new;
	q->tail = new;
    } 
    else
    {
	q->tail->next = new;
	q->tail = new;
    }
    q->size++;
}

void *dequeue(struct queue* q) {
    struct elmt *former_head = q->head;
    q->head = former_head->next;
    void *res = former_head->val;
    q->size--;
    free(former_head);
    return res;
}
