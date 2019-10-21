#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
/*
typedef struct queue_Node {
	void* data;
	int numInQueue;
	queue_Node next;
	queue_Node prev;
};*/

struct queue {
	void* data;
	int numInQueue;
	queue_t next;
	queue_t prev;
};

//queue_t head = NULL;

queue_t queue_create(void)
{
	queue_t head = (queue_t)malloc(sizeof(struct queue));
	head->numInQueue = 0;

	return head;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->next != NULL)
		return -1;
	else
		free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)
		return -1;

	queue_t head = (queue_t)malloc(sizeof(struct queue));
	head->data = data;

	if (queue->next != NULL)
	{
		head->next = queue;
		queue->prev = head;
	}

	else
		queue = head;

	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL)
		return -1;

	queue_t current = queue;

	while (current->next != NULL)
		current = current->next;

	current->prev->next = NULL;

	*data = current->data;
	queue_destroy(current);

	return 0;
}/*

int queue_delete(queue_t queue, void *data)
{
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	
}

int queue_length(queue_t queue)
{
	
}*/

