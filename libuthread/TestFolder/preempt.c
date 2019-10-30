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
#define USECONDS_PER_SECOND 1000000

/* Global Data */
static struct itimerval preempt_timer;
static struct sigaction preempt_sa;
static sigset_t preempt_sigset;

/* Signal handler */
static void preempt_handler(int arg)
{
	uthread_yield();
	// printf("handler\n");
	return;
}

void preempt_disable(void)
{
	sigprocmask(SIG_BLOCK, &preempt_sigset, NULL);
}

void preempt_enable(void)
{
	sigprocmask(SIG_UNBLOCK, &preempt_sigset, NULL);
}

void preempt_start(void)
{
	/* Setup sigset with SIGVTALARM */
	sigemptyset(&preempt_sigset);
	sigaddset(&preempt_sigset, SIGVTALRM);

	/* Setup SIGVTARLM handler */
	sigemptyset(&preempt_sa.sa_mask);
	preempt_sa.sa_flags = 0;
	preempt_sa.sa_handler = &preempt_handler;
	sigaction(SIGVTALRM, &preempt_sa, NULL);

	/* Start Timer */
	preempt_timer.it_value.tv_sec = 0;
	preempt_timer.it_value.tv_usec = USECONDS_PER_SECOND / HZ;
	preempt_timer.it_interval.tv_sec = 0;
	preempt_timer.it_interval.tv_usec = USECONDS_PER_SECOND / HZ;
	setitimer(ITIMER_VIRTUAL, &preempt_timer, NULL);

	// printf("preempt start\n");
}
