#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#include "fine_queue.h"

void Queue_Init(queue_t *q,int _size) {

    node_t *tmp = malloc(sizeof(node_t));
    tmp->next = NULL;
    q->size = _size; q->count = 0;
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->head_lock,NULL);
    pthread_mutex_init(&q->tail_lock,NULL);
}

void Queue_Print(queue_t *q) {
    node_t *cur = q->head->next;
    while(cur != NULL) {
        printf("%d ", cur->value);
        cur = cur->next;
    }
}

void *Queue_Enqueue(void* multi_arg){
    EA *arg = (EA *)multi_arg;

    queue_t *q  = arg->_q;
    int value   = arg->_value;

    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value  = value;
    tmp->next   = NULL;

    pthread_mutex_lock(&q->tail_lock);
    q->count        += 1;
    q->tail->next   = tmp;
    q->tail         = tmp;
    pthread_mutex_unlock(&q->tail_lock);
}

void *Queue_Dequeue(void* multi_arg){
    DA *arg = (DA *)multi_arg;

    queue_t *q  = arg->_q;
    int *value  = arg->_value;

    pthread_mutex_lock(&q->head_lock);
    q->count            -= 1;
    node_t *tmp         = q->head;
    node_t *new_head    = tmp->next;
    if (new_head == NULL) {
        pthread_mutex_unlock(&q->head_lock);
        return 0;
    }
    *value = new_head->value;
    q->head = new_head;
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0;
}

int main() {
    pthread_t t1, t2;
    queue_t q;
    Queue_Init(&q,10);

    printf("Q_Size : %d\n", q.size);
    printf("Q_Count : %d\n", q.count);

    EA *enqueue_arg;
	enqueue_arg = (EA *)malloc(sizeof(EA));
	enqueue_arg->_q = &q; enqueue_arg->_value = 20;

    DA *dequeue_arg;
    int pop_value;
	dequeue_arg = (DA *)malloc(sizeof(DA));
	dequeue_arg->_q = &q; dequeue_arg->_value = &pop_value;

    pthread_create(&t1, NULL, Queue_Enqueue, (void *) enqueue_arg);
    pthread_create(&t2, NULL, Queue_Enqueue, (void *) enqueue_arg);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Q_Size : %d\n", q.size);
    printf("Q_Count : %d\n", q.count);

    Queue_Print(&q);
    printf("\n");
    
    pthread_create(&t1, NULL, Queue_Dequeue, (void *) dequeue_arg);
    pthread_join(t1, NULL);

    Queue_Print(&q);

}