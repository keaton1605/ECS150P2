#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "queue.h"

void test_create(void)
{
	queue_t q;
	queue_t newQ;

	q = queue_create();
	//printf("%d\n",q->numInQueue);
	assert(q != NULL);
	int retVal = queue_destroy(q);
	assert(retVal != -1);
}

void test_queue_simple(void)
{
	queue_t q;
	int data = 3, *ptr;

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	assert(ptr == &data);
}

int main()
{
	//test_create();
	test_queue_simple();
	return 0;
}

