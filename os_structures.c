#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os_structures.h"
#include "page.h"
#include "interpreter.h"

Page* framestore[FRAME_STORE_SIZE_SLOTS];

int load_page(char* process_name, int pageno) {
    int frame_loc = fetch_page(process_name, pageno);

    // iterate through ready queue, for each process
    PCB* head = ready_queue->head;

    while (head) {
        if (strcmp(head->name, process_name) == 0) {
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

int fetch_page(char* process_name, int pageno) {
    Page* loaded = init_page(process_name, pageno);
    for (int i = 0; i < FRAME_STORE_SIZE_SLOTS; i++) {
        // search for free slot in framestore
        if (!framestore[i]) { // free slot found, insert into this slot
            //printf("NEVER NEEDED TO EVICT PAGE! FOUND FREE SLOT @ FRAME NUMBER %d\n", i);
            framestore[i] = loaded;
            return i;
        }  
    }
    // empty slot not found, so must evict page
    //printf("EVICTING PAGE\n");
    int slot = evict(process_name);
    framestore[slot] = loaded;
    return slot;
}

int evict(char* process_name) {

    // find pageno to evict (LRU)
    
    evict_page(process_name, 0);
    return 0; // choose 0 to evict for now -- change to LRU policy later
}

void evict_page(char* process_name, int pageno) {
    
    int frame_loc = -1;

    // find page's location in framestore by iterating through ready queue, cleaning each matching process instance's pcb along the way

    PCB* head = ready_queue->head;

    while (head) {
        if (strcmp(head->name, process_name) == 0) {
            if (frame_loc < 0) {
                frame_loc = head->page_table[pageno];
            }
            // update page table
            update_pcb_pagetable(head, pageno, -1); // update page table to invalid
        }
        head = head->next;
    }

    Page* evicted_page = framestore[frame_loc];
    framestore[frame_loc] = NULL;

    printf("Victim page contents:\n");
    for (int i = 0; i < 3; i++) {
        printf("%s", evicted_page->lines[i]);
    }
    printf("End of victim page contents.\n");

    free_page(evicted_page);
}