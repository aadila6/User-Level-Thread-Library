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
#include "queue.c"
#include "uthread.h"
typedef struct queue* queue_t;
//Thread States
#define READY   0
#define ACTIVE  1
#define BLOCKED 2
#define ZOMBIE  3

uthread_t newTid = 1;
/* TODO Phase 2 */
struct threads{
	uthread_t tid;
	uthread_t next;
	int state;
	uthread_ctx_t *context;
	void* stack;
	int retval;
};
struct threads *current = NULL;
queue_t readyQ = NULL;
queue_t zombieQ = NULL;
queue_t blockQ = NULL;

void uthread_yield(void){
	// /* TODO Phase 2 */
	preempt_disable();
	struct threads *temp = current;
	queue_enqueue(readyQ,current);
	current->state= READY;
	queue_dequeue(readyQ,(void **)&current);
	current->state = ACTIVE;
	preempt_enable();
	uthread_ctx_switch(temp->context,current->context);

}
uthread_t uthread_self(void){
	/* TODO Phase 2 */
	return current->tid;
}
void iniNewQueue(void){
	readyQ = queue_create();
	zombieQ = queue_create();
	blockQ = queue_create();
	current = malloc(sizeof(struct threads));
	current->tid = 0;
	current->next = 0;
	current->state = ACTIVE;
	current->context = malloc(sizeof(uthread_ctx_t));
	current->retval = 0;
	current->stack = NULL;
	preempt_start();
}
int uthread_create(uthread_func_t func, void *arg){
	/* TODO Phase 2 */
	struct threads *mythread = malloc(sizeof(struct threads));
	if(newTid == 1){
		iniNewQueue();
	}
	if(mythread == NULL ){
		return -1;
	}
	preempt_disable();
	mythread->tid = newTid;
	mythread->next = 0;
	mythread->state = READY; //Initialize it to READY state
	mythread->context = malloc(sizeof(uthread_ctx_t));
	mythread->stack = uthread_ctx_alloc_stack();
	mythread->retval = 0;
	
	getcontext(mythread->context);
	uthread_ctx_init(mythread->context, mythread->stack, func, arg);
	newTid++;
	preempt_enable();
	queue_enqueue(readyQ, mythread);
	return mythread->tid;
	
}

//Check if the thread exists
int func(void *data, void *arg){
	if(*(uthread_t*)arg == ((struct threads*) data)->tid){
		return 1;
		}
 	return 0;
}
//Check if the thread->next tid exist in the blocked queue. 
int funcNext(void *data, void *arg){
	if(*(uthread_t*)arg == ((struct threads*) data)->next){
		return 1;
		}
 	return 0;
}
void uthread_exit(int retval){
	/* TODO Phase 2 */
	struct threads *temp = current;
	struct threads *blockedThread = NULL;
	current->retval = retval;
	preempt_disable();
	if(queue_iterate(blockQ, *funcNext, (void*)&current->tid, (void**)&blockedThread)){
		blockedThread->next = 0;
		queue_delete(blockQ,blockedThread);
		queue_enqueue(readyQ,blockedThread);
		blockedThread->state = READY;
	}
	queue_enqueue(zombieQ, (void*)current);
	current->state = ZOMBIE;
	if(readyQ->count!=0){
		queue_dequeue(readyQ,(void **)&current);
		current->state = ACTIVE;}
	preempt_enable();
	uthread_ctx_switch(temp->context,current->context);
}
/*
Thread 1 joins thread 2,
case 1: T2 is alREADY in zombieQ
case 2: T2 is not in zombieQ
*/
int uthread_join(uthread_t tid, int *retval){
	/* TODO Phase 2 */
	preempt_disable();
	if(!tid){
		preempt_enable();
		return -1;
	}
  	struct threads *T1  = current;
	struct threads *T2 = NULL;
	queue_iterate(zombieQ, *func, (void*)&tid, (void **)&T2);
	if(!T2){
		//if T2 is not in the zombie/not null
		T1 = current;
		queue_enqueue(blockQ, current);
		current->state = BLOCKED;
		current->next = tid; //waiting for this TID to die and then unblocked. 
		queue_dequeue(readyQ,(void**)&current);			
		current->state = ACTIVE;
		preempt_enable();
		uthread_ctx_switch(T1->context,current->context);
		}
	queue_iterate(zombieQ, *func, (void*)&tid, (void**)&T2);
	*retval = T2->retval;
	preempt_disable();
	queue_delete(zombieQ,T2);
	uthread_ctx_destroy_stack(T2->stack);
	free(T2);
	preempt_enable();
	return 0;
	/* TODO Phase 3 */
}

