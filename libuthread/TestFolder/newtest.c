#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include "queue.h"
#include "uthread.h"
#include "preempt.h"

/* Global Definitions */
volatile int preempt_var = 0;

/***** Function Definitions *****/
/* Test utility functions */
/* Call at the beginning of user defined test */
void TEST_START(const char *testName)
{   /* Print the name of test function */
    printf("TEST: " "\x1b[33m" "%s" "\x1b[0m" "\n", testName);
}
/* Call at the end of user defined test */
void TEST_END(void)
{
    printf("\x1b[32m" "PASS" "\x1b[0m" "\n");
}

/* Thread functions */
int hello(void* arg)
{
	printf("Hello world!\n");
	return 0;
}

int thread_1(void* arg)
{
    int a = (int)(long) arg;
	printf("Thread %d, arg: %d\n", uthread_self(), a);
    uthread_join(uthread_create(hello,NULL), NULL);
	return a;
}

int thread_wakeup(void *arg)
{
    printf("Wakeup: If preempt works I printed before wait\n");
    preempt_var = 1;
    return preempt_var;
}

int thread_wait(void *arg)
{
    int tid = uthread_create(thread_wakeup, NULL);
    int retval;

    while(preempt_var == 0) {}

    printf("Wait: If preempt works I printed after wakeup\n");
    uthread_join(tid, &retval);

    assert(retval ==  1);

    return retval;
}

/* Test Functions */
void test_1(void) 
{   
    TEST_START(__func__);

    int retval = -1;
    int tid = uthread_create(hello, NULL);
    uthread_join(tid, &retval);
    assert(retval == 0);

    TEST_END();
}

void test_2(void) 
{   
    TEST_START(__func__);

    int arg = 2;
    int retval = 0;

    int tid = uthread_create(thread_1, (void*)(long) arg);
    uthread_join(tid, &retval);
    assert(retval == arg);

    TEST_END();
}

void test_preempt (void)
{
    TEST_START(__func__);

    int retval = 0;

    uthread_join(uthread_create(thread_wait, NULL), &retval);

    assert(retval == 1);

    TEST_END();
}


/***** Main *****/
int main(void)
{
    /* Test functions */
    //test_1();
    //test_2();
    test_preempt();
    return 0;
}
