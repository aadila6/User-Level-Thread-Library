#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
typedef struct queue* queue_t;
struct queue {
	/* TODO Phase 1 */
	int count;
	struct Node *front, *rear; 
};
struct Node{
	void *data;
	struct Node *prev;
	struct Node *next;
};
queue_t queue_create(void)
{
	/* TODO Phase 1 */
	queue_t q = (queue_t)malloc(sizeof(queue_t));
	if (q == NULL) {
    	return NULL;
  	}
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
    return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue ==NULL || queue->count != 0 ){
		return -1; //Cannot be destroyed if the queue is not empty
	}else{
		free(queue);
		return 0;
	}
}
int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data == NULL){
		return -1;
	}else{
		struct Node *itr = NULL;
    	itr = malloc(sizeof(struct Node));
 		itr->data = data;
    	itr->next = NULL;
		itr->prev = NULL;
		if(queue->count == 0){
			queue->front = queue->rear = itr;
			queue->count++;
		}else{
			queue->front->prev = itr;
			itr->next = queue->front;
			queue->front = itr;
			queue->count++;
		}
		return 0;
	}
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data == NULL ){
		return -1;
	}else if(queue->count == 1){
		*data = queue->front->data;
		free(queue->front);
		queue->front = NULL;
		queue->rear = NULL;
		queue->count--;
		return 0;
	}else{
		// queue->rear->prev = queue->rear->prev->prev;
		queue->rear->prev->next = NULL; 
		*data = queue->rear->data;
		queue->rear = queue->rear->prev;
		queue->count--;
		return 0;
	}
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data == NULL ){
		return -1;
	}else{
		struct Node *itr = NULL;
		itr = queue->rear;
		for(int i = 0; i<queue->count; i++){
			if(itr->data == data){
				itr->prev->next = NULL;
				itr->prev->next = itr->next;
				itr->next->prev = itr->prev;
				itr->next=NULL;
				free(itr);
				queue->count--;
				return 0;
			}else{
				itr = itr->prev;
			}
		}
	return -1; //If data not found
	}
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
	if(func == NULL || queue == NULL){
		return -1;
	}else if (queue->count == 0){
		return 0;
	} else {
		struct Node *itr = NULL;
		itr = queue->front;
		while(itr->next != NULL){
			if(func(itr->data, arg) == 1){
				*data = itr->data;
				return 0;
			}else{
				itr = itr->next;
			}
		}
		return 0;
	}
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL){
		return -1;
	}else{
		return queue->count;
	}
}

