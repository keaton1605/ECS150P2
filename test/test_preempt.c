#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <preempt.h>
#include <queue.h>
#include <uthread.h>

int preemptDone = 0;

int hello(void* arg)
{
	printf("Hello world!\n");
	return 0;
}

int dontWaitMore(void* arg)
{
	printf("printing before 'wait' complete.\n");
	preemptDone = 1;
	return 0;
} // Executes and increments global variable


int dontWait(void* arg)
{
	int tid = uthread_create(dontWaitMore, NULL);
	while (preemptDone != 1) {};
	printf("printing after 'dontWaitMore' complete.\n");
	preemptDone++;
	uthread_join(tid, NULL);
	return 0;
} // Waits for dontWaitMore to finish


int wait(void* arg)
{
	int tid = uthread_create(dontWait, NULL);
	while (preemptDone != 2) {};
	printf("printing after 'dontWait' complete.\n");
	uthread_join(tid, NULL);
	return 0;
} // Waits for dontWait to finish

void preempt_test(void)
{
	uthread_join(uthread_create(wait, NULL), NULL);
} // Calls join and creates thread


int main(void)
{
	preempt_test();
	return 0;
}
