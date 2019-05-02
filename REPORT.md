# **ECS 150**
---

## **Project 2:** User-level Thread Library
---

### Implementation

**queue.c:**
First, we implemented a queue using a linked list and initialized a count 
variable that counts the number of nodes in our queue, and two nodes, 
front and rear, that will point to the beginning and end of the list. 
Then, we created two additional nodes that points to the next and 
previous node, and a pointer variable called data that refers to each 
item in the list. Next we dynamically allocated a new queue using malloc() 
to create an empty queue and initialized count and the front and rear nodes.

Next, we deallocated the memory of our queue using free(). Then, in order 
to enqueue a data item, we created a temp node that will point to our data 
node and dynamically allocated it. Using an if statement, if count is zero, 
then our front and rear nodes will point to our temp data node; otherwise, 
our front node points to our next temp node.

To dequeue an item, we simply implemented our rear node to point to the 
previous node in our list. To delete an item, we created a temp node 
called itr, pointing to the last item of the doubly linked list. 
In an if-statement, we search for the data that is passed in, through 
this entire list. It begins to search from the rear pointer because we 
are removing the oldest item, which equals to data. If data is found, 
we detach the prev and next data nodes and connect the two adjacent nodes 
to the found node, decrementing count. This whole process is implemented 
through a for loop and it iterates through itr = itr->prev.

To iterate through the queue, it works similarly through deleting a node 
in the queue, as it searches through the queue and each node. We passed 
in the thread ID when we worked on uthread.c, and it will go through each 
thread in the queue, and matches the ID number. If there is a match, this 
function returns the specific thread through the iterator 
node: *data = itr->data. 

Lastly, we set the queue to count and return this to get the length 
of our queue.

**uthread.c:**
We initialized each thread to its own execution context; it contains a 
thread identifier (tid), a stack, its state, which is implemented through 
queues, and we created three queues: the ready queue (readyQ), zombie 
queue (zombieQ), and the blocked queue (blockQ). In our uthread_create 
function, we initialized a single thread and set its tid number, and its 
relevant members, and enqueued them to our ready queue. Additionally, if 
this creation occurs for the first time, it will create a main thread of 
tid = 0 and the three queues representing the thread's states 
(readyQ, zombieQ, blockQ) are officially created, and we enqueued the 
ready queue to readyQ. 

In our uthread_yield function, we created a temp thread that is assigned 
to the current running thread, and enqueued the current thread to readyQ. 
Then, we dequeued another thread and stored it to the current running 
thread. Next, we implemented a context switch. 

In our uthread_join function, it takes in a specific tid to join. We blocked 
the current running thread if the given tid numbered thread is not in 
zombieQ. After all, our blocked thread, which is waiting for a specific 
thread, will become unblocked and collects the tid numbered thread, which 
means this is deleted from zombieQ and freed from memory.

An important thing we noticed was that *queue_func_t func played a 
significant role: it looked for the specific tid number, iterated the queue, 
found the thread, and returned it. Also, for finding the next value, which 
we defined in struct threads, it stored the joining thread's ID number.

**preempt.c**
We implemented preempt_start() using SIGACTION and itimerval, passed it, 
and converted it into a given time for processing. This yields automatically 
100 times per second. Finally, we created a global variable that keeps track 
when to disable and enable. We also included a test_preempt.c file to test 
if our preempting works. In this file, we created a thread in an infinite 
loop that prints "Hallo". Then, we called uthread_yield() to switch to the 
newly created thread that loops infinitely, which switches back to the main 
function after the given timer expired.

### Resources
+ Information/code provided on lecture slides
+ StackOverflow
+ GeeksForGeeks