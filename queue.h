#pragma once
#include <stdbool.h>
//#include "schedule_policy.h"
#include "pcb.h"

typedef struct {
    PCB *head;
} Queue;

Queue *alloc_queue();
void free_queue(Queue *q);


int program_already_scheduled(Queue *q, char *name);


void enqueue_ignoring_priority(Queue *q, PCB *pcb);


void enqueue_fcfs(Queue *q, PCB *pcb);

void enqueue_sjf(Queue *q, PCB *pcb);

void enqueue_aging(Queue *q, PCB *pcb);


PCB *dequeue_typical(Queue *q);

PCB *dequeue_aging(Queue *q);

bool is_queue_empty(Queue *q);