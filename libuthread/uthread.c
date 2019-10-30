#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

// Global Variables
struct thread* Main;
struct thread* Current;

int numThreads = 0;
static ucontext_t ctx;

// Queues for each state
queue_t READY_q;
queue_t BLOCKED_q;
queue_t ZOMB_q;


typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	ZOMBIE
} states; // Enum of states


struct thread
{
	uthread_t TID;
	uthread_t joinTID;
	void* stack;
	states state;
	ucontext_t *context;
	int retval;
}; // Thread Control Block

int tidFindFunc(void *data, void *arg)
{
	struct thread* temp = (struct thread*)data;
	int tid = (int)(long)arg;

	if(temp->TID == tid) 
		return 1;

	return 0;
} // Find specific TID's


int tidFind(queue_t newQ, void* data, int tid)
{
	int retval = queue_iterate(newQ, tidFindFunc, (void*)(long)tid, data);
	return retval;
} // Finds specific tid given a specific queue


int jointidFindFunc(void *data, void *arg)
{
	struct thread* temp = (struct thread*)data;
	int tid = (int)(long)arg;

	if(temp->joinTID == tid)
		return 1;

	return 0;
} // Find specific TID's for joined threads

void unBlock()
{
	struct thread* blocked;
	void *ptr = NULL;

	/* Checks blocked queue and reinserts into ready queue*/
	if (queue_iterate(BLOCKED_q, jointidFindFunc, (void*)(long)Current->TID, &ptr) == 1)
	{
		blocked = (struct thread*)ptr;
		blocked->joinTID = 0;
		queue_delete(BLOCKED_q, &blocked);
		queue_enqueue(READY_q, blocked);
	}
	return;
} // Unblock parent after child exits


void uthread_yield(void)
{
	if (READY_q == NULL)
		return;
	
	struct thread* prev;
	struct thread* temp;

	preempt_disable();	

	/* Change current state to ready and enqueue */
	Current->state = READY;
	temp = Current;
	queue_enqueue(READY_q, (void**)Current);

	/* Dequeue next thread */
	queue_dequeue(READY_q, (void**)&prev);	

	/* Change previous thread to Current */
	Current = prev;
	Current->state = RUNNING;

	preempt_enable();
		
	uthread_ctx_switch(temp->context, Current->context);
}


uthread_t uthread_self(void)
{
	return Current->TID;
} // Returns Current threads TID


int uthread_create(uthread_func_t func, void *arg)
{
	/* Initialize the main thread */
	if (numThreads == 0)
	{
		struct thread* newThread1 = (struct thread*)malloc(sizeof(struct thread));

		newThread1->TID = numThreads;
		newThread1->joinTID = -1;
		newThread1->stack = NULL;
		newThread1->state = READY;
		newThread1->context = &ctx;

		Main = newThread1;
		Current = newThread1;
		READY_q = queue_create();
		preempt_start();
	}

	preempt_disable();

	/* Initialize a child thread */		
	struct thread* newThread = (struct thread*)malloc(sizeof(struct thread));

	newThread->TID = ++numThreads;
	newThread->joinTID = -1;
	newThread->stack = uthread_ctx_alloc_stack();
	newThread->state = READY;
	newThread->context = malloc(sizeof(uthread_ctx_t));

	uthread_ctx_init(newThread->context, newThread->stack, func, arg);
	queue_enqueue(READY_q, newThread);

	preempt_enable();

	return newThread->TID;
}


void uthread_exit(int retval)
{
	/* Check if any processes have already exited */
	if (ZOMB_q == NULL)
		ZOMB_q = queue_create();

	struct thread* prev;
	struct thread* temp;

	temp = Current;

	preempt_disable();
	
	/* Put current thread in Zombie mode */
	Current->retval = retval;
	Current->state = ZOMBIE;
	queue_enqueue(ZOMB_q, Current);

	/* Put next thread in Current, unBlock any threads blocked by thread about to switch to zombie */
	if (queue_length(READY_q) != 0)
		queue_dequeue(READY_q, (void**)&prev);
	else
		prev = Main;

	unBlock();

	Current = prev;

	preempt_enable();
	
	uthread_ctx_switch(temp->context, Current->context);
}

int uthread_join(uthread_t tid, int *retval)
{
	struct thread* Join;
	struct thread* deadThread;

	if (BLOCKED_q == NULL)
		BLOCKED_q = queue_create();

	preempt_disable();

	/* Check if tid is valid */
	if (tid == 0 || Current->TID == tid)	
		return -1;
	
	preempt_enable();

	/* Check if TID is in Zombie state */
	if (tidFind(ZOMB_q, &Join, tid) == 1)
	{
		queue_dequeue(ZOMB_q, (void**)&deadThread);
		if (retval != NULL)
			*retval = deadThread->retval;

		uthread_ctx_destroy_stack(deadThread->stack);
		free(deadThread);
	}
	
	/* If not in zombie state, but still a thread, block current thread and switch context */
	else if (tidFind(BLOCKED_q, &Join, tid) == 1 || tidFind(READY_q, &Join, tid) == 1)
	{
		struct thread* tempRet;
		preempt_disable();

		Current->state = BLOCKED;
		Current->joinTID = tid;
		queue_enqueue(BLOCKED_q, Current);

		tempRet = Join;

		Join = Current;
		queue_dequeue(READY_q, (void**)&deadThread);
		Current = deadThread;
		Current->state = RUNNING;
		
		preempt_enable();
		
		uthread_ctx_switch(Join->context, Current->context);

		/* Return the exited value of Join when it finishes */
		if (retval != NULL)
			*retval = tempRet->retval;
	}
	
	/* Thread wasn't found */
	else
		return -1;	
	
	return 0;
}

