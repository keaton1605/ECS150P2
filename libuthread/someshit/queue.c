#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

//Helper functions
static int isEmpty(queue_t queue);

typedef struct node {
 void *data;
 struct node *next;
} node;

struct queue {
 struct node *head;
 struct node *tail;
 int length;
};

queue_t queue_create(void)
{
 queue_t queue;
 // Allocate a queue
 queue = (queue_t) malloc(sizeof(struct queue));
 // Set length to 0
 queue->length = 0;
 // Initialize head and tail
 queue->head = (node*) malloc(sizeof(node));
 queue->head->data = NULL;
 queue->head->next = NULL;
 queue->tail = queue->head;
 //Return the queue
 return queue;
}

int queue_destroy(queue_t queue)
{
 if (queue == NULL) { // If queue is NULL
 return -1;
 } else if ( queue->length != 0) { // If queue is not empty
 return -1;
 } else {
 free(queue->head);
 free(queue);
 }

 return 0;
}

int queue_enqueue(queue_t queue, void *data)
{ // check if queue or data is null
 if (queue == NULL || data == NULL) {
 return -1;
 }
 // don't make a new node if length is 0
 if (queue->length == 0) {
	queue->head->data = data;
 } else {
 queue->tail->next = (node*) malloc(sizeof(node));
 // check memory allocation error
 if (queue->tail->next == NULL){
 return -1;
 }
 // Setup new node with data
 queue->tail = queue->tail->next;
 queue->tail->next = NULL;
 queue->tail->data = data;
 }
 
 // Increment length
 (queue->length)++;
 return 0; 
}

int queue_dequeue(queue_t queue, void **data)
{ // check if queue or data is null
 if (queue == NULL || data == NULL) {
 return -1;
 }
 // Check if queue is empty
 if (queue->length == 0) {
 return -1;
 }
 // Set data to oldest item
 *data = queue->head->data;
 // Decrement length
 (queue->length)--;
 // Special case if queue is now empty
 if (queue->length == 0) {
 queue->head->data = NULL;
 } else { // Otherwise if items still in queue
 // Make a copy of the old head pointer
 node* oldHead = queue->head;
 // Set head to next in queue
 queue->head = queue->head->next;
 // free the old head node
 free(oldHead);
 }
 
 return 0; 
}

int queue_delete(queue_t queue, void *data)
{ // check if queue or data is null
 if (queue == NULL || data == NULL) {
 return -1;
 }
 // See if item is first in queue
 node *currentNode = queue->head; 
 if (currentNode->data == data) {
 void *ptr;
 queue_dequeue(queue, &ptr);
 } else { // Iterate until Item is found
 while (currentNode != NULL) {
 if (currentNode->next != NULL && currentNode->next->data == data) {
 node* deleteNode = currentNode->next;
 currentNode->next = currentNode->next->next;
 (queue->length)--;
 free(deleteNode);
 break;
 }
 currentNode = currentNode->next;
 }
 // If this point is reached, data does not exist
 return -1;
 }
 return 0;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{ //Check if queue or func are NULL
 if(queue == NULL || func == NULL) {
 return -1;
 }
 //Return if the queue is empty
 if(isEmpty(queue)) {
 return 0;
 }
 //Iterate through queue
 node *current = queue->head; //Start at the head

 while(current) {
 //Call the function. If 1, stop.
 if((*func)(current->data, arg) == 1) {
 //If data is not null, it receives data item where iteration stops
 if(data != NULL) {
 *data = current->data;
 return 1;
 }
 } 
 //Move to next node
 current = current->next;
 }

 return 0; 
}

int queue_length(queue_t queue)
{
	if( queue == NULL){
 return -1;
 }
 return queue->length; 
}

/* Helper functions */

//Check if queue is empty
static int isEmpty(queue_t queue)
{
 if(queue->length == 0)
 {
 return 1;
 }

 return 0;
}
