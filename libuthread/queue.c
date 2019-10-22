#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct queue_Node {
	void* data;
	struct queue_Node* next;
	struct queue_Node* prev;
};

struct queue {
	int numInQueue;
	struct queue_Node* head;
	struct queue_Node* tail;
};

//queue_t head = NULL;

queue_t queue_create(void)
{
	queue_t newQueue = (queue_t)malloc(sizeof(struct queue));
	newQueue->numInQueue = 0;
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL)
		return -1;
	else if (queue->head != NULL && queue->head->next != NULL)
		return -1;
	else
		free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)
		return -1;

	struct queue_Node* newHead = (struct queue_Node*)malloc(sizeof(struct queue_Node));
	newHead->data = data;

	if (queue->head == NULL)
	{
		queue->head = newHead;
		queue->tail = newHead;
	}
	else
	{
		newHead->next = queue->head;
		queue->head->prev = newHead;
		queue->head = newHead;
	}
	queue->numInQueue += 1;

	//printf("%d\n", queue->numInQueue);
	//printf("%d\n", queue->head->data);
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL)
		return -1;

	*data = queue->tail->data;

	if (queue->tail == queue->head)
	{
		free(queue->tail);
		free(queue->head);
	}
	else
	{
		struct queue_Node* toDelete = queue->tail;
		queue->tail->prev->next = NULL;
		queue->tail = queue->tail->prev;
		toDelete->prev = NULL;
		free(toDelete);
	}
	queue->numInQueue -= 1;
	//printf("%d\n", queue->numInQueue);

	return 0;
}

int queue_delete(queue_t queue, void *data)
{

	return 0;
}/*

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	
}*/

int queue_length(queue_t queue)
{
	return queue->numInQueue;
}

