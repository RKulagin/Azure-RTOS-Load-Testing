#include "pc_messages_queue.h"

struct Queue* pc_messages_queue;

struct Queue* PC_Messages_Queue_Init(){
	struct Queue* queue = malloc(sizeof(struct Queue));
	if (queue == NULL){
		return NULL;
	}
	queue->start = 0;
	queue->end = 0;
	return queue;
}

void Push(struct Queue* q, uint8_t* message){
	q->queue[q->end] = malloc((strlen(message)+1) * sizeof(uint8_t));
	strcpy(q->queue[q->end], message);
	memset(message, 0, sizeof(message));
	q->end = (q->end + 1) % PC_MESSAGES_QUEUE_SIZE;
}

uint8_t* Pop(struct Queue* q){
	uint8_t* poped = q->queue[q->start];
	q->queue[q->start] = NULL;
	q->start = (q->start + 1) % PC_MESSAGES_QUEUE_SIZE;
	return poped;
}
