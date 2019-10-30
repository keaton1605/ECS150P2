# Phase 1
## Overview
### Structs
We implemented a doubly linked list for the queue.  We have a `struct 
queue` that includes a pointer to the head node, a pointer to the tail node, 
and the number of items in the queue. We also have a `struct 
queue_Node` that stores the node data, a pointer to the next node and a 
pointer to the previous node. We chose this data structure because it has 
methods to access, and thereby add to or delete, the first and last elements 
in O(1) time. 

### Functions
The `queue_enqueue` function inserts a new node at the beginning of the 
queue (at the head). The `queue_dequeue` function removes the tail node
at the end of the queue. The `queue_delete` function iterates through the 
queue from oldest to youngest until it finds the correct data, or goes 
through all the nodes. If the node is at the head or tail, the deletion is 
simple and much like the dequeue function. If the deletion is in the middle, 
the nodes adjacent to the deleted one simply point to each other. 

## Testing

For queue testing purposes we wrote many small functions with 
combinations of the queue functions (enqueue, dequeue, delete, iterate, 
destroy). Inside these functions we used print and assert statements to 
determine whether the values were what they were supposed to be. This 
helped us catch an error where originally the dequeue function would erase 
a queue of only length one instead of making the head and tail both NULL. 

# Phase 2
## Overview
### Struct
We create a TCB struct that includes the following elements:

	uthread_t TID; - the TID of the thread
	uthread_t joinTID; - the TID of thread to join
	void* stack; - the thread's stack
	states state; - the thread's state (READY, RUNNING, BLOCKED, ZOMBIE) 
			which is stored in an enum
	ucontext_t *context; - the thread's context
	int retval; - return value after thread completion


### Global Variables

    struct thread* Current;
Contains the current running thread. This is global so we can change or 
access it in any function that needs to modify the thread that is currently 
executing.

    queue_t READY_q;
Stores all the threads that are READY. Threads are enqueued to this in 
uthread_create() once they are first created. In uthread_yield(), the current 
thread is put back into the queue and the next thread is dequeued, and then 
switched to.

    queue_t BLOCKED_q;
Contains all threads that try to join an active thread. Threads are deleted 
from this queue after the active thread finishes execution and is collected 
by the blocked thread. 

    queue_t ZOMB_q; 
This queue contains all threads that have finished execution but have not 
been collected by a join statement yet.

### Functions
The `uthread_yield()` function puts the Current running thread back into 
the READY_q, and then takes out the next thread from the queue and makes
it the new Current before swapping to its context.

The `uthread_create()` function initializes the main thread if there are no 
threads running yet. No matter the number of threads, it then initializes 
another thread with the passed in function, and puts this in the READY_q.

The `uthread_exit()` function puts the Current, finishing thread into 
ZOMB_q, and changes Current to the next thread to execute. It also 
attempts to unblock any threads waiting for the zombie thread to finish, 
using a helper function we made called `unBlock()`. This function simply 
checks if the TID of the zombie thread is being waited on, and if so, puts that
thread back in the READY_q.

## Testing
As per the instructions, we tested both the uthread_hello.c file as well as the 
uthread_yield.c file. On top of this, we tried more combinations of create, 
yield, and join to make sure our context switching was correct. 

# Phase 3
## Overview
We started this phase by rewriting join(). When join is called with the TID 
and return value of a thread, the TID is searched for in the ZOMB_q  and if 
found, is dequeued and freed. The TID (if not found in ZOMB_q) is searched
for in both the BLOCKED_q and the READY_q to check if the thread is 
active. If active, the joining thread is blocked and enqueued within the 
BLOCKED_q.

When the active thread finishes execution and exits, a joined TID is searched
for within the BLOCKED_q, and if found, is inserted back into the 
READY_q. 

## Testing
We used the same uthread_yield.c tester as before, and tested as well with 
the BLOCKED_q and the return values from the completed threads. 

# Phase 4
## Overview
We designed a timer, set of signals, and sigaction struct to keep track of 
when signals should be sent, and when they should be ignored. 

### Global Variables

    sigset_t signalSet;
A set of signals initially empty that determines what signals should be 
blocked and which ones should be able to pause the current thread.

    struct itimerval Timer;
An itimerval Timer that is initialized to go off and produce an alarm signal 
with a frequency of 100 Hz.

### Functions

`sigAlarmHandler()` is our signal handler, that simply waits for a signal 
and if it receives one, forces the Current thread to yield.

`preempt_disable()` and `preempt_enable()` use a signal set that only 
includes the alarm signal to determine whether or not alarm signals are 
blocked.

`preempt_start()` initializes the timer and signal set, and uses sigaction to 
make the signals appear to the signal handler.
