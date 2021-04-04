#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef struct __node_t {
    int             value;
    struct __node_t *next;
}node_t;

typedef struct __queue_t {
    int             size;
    int             count;
    node_t          *head;
    node_t          *tail;
    pthread_mutex_t head_lock, tail_lock;
} queue_t;

typedef struct Enqueue_arg {
	queue_t        *_q;
	int             _value;
}EA;

typedef struct Dequeue_arg {
	queue_t        *_q;
	int            *_value;
}DA;

void  Queue_Init(queue_t *q,int _size);
void  Queue_Print(queue_t *q);
void *Queue_Enqueue(void* multi_arg);
void *Queue_Dequeue(void* multi_arg);