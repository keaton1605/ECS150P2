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
	int *ptr, *ptr2;
	//printf("%d\n",&data);
	q = queue_create();
	queue_enqueue(q, &data);
	queue_enqueue(q, &data2);
	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr2);
	printf("%d %d\n",ptr,&data);
	printf("%d %d\n",ptr2,&data2);
	assert(ptr == &data);
}

int main()
{
	//test_create();
	test_queue_simple();
	return 0;
}

