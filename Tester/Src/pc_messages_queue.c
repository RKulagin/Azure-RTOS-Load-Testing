#include "pc_messages_queue.h"


struct Queue* PC_Messages_Queue_Init(){
	struct Queue* queue = malloc(sizeof(struct Queue));
	if (queue == NULL){
		return NULL;
	}
	return queue;
}

void Push(struct Queue* q, uint_8* message){
	strcpy(q->queue[q->end], message);
	q->end = (q->end + 1) % PC_MESSAGES_QUEUE_SIZE;
}

uint_8* Pop(struct Queue* q){
	uint_8* poped = q->queue[q->start];
	q->queue[q->start] = NULL;
	q->start = (q->end + 1) % PC_MESSAGES_QUEUE_SIZE;
}
