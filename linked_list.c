#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "linked_list.h"

// Linked list interface for LRU implementation

// Initializes LL
LinkedList* init_ll() {
    LinkedList* dummy = createnode_ll(0, "", -1);
    return dummy;
}

// Helper method for other queue operations
LinkedList* createnode_ll(int e, char* process_name, int pageno) {
    LinkedList* node = malloc(sizeof(LinkedList));
    node->e = e;
    node->process_name = strdup(process_name);
    node->pageno = pageno;
    node->next = 0; // null ptr
    return node;
}

// Enqueues int to the back of the LL
int enqueue_ll(LinkedList* ll, int e, char* process_name, int pageno) {
    LinkedList* curr = ll;
    while (curr->next) {
        curr = curr->next;
    }
    LinkedList* new = createnode_ll(e, process_name, pageno);
    curr->next = new;

    return 0;
}

// helper for move_to_front_ll - finds node in linked list with element e
// works because all int elements in our linked lists are unique
// NOTE: returns node BEFORE target
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

int move_to_front_ll(LinkedList* ll, int e) {

    if (isempty_ll(ll) || ll->next->e == e) {
        return 0;
    }

    LinkedList* prev = find_ll(ll,e); // node BEFORE the node to move
    if (prev == NULL) {
      return 1; // search for index that doesn't exist
    }
    LinkedList* node_to_move = prev->next;
    
    LinkedList* temp = ll->next;
    ll->next = node_to_move;
    prev->next = node_to_move->next;
    node_to_move->next = temp;

    return 0;  
}

LinkedList* get_ll(LinkedList* ll, int idx) { // gets previous element of element at specified index, or idx < 0 to get at last index
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
    }
    else {
        while (head->next && head->next->next) {
            head = head->next;
        }
    }
    
    return head;
}

// Enqueues int at the head of the specified LL

int enqueuehead_ll(LinkedList* ll, int e, char* process_name, int pageno) {
    LinkedList* temp = ll->next;
    LinkedList* newhead = createnode_ll(e, process_name, pageno);
    ll->next = newhead;
    newhead->next = temp;
    return 0;
}

LinkedList* pop_ll(LinkedList* ll) {
    if(isempty_ll(ll)) {
        return NULL; // ERROR: (note all elements in LL must be non-negative ints)
    }
    LinkedList* before_tail = get_ll(ll, -1);
    LinkedList* tail = before_tail->next;

    //remove tail node from ll
    before_tail->next = NULL;
    
    return tail;
}

void deletenode_ll(LinkedList* ll, int e) {
    LinkedList* prev = find_ll(ll, e);
    LinkedList* to_remove = prev->next;
    prev->next = to_remove->next;
    to_remove->next = NULL;

    free_ll(to_remove);
}

// Dequeues a int from the specified LL
int dequeue_ll(LinkedList* ll) {
    if (!ll->next) {
        printf("LL is empty");
        return NULL; // empty queue
    }

    LinkedList* first = ll->next;
    int value = first->e;

    ll->next = first->next;
    free(first);

    return value;
}

// Returns but does not remove the first int of the LL
int peek_ll(LinkedList* ll) {
    if (!ll->next) return NULL;
    return ll->next->e;
}

// Checks if LL is empty
int isempty_ll(LinkedList* ll) {
    return (!ll->next);
}

// Dequeues from the LL until it is empty
void clear_ll(LinkedList* ll) {
    while (!isempty_ll(ll)) {
        dequeue_ll(ll);
    }
}

// Frees LL from heap
void free_ll(LinkedList* ll) {
    LinkedList* curr = ll;
    while (curr) {
        LinkedList* temp = curr;
        curr = curr->next;
        free(temp->process_name);
        free(temp);
    }
}
