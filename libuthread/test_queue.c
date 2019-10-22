#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "queue.h"

//typedef struct queue* queue_t;

void test_create(void)
{
	queue_t q;

	q = queue_create();
	assert(q != NULL);
	int retVal = queue_destroy(q);
	//printf("%d\n", q->numInQueue);
	assert(retVal != -1);
}

void test_queue_simple(void)
{
	queue_t q;
	int data = 3;
	int data2 = 5;
	int data3 = 6;
	int data4 = 7;
	int *ptr, *ptr2, *ptr4;
	//printf("%d\n",&data);
	q = queue_create();
	queue_enqueue(q, &data);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_enqueue(q, &data4);
	//queue_delete(q, &data3);
	//queue_delete(q, &data2);
	queue_delete(q, &data);
	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr4);
	printf("%d %d\n",ptr,&data2);
	printf("%d %d\n",ptr2,&data3);
	printf("%d %d\n",ptr4,&data4);
	assert(ptr == &data2);
}

int main()
{
	//test_create();
	test_queue_simple();
	return 0;
}

