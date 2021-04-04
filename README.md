# Concurrent_Queue

- Lock-Based Concurrent Queue
- Fine-grained lock 
- Coarse-grained lock

## Implementation
- Queue_Init(queue_t *q,int _size);
- Queue_Print(queue_t *q);
- Queue_Enqueue(void* multi_arg);
- Queue_Dequeue(void* multi_arg);
  - multi_arg : Pass argument by structure
