#include "pcb.h"
#include "shell.h" // MAX_USER_INPUT
#include "os_structures.h"

PCB* init_pcb(char* process_name) {
    PCB *pcb = malloc(sizeof(PCB));

    static size_t pid_counter = 0; // retains value after function calls;
    pcb->pid = pid_counter++;

    pcb->name = strdup(process_name);
    pcb->next = NULL;
    pcb->pc = 0;
    pcb->page_table = malloc(sizeof(int) * pcb->page_count);

    // try and find pre-existing instance of process to copy from
    int instance_exists = 0;
    PCB* head = ready_queue->head;

    while(head) {
        if(strcmp(head->name, process_name) == 0) {
            // found another instance
            instance_exists = 1;
            break;
        }
        head = head->next;
    }

    // copy info from existing instance over
    if(instance_exists) {
        pcb->line_count = head->line_count;
        pcb->page_count = head->page_count;

        for (int i = 0; i < pcb->page_count; i++) {
            pcb->page_table[i] = head->page_table[i];
        }
        
    } 
    else {
        FILE* fptr = fopen(process_name, "rt");

        char linebuf[MAX_USER_INPUT];
        int line_count = 0;
        while (!feof(fptr)) {
            //memset(linebuf, 0, sizeof(linebuf));
            fgets(linebuf, MAX_USER_INPUT, fptr);

            size_t index = allocate_line(linebuf);
            if (index == (size_t)(-1)) {
                //free the pcb here
                fclose(fptr);
                return NULL;
            }

            line_count++;

            // if (pcb->line_count == 0) {
            //     //pcb->line_base = index;
            // }
            //pcb->line_count++;
        }
        fclose(fptr);
        pcb->line_count = line_count;
        pcb->page_count = (pcb->line_count + 2) / 3;  // ceiling integer division trick

        for (int i = 0; i < pcb->page_count; i++) {
            pcb->page_table[i] = -1;
        }
    }
    
    for (int i = 0; i < 2; i++) {
        if (pcb->page_table[i] < 0) { //invalid, load page
            // PROBLEM! CURRENT PCB BEING DEALT WITH NOT ALREADY IN READY QUEUE! SO, IT DOESN'T GET UPDATED AUTOMATICALLY BY LOAD_PAGE
            load_page(pcb->name, i);
        }
    }
    pcb->duration = pcb->line_count;


}

void free_pbc(PCB* pcb) {
    free(pcb->page_table);
}



// int pcb_has_next_instruction(struct PCB *pcb) {
//     return pcb->pc < pcb->line_count;
// }

// size_t pcb_next_instruction(struct PCB *pcb) {
//     size_t i = pcb->line_base + pcb->pc;
//     pcb->pc++;
//     return i;
// }