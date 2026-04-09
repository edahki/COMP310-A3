#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os_structures.h"
#include "page.h"
#include "interpreter.h"

Page* framestore[FRAME_STORE_SIZE_SLOTS]; // frame store for shell memory
LinkedList* LRU_list; // LRU linked list for eviction strategy

// loads pageno of process_name into frame store and updates all page tables
// returns the frame store index where the page was loaded
int load_page(char* process_name, int pageno) {
    int frame_loc = fetch_page(process_name, pageno); // call fetch_page to insert page into frame store

    // iterate through ready queue, updating all page tables
    PCB* head = ready_queue->head;

    while (head) {
        if (strcmp(head->name, process_name) == 0) { // process with same name
            // update page table
            update_pcb_pagetable(head, pageno, frame_loc);
        }
        head = head->next;
    }

    return frame_loc;
}

// helper to update page table for an individual process
void update_pcb_pagetable(PCB* pcb, int pageno, int frame_loc) {
    pcb->page_table[pageno] = frame_loc;
}

// helper that fetches page pageno from process_name and inserts it into the frame store
// note: if free page does not exist, method evicts LRU page first
// returns frame store index of the inserted page
int fetch_page(char* process_name, int pageno) {
    Page* loaded = init_page(process_name, pageno);
    for (int i = 0; i < FRAME_STORE_SIZE_SLOTS; i++) {
        // search for free slot in framestore
        if (!framestore[i]) { // free slot found, insert into this slot
            framestore[i] = loaded;
            return i;
        }  
    }
    // empty slot not found, so must evict page
    int slot = evict();
    framestore[slot] = loaded;
    return slot;
}

// helper that evicts the LRU page from the frame store using the LRU list
// returns the frame store index that was evicted
int evict() {
    // find frame number to evict (LRU)
    LinkedList* evicted = dequeue_ll(LRU_list); // get tail of LRU list
    int frame_loc = evicted->e;
    
    evict_page(evicted->process_name, evicted->pageno, frame_loc); // use fields of evicted as arguments to evict_page()
    free_ll(evicted);
    return frame_loc;
}

// helper that invalidates page table entries for all instances of process_name that have
// pageno mapped to frame_loc
void evict_page(char* process_name, int pageno, int frame_loc) {

    // find page's location in frame store by iterating through ready queue, cleaning each matching process instance's PCB along the way

    PCB* head = ready_queue->head;

    while (head) {
        if (strcmp(head->name, process_name) == 0) {
            // update page table to invalid
            update_pcb_pagetable(head, pageno, -1);
        }
        head = head->next;
    }

    Page* evicted_page = framestore[frame_loc];
    framestore[frame_loc] = NULL; // reset frame store contents

    printf("Victim page contents:\n\n"); // print evicted page contents
    for (int i = 0; i < 3; i++) {
        printf("%s", evicted_page->lines[i]);
    }
    printf("\nEnd of victim page contents.\n");

    free_page(evicted_page);
}