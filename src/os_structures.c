#include "os_structures.h"
#include "page.h"
#include "interpreter.h"
#include "pcb.h"

#include <stdio.h>
#include <string.h>

void load_page(char* process_name, int pageno) {
    int frame_loc = fetch_page(process_name, pageno);

    // iterate through ready queue, for each process
    PCB* head = ready_queue->head;

    while(head) {
        if(strcmp(head->name, process_name) == 0) {
            // update page table
            head->page_table[pageno] = frame_loc;
        }
        head = head->next;
    }
}

int fetch_page(char* process_name, int pageno) {
    Page* loaded = page_init(process_name, pageno);
    for (int i = 0; i < FRAME_STORE_SIZE; i++) {
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

int evict() {
    
}

void evict_page(char* process_name, int pageno) {
  return;
}