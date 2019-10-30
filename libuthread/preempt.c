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
#define sec 1000000

sigset_t signalSet;
struct itimerval Timer;

static void sigAlarmHandler(int sigNum)
{
	/* Forces Current thread to yield */
	uthread_yield();
}

void preempt_disable(void)
{
	sigprocmask(SIG_BLOCK, &signalSet, NULL);
} // Blocks alarm signal

void preempt_enable(void)
{
	sigprocmask(SIG_UNBLOCK, &signalSet, NULL);
} // Renables alarm signal

void preempt_start(void)
{
	struct sigaction Action;
	memset(&Action, 0, sizeof Action);

	/* Empties initially and adds alarm signal to signalSet */
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
	Timer.it_value.tv_usec = sec / HZ;
	Timer.it_interval.tv_usec = sec / HZ;
	setitimer(ITIMER_VIRTUAL, &Timer, NULL);
}

