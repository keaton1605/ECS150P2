/*
 * Thread preemption
 *
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "preempt.h"
#include "queue.h"
#include "uthread.h"

int preemptDone = 0;

int hello(void* arg)
{
	printf("Hello world!\n");
	return 0;
}

void first_test(void)
{
	int retval = -1;
	int tid = uthread_create(hello,NULL);
	uthread_join(tid, &retval);
	assert(retval == 0);
}

int dontWait(void* arg)
{
	printf("printing before 'wait' complete.\n");
	preemptDone = 1;
	return 0;
}

int wait(void* arg)
{
	int tid = uthread_create(dontWait, NULL);
	while (preemptDone == 0) {};
	printf("printing after 'dontWait' complete.\n");
	uthread_join(tid, NULL);
	return 0;
}

void preempt_test(void)
{
	uthread_join(uthread_create(wait, NULL), NULL);
}


int main(void)
{
	//first_test();
	preempt_test();
	//uthread_join(uthread_create(thread1, NULL), NULL);
	return 0;
}
