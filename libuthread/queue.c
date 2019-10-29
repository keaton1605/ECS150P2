#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct queue_Node {
	void* data;
	struct queue_Node* next;
	struct queue_Node* prev;
}; // A struct of nodes for each item in the queue

struct queue {
	int numInQueue;
	struct queue_Node* head;
	struct queue_Node* tail;
}; // A struct containing pointers to the first and last members of the queue


queue_t queue_create(void)
{
	queue_t newQueue = (queue_t)malloc(sizeof(struct queue));
	newQueue->numInQueue = 0;
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
} // Allocates a new, empty queue

int queue_destroy(queue_t queue)
{
	if (queue == NULL)	// Error check
		return -1;
	else if (queue->head != NULL && queue->head->next != NULL)	// Error check
		return -1;
	else
	{
		free(queue->head);
		free(queue->tail);
		free(queue);
	}
	return 0;
} // Deallocates an empty queue

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)	// Error check
		return -1;

	struct queue_Node* newHead = (struct queue_Node*)malloc(sizeof(struct queue_Node));
	newHead->data = data;

	/* First item in queue */
	if (queue->head == NULL)
	{
		queue->head = newHead;
		queue->tail = newHead;
	} 

	/* Queue is not empty, moves head pointer */
	else
	{
		newHead->next = queue->head;
		queue->head->prev = newHead;
		queue->head = newHead;
	} 

	queue->numInQueue += 1;
	
	return 0;
} // Inserts a new item at the beginning of queue

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->head == NULL)	// Error check
		return -1;

	*data = queue->tail->data;

	/* Removes single item in queue */
	if (queue->tail == queue->head)
	{
		queue->tail = NULL;
		queue->head = NULL;
	} 

	/* Multiple items in queue, moves tail pointer */
	else
	{
		struct queue_Node* toDelete = queue->tail;
		queue->tail->prev->next = NULL;
		queue->tail = queue->tail->prev;
		toDelete->prev = NULL;
		free(toDelete);
	}

	queue->numInQueue -= 1;

	return 0;
} // Removes an item from end of queue

int queue_delete(queue_t queue, void *data)
{
	int i = 0;
	struct queue_Node* current = queue->tail;
	
	/* Iterates through queue until data is found or head is reached */
	for (i = queue->numInQueue; i > 0; i--)
	{
		if (current->data == data)
		{	
			if (current == queue->tail)			// Edge cases: 
				queue->tail = queue->tail->prev;	// Data is at head or tail.
			else if (current == queue->head)		// Delete is very similar to 
				queue->head = queue->head->next;	// Enqueue/Dequeue

			/* Make nodes before and after point to each other */
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
	return -1;	// Error check
} // Removes specific item from queue

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func == NULL)	// Error check
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
} // Performs function func on each node in queue

int queue_length(queue_t queue)
{
	if (queue == NULL)	// Error check
		return -1;

	return queue->numInQueue;
} // Returns length of queue
