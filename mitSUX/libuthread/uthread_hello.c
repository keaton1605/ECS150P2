/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"
#include "queue.h"

int hello(void* arg)
{	
	printf("Hello world!\n");
	return 0;
}

int hello2(void* arg)
{	
	printf("Hello world!!\n");
	return 0;
}

int hello3(void* arg)
{	
	printf("Hello world!!!\n");
	return 0;
}

int main(void)
{
	uthread_t tid;
	tid = uthread_create(hello, NULL);
	tid = uthread_create(hello2, NULL);
	tid = uthread_create(hello3, NULL);
	//uthread_join(tid, NULL);
	uthread_yield();
	//uthread_yield();
	uthread_join(43, 0);
	printf("hi\n");
	return 0;
}

