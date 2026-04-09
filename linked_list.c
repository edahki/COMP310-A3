#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "linked_list.h"

// Linked list interface for LRU implementation

// initializes linked list
LinkedList* init_ll() {
    LinkedList* dummy = createnode_ll(0, "", -1); // use dummy node as head
    return dummy;
}

// creates node at end of linked list - helper method for other queue operations
LinkedList* createnode_ll(int e, char* process_name, int pageno) {
    LinkedList* node = malloc(sizeof(LinkedList));
    node->e = e;
    node->process_name = strdup(process_name);
    node->pageno = pageno;
    node->next = 0; // null ptr
    return node;
}

// enqueues int to the back of the LL
int enqueue_ll(LinkedList* ll, int e, char* process_name, int pageno) {
    LinkedList* curr = ll;
    while (curr->next) {
        curr = curr->next;
    }
    LinkedList* new = createnode_ll(e, process_name, pageno);
    curr->next = new;

    return 0;
}

// helper for move_to_front_ll - finds node in linked list ll with element e
// works because all frame indices in the linked list are unique
// note: returns node BEFORE target
LinkedList* find_ll(LinkedList* ll, int e) {
    if (isempty_ll(ll)) {
        return NULL;
    }
    LinkedList* prev = ll;
    LinkedList* head = ll->next;
    while (head) {
        if (head->e == e) {
            return prev;
        }
        prev = head;
        head = head->next;
    }
    return NULL; // node doesn't exist
}

// finds node with frame index e and moves it to the front of linked list ll
int move_to_front_ll(LinkedList* ll, int e) {

    if (isempty_ll(ll) || ll->next->e == e) { 
        return 0; // list is empty OR node is already at the front of thel ist
    }

    LinkedList* prev = find_ll(ll,e); // gets node BEFORE the node to move
    if (prev == NULL) {
      return 1; // error: searched for index that doesn't exist
    }
    LinkedList* node_to_move = prev->next;
    
    LinkedList* temp = ll->next;
    ll->next = node_to_move;
    prev->next = node_to_move->next;
    node_to_move->next = temp;

    return 0;  
}

// gets PREVIOUS element of list at specified index idx
// if idx < 0, gets element at last index
LinkedList* get_ll(LinkedList* ll, int idx) { 
    if (isempty_ll(ll)) {
        return NULL;
    }
    LinkedList* head = ll;
    if (idx >= 0) {
        for (int i = 0; i < idx; i++) {
            if (!head->next) { // index unreachable
                return NULL;
            }
            head = head->next;
        }
    } else {
        while (head->next && head->next->next) {
            head = head->next;
        }
    }
    
    return head;
}

// creates and enqueues node at the front of linked list ll
int enqueuehead_ll(LinkedList* ll, int e, char* process_name, int pageno) {
    LinkedList* temp = ll->next;
    LinkedList* newhead = createnode_ll(e, process_name, pageno);
    ll->next = newhead;
    newhead->next = temp;
    return 0;
}

// removes and returns node at end of linked list ll
LinkedList* dequeue_ll(LinkedList* ll) {
    if (isempty_ll(ll)) {
        return NULL; // error: (note all elements in LL must be non-negative ints)
    }
    LinkedList* before_tail = get_ll(ll, -1);
    LinkedList* tail = before_tail->next;

    //remove tail node from ll
    before_tail->next = NULL;
    
    return tail;
}

// deletes node of linked list ll with frame index e
void deletenode_ll(LinkedList* ll, int e) {
    LinkedList* prev = find_ll(ll, e); // find node BEFORE the node to remove
    LinkedList* to_remove = prev->next;
    prev->next = to_remove->next;
    to_remove->next = NULL;
    free_ll(to_remove);
}

// checks if linked list is empty
int isempty_ll(LinkedList* ll) {
    return (!ll->next);
}

// frees linked list from heap
void free_ll(LinkedList* ll) {
    LinkedList* curr = ll;
    while (curr) {
        LinkedList* temp = curr;
        curr = curr->next;
        free(temp->process_name);
        free(temp);
    }
}
