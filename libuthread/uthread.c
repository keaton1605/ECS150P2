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

#define UTHREAD_STACK_SIZE 32768

/* TODO Phase 2 */
int numThreads = 0;
queue_t q;

struct thread {
	uthread_t TID;
	char* stack;
	int state;
	ucontext_t* context;
};


void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct thread* newThread = (struct thread*)malloc(sizeof(struct thread));

	if (numThreads == 0)
	{
		newThread->TID = numThreads;
		newThread->stack = (char*)uthread_ctx_alloc_stack();
		newThread->state = 0;
		uthread_ctx_init(newThread->context, (void*)newThread->stack[0], NULL, NULL);
		q = queue_create();
		queue_enqueue(q, newThread);
	}

	newThread->TID = ++numThreads;
	newThread->stack = (char*)uthread_ctx_alloc_stack();
	newThread->state = 0;
	uthread_ctx_init(newThread->context, (void*)newThread->stack[0], func, arg);
	queue_enqueue(q, newThread);

	return 0;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}

