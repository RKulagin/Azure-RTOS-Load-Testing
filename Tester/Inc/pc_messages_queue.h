// Copyright 2022 Ruslan Kulagin

#ifndef INCLUDE_PC_MESSAGES_QUEUE
#define INCLUDE_PC_MESSAGES_QUEUE

#define PC_MESSAGES_QUEUE_SIZE 256

struct Queue{
	uint_8* queue[PC_MESSAGES_QUEUE_SIZE];

	uint_16 start = 0;
	uint_16 end = 0;
};

Queue* pc_messages_queue;

Queue* PC_Messages_Queue_Init();

void Push(Queue* q, uint_8* message);

uint_8* Pop(Queue* q);


#endif // INCLUDE_PC_MESSAGES_QUEUE
