#pragma once

typedef struct LinkedList {
    int e; // element -- index of frame store
    struct LinkedList* next;
} LinkedList;

LinkedList* init_ll();
LinkedList* createnode_ll(int e);
int enqueue_ll(LinkedList* ll, int e);
int move_to_front_ll(LinkedList* ll, int e);
int peek_q(LinkedList* ll);
void free_ll(LinkedList* ll);
int isempty_q(LinkedList* ll);
void clear_q(LinkedList* ll);
int enqueuesorted_q(LinkedList* ll, int e);
int enqueuehead_q(LinkedList* ll, int e);
int enqueue_q(LinkedList* ll, int e);