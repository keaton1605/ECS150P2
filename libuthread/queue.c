#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue {
	/* TODO Phase 1 */
	void* data;
	int numInQueue;
	struct queue* next;
	struct queue* prev;
};

queue_t queue_create(void)
{
	struct queue* newQueue = (struct queue*)malloc(sizeof(struct queue));
	newQueue->data = NULL;
	newQueue->numInQueue = 9;
	newQueue->next = NULL;
	newQueue->prev = NULL;
	return newQueue;
}

/*int queue_destroy(queue_t queue)
{
	
}

int queue_enqueue(queue_t queue, void *data)
{
	
}

int queue_dequeue(queue_t queue, void **data)
{
	
}

int queue_delete(queue_t queue, void *data)
{
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	
}

int queue_length(queue_t queue)
{
	
}*/

