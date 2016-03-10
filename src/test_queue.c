#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main (int argc, char** argv)
{
    struct queue *q = queue_init();
    assert(q != NULL);
    assert(queue_size(q) == 0);

    int e = 1;
    enqueue(q, &e);
    assert(queue_size(q) == 1);
    assert(*(int *)dequeue(q) == 1);
    
    int e2 = 2;
    enqueue(q, &e);
    enqueue(q, &e2);
    assert(queue_size(q) == 2);
    assert(*(int *)dequeue(q) == 1);
    assert(*(int *)dequeue(q) == 2);

    queue_free(q);
  
    printf("Tests of queue.c are successful\n");
    return EXIT_SUCCESS;

}

