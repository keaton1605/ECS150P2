#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction Action;
struct itimerval Timer;

static void sigAlarmHandler(int sigNum)
{
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
}

void preempt_start(void)
{
	/* Sets up the signal handler */
	Action.sa_handler = sigAlarmHandler;
	sigemptyset(&Action.sa_mask);
	sigaction(SIGVTALRM, &Action, NULL);
	
	/* Sets first Timer period and successive timer periods to 10000 microseconds (100 HZ) */
	Timer.it_value.tv_sec = 0;
	Timer.it_interval.tv_sec = 0;
	Timer.it_value.tv_usec = 1000000 / HZ;
	Timer.it_value.tv_usec = 1000000 / HZ;
	setitimer(ITIMER_REAL, &Timer, NULL);
}

