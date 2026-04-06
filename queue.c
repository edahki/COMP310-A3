#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

Queue *alloc_queue() {
    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    return q;
}

void free_queue(Queue *q) {

    PCB *p = q->head;
    while (p) {
        PCB *next = p->next;
        pcb_free(p);
        p = next;
    }
    free(q);
}

int program_already_scheduled(Queue *q, char *name) {
    PCB *p = q->head;
    while (p) {
        if (strcmp(p->name, name) == 0) return 1;
        p = p->next;
    }
    return 0;
}


void enqueue_ignoring_priority(Queue *q, PCB *pcb) {
    pcb->next = q->head;
    q->head = pcb;
}

void enqueue_fcfs(Queue *q, PCB *pcb) {
    assert(pcb->next == NULL);
    PCB *p = q->head;

    if (!p) {
        q->head = pcb;
        return;
    }

    while (p->next) {
        p = p->next;
    }

    p->next = pcb;
}

void enqueue_sjf(Queue *q, PCB *pcb) {
    size_t dur = pcb->duration;

    PCB *p = q->head;

    if (!p || p->duration > dur) {
        pcb->next = p;
        q->head = pcb;
        return;
    }

    while (p->next) {
        if (p->next->duration > dur) {
            pcb->next = p->next;
            p->next = pcb;
            return;
        }
        p = p->next;
    }

    p->next = pcb;
}

void enqueue_aging(Queue *q, PCB *pcb) {

    if (q->head && q->head->duration == pcb->duration && pcb->pc) {
        enqueue_ignoring_priority(q, pcb);
    } else {
        enqueue_sjf(q, pcb);
    }
}


PCB *dequeue_typical(Queue *q) {
    if (q->head == NULL) {
        return NULL;
    }

    // q -> head -> next
    PCB *head = q->head;
    // q -> next
    q->head = head->next;

    head->next = NULL;
    return head;
}


#ifdef NDEBUG
#define debug_with_age(q)
#else
#define debug_with_age(q) __debug_with_age(q)
#endif

void __debug_with_age(Queue *q) {
    PCB *pcb = q->head;
    printf("q");
    while (pcb) {
        printf(" -> %d %s", pcb->duration, pcb->name);
        pcb = pcb->next;
    }
    printf("\n");
}

PCB *dequeue_aging(Queue *q) {
    debug_with_age(q);
    PCB *r = dequeue_typical(q);

    PCB *p = q->head;
    while (p) {
        if (p->duration > 0) {
            p->duration--;
        }
        p = p->next;
    }

    return r;
}

bool is_queue_empty(Queue *q) {
    if (q == NULL) return true;
    return q->head == NULL;
}