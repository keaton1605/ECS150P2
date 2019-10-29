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
	
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->head == NULL)
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
	int i = 0;
	struct queue_Node* current = queue->tail;
	
	for (i = queue->numInQueue; i > 0; i--)
	{
		if (current->data == data)
		{	
			if (current == queue->tail)
				queue->tail = queue->tail->prev;
			else if (current == queue->head)
				queue->head = queue->head->next;
			else
			{
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}

			free(current);
			queue->numInQueue -= 1;

			return 0;
		}
		current = current->prev;
	}
	free(current);
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func == NULL)
		return -1;
	
	struct queue_Node* current = queue->tail;

	while (current != NULL)
	{
		if (func(current->data, arg) == 1)
		{
			*data = current->data;
			return 1;
		}
		current = current->prev;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL)
		return -1;

	return queue->numInQueue;
}
