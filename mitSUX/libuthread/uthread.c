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
//struct thread* PrevT;

int numThreads = 0;
static ucontext_t ctx;

queue_t q; // READY queue
queue_t BLOCKED_q;
queue_t ZOMB_q;

typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	ZOMBIE
} states;


struct thread
{
	uthread_t TID;
	uthread_t joinTID;
	void** stack;
	states state;
	ucontext_t *context;
	int retval;
};

int tidFindFunc(void *data, void *arg)
{
	struct thread* temp = (struct thread*)data;
	int tid = (int)(long)arg;

	if(temp->TID == tid) 
	{
		return 1;
	}
	return 0;
}

int tidFind(queue_t q, void* data, int tid)
{
	int retval = queue_iterate(q, tidFindFunc, (void*)(long)tid, data);
	//printf("%d\n", retval);
	return retval;
}

void uthread_yield(void)
{
	if (q == NULL)
		return;

	struct thread* prev;
	struct thread* temp;

	/* Change current state to ready and enqueue */
	Current->state = READY;
	temp = Current;
	queue_enqueue(q, (void**)Current);

	/* Dequeue next thread */
	queue_dequeue(q, (void**)&prev);	

	/* Change previous thread to Current */
	Current = prev;
	Current->state = RUNNING;

	uthread_ctx_switch(temp->context, Current->context);
}


uthread_t uthread_self(void)
{
	return Current->TID;
}


int uthread_create(uthread_func_t func, void *arg)
{
	
	struct thread* newThread = (struct thread*)malloc(sizeof(struct thread));

	if (numThreads == 0)
	{
		struct thread* newThread1 = (struct thread*)malloc(sizeof(struct thread));
		newThread1->TID = numThreads;
		newThread1->stack = uthread_ctx_alloc_stack();
		newThread1->state = 0;
		newThread1->context = &ctx;
		//printf("%d\n", *(int*)&newThread->stack);
		//uthread_ctx_init(&newThread1->context, &newThread1->stack, NULL, NULL);
		Main = newThread1;
		Current = newThread1;
		q = queue_create();
		//queue_enqueue(q, newThread1);
	}
	
	newThread->TID = ++numThreads;
	//printf("TID: %d\n", newThread->TID);
	newThread->stack = uthread_ctx_alloc_stack();
	newThread->state = 0;
	newThread->context = malloc(sizeof(uthread_ctx_t));
	//func(arg);
	uthread_ctx_init(newThread->context, newThread->stack, func, arg);
	//Current = newThread;
	queue_enqueue(q, newThread);
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
	
	/* Put current thread in Zombie mode */
	Current->retval = retval;
	Current->state = ZOMBIE;
	queue_enqueue(ZOMB_q, Current);

	/* Put next thread in Current */
	queue_dequeue(q, (void**)&prev);
	Current = prev;

	uthread_ctx_switch(temp->context, Current->context);
}

int uthread_join(uthread_t tid, int *retval)
{
	/* PHASE 2 testing stuff
	struct thread *ptr, *mainPtr;
	printf("Queue_Length and NumThreads %d %d\n", queue_length(q), numThreads);

	queue_dequeue(q, (void**)&mainPtr);
	queue_dequeue(q, (void**)&ptr);

	printf("mainTID ptrTID %d %d\n", mainPtr->TID, ptr->TID);

	uthread_ctx_switch(mainPtr->context, ptr->context);
	*/

	// TODO Phase 3 

	struct thread* Join;

	struct thread* Test = (struct thread*)malloc(sizeof(struct thread));
	Test->TID = 43;
	queue_enqueue(ZOMB_q, Test);


	if (tid == 0 || Current->TID == tid)	
		return -1;

	
	
	/* TO DO (if T2 already dead) 	*/

	if (tidFind(ZOMB_q, &Join, tid) == 1)
		printf("YES\n");
	else
		printf("NO\n");


	//Dead_Collect();
	
	/* TO DO (if T1 alive)

	if (tidFind(ZOMB_q, &Join, tid) == 1 || tidFind(BLOCKED_q, &Join, tid) == 1)
		queue_enqueue(BLOCKED_q, Current);
	*/


	
	return 0;
}

