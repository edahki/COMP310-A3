#pragma once

// Linked list interface for LRU implementation

typedef struct LinkedList {
    int e; // element -- index of frame store
    char* process_name;
    int pageno;
    struct LinkedList* next;
} LinkedList;

LinkedList* init_ll();
LinkedList* createnode_ll(int e, char* process_name, int pageno);
int enqueue_ll(LinkedList* ll, int e, char* process_name, int pageno);
int move_to_front_ll(LinkedList* ll, int e);
LinkedList* get_ll(LinkedList* ll, int idx);
int enqueuehead_ll(LinkedList* ll, int e, char* process_name, int pageno);
LinkedList* pop_ll(LinkedList* ll);
int dequeue_ll(LinkedList* q);
int peek_ll(LinkedList* ll);
int isempty_ll(LinkedList* ll);
void free_ll(LinkedList* ll);
void deletenode_ll(LinkedList* ll, int e);