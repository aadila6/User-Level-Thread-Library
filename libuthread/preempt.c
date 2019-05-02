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
bool flag = false;

void handler(int signal)
{
	if(flag){
		uthread_yield();
	}
}
void preempt_disable(void)
{
	/* TODO Phase 4 */
	flag = false;
	
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	flag = true;
	
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	struct sigaction action;
	struct itimerval interval; //alarm
	interval.it_interval.tv_usec = 1000000/HZ;
	interval.it_interval.tv_sec = 0;
	interval.it_value.tv_usec = 1000000/HZ;
	interval.it_value.tv_sec = 0;
	action.sa_handler = &handler;
	sigaction(SIGVTALRM, &action, NULL);
	setitimer(ITIMER_VIRTUAL,&interval, NULL);
	
	
	
}

