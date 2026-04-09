#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pcb.h"
#include "shell.h" // MAX_USER_INPUT
#include "os_structures.h"
#include "interpreter.h" // ready_queue
#include "linked_list.h"

// initializes a PCB with process_name

PCB* pcb_init(char* process_name) {
    PCB *pcb = malloc(sizeof(PCB));

    static size_t pid_counter = 0; // retains value after function calls
    pcb->pid = pid_counter++;

    pcb->name = strdup(process_name);
    pcb->next = NULL;
    pcb->pc = 0;

    // try to find pre-existing instance of process to copy from
    int instance_exists = 0;
    PCB* head = ready_queue ? ready_queue->head : NULL; 

    while (head) {
        if (strcmp(head->name, process_name) == 0) {
            // found another instance of process
            instance_exists = 1;
            break;
        }
        head = head->next;
    }

    // copy info from existing instance over
    if (instance_exists) {
        pcb->line_count = head->line_count;
        pcb->page_count = head->page_count;
        pcb->page_table = malloc(sizeof(int) * pcb->page_count);

        for (int i = 0; i < pcb->page_count; i++) { // populate page table
            pcb->page_table[i] = head->page_table[i];
        }
        
    } else { // need to populate rest of fields ourselves
        FILE* fptr = fopen(process_name, "rt");

        char linebuf[MAX_USER_INPUT];
        int line_count = 0;
        while (!feof(fptr)) {
            fgets(linebuf, MAX_USER_INPUT, fptr);

            size_t index = allocate_line(linebuf);
            if (index == (size_t)(-1)) {
                // free the pcb here
                fclose(fptr);
                return NULL;
            }
            line_count++;
        }
        fclose(fptr);
        pcb->line_count = line_count;
        pcb->page_count = (pcb->line_count + 2) / 3; // ceiling integer division trick
        pcb->page_table = malloc(sizeof(int) * pcb->page_count);

        for (int i = 0; i < pcb->page_count; i++) {
            pcb->page_table[i] = -1;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        if (pcb->page_table[i] < 0) { //invalid, load page
            int frame_loc = load_page(pcb->name, i);
            update_pcb_pagetable(pcb, i, frame_loc);
            enqueuehead_ll(LRU_list, frame_loc, pcb->name, i);
        }
        else {
            move_to_front_ll(LRU_list, pcb->page_table[i]);
        }
    }
    pcb->duration = pcb->line_count;
    
    return pcb;

}

// frees a PCB from memory
void pcb_free(PCB* pcb) {
    free(pcb->page_table);
    if (strcmp("", pcb->name)) {
        free(pcb->name);
    }
    free(pcb);
}

int pcb_has_next_instruction(PCB* pcb) {
    return pcb->pc < pcb->line_count;
}

// gets page of next instruction from PCB's PC
// note: only gets location of page, we get offset before in top level
int pcb_page_of_next_instruction(PCB* pcb) {
    // within same page
    
    int pageno = pcb->pc / 3;

    if (pcb->page_table[pageno] < 0) { // page fault! return back to interpreter with -1 to signal
        return -1;
    }
    else {
        pcb->pc++;
        return pcb->page_table[pageno];
    }
}

// gets the current page frame location from a given PCB's program counter
int pcb_current_page_frame_loc(PCB* pcb) {
    return pcb->page_table[pcb->pc / 3];
}

// kept in to avoid compilation errors - we never actually call this in any of the test cases
PCB *create_process_from_FILE(FILE *script) {
    printf("You shouldn't see me");
    return NULL;
}