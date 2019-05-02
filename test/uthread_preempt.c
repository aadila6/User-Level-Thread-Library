/*
 * Simple preempt test
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int t1(void* arg)
{
    printf("Hallo!\n");
	while(1);
}

int main(void)
{
	uthread_create(t1, NULL);
    uthread_yield();
    printf("Success!\n");
	return 0;
}
