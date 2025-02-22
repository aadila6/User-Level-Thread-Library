#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libuthread/queue.h"

void test_create(void)
{
    queue_t q;
    q = queue_create();
    assert(q != NULL);
}

void test_queue_simple(void)
{
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

int main()
{
    test_create();
    test_queue_simple();
    printf("PASSED ALL!");
}