// Copyright 2022 Ruslan Kulagin

#ifndef INCLUDE_MESSAGES_QUEUE
#define INCLUDE_MESSAGES_QUEUE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGES_QUEUE_SIZE 256

struct Queue{
	uint8_t* queue[MESSAGES_QUEUE_SIZE];

	uint16_t start;
	uint16_t end;
};

extern struct Queue* pc_messages_queue;

struct Queue* Messages_Queue_Init();

void Push( struct Queue* q, uint8_t* message);

uint8_t* Pop(struct Queue* q);


#endif // INCLUDE_MESSAGES_QUEUE
