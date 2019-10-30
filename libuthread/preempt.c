#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

sigset_t signalSet;
struct itimerval Timer;

static void sigAlarmHandler(int sigNum)
{
	//printf("Handling signal\n");
	//exit(0);
	/* Forces Current thread to yield */
	uthread_yield();
}

void preempt_disable(void)
{
	//sigaddset(&Action.sa_mask, SIGVTALRM);
	sigprocmask(SIG_BLOCK, &signalSet, NULL);
}

void preempt_enable(void)
{
	//sigdelset(&Action.sa_mask, SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
}

void preempt_start(void)
{
	struct sigaction Action;
	memset(&Action, 0, sizeof Action);

	sigemptyset(&signalSet);
	sigaddset(&signalSet, SIGVTALRM);

	/* Sets up the signal handler */
	Action.sa_handler = &sigAlarmHandler;
	sigemptyset(&Action.sa_mask);
	Action.sa_flags = 0;
	sigaction(SIGVTALRM, &Action, NULL);
	
	/* Sets first Timer period and successive timer periods to 10000 microseconds (100 HZ) */
	Timer.it_value.tv_sec = 0;
	Timer.it_interval.tv_sec = 0;
	Timer.it_value.tv_usec = 1000000 / HZ;
	Timer.it_value.tv_usec = 1000000 / HZ;
	setitimer(ITIMER_VIRTUAL, &Timer, NULL);
}

