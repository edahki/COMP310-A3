#pragma once
#include <stdio.h>

typedef size_t pid;

// This is the redefined PCB struct that we used to support demand paging.

typedef struct PCB {
    pid pid;
    char *name;
    int line_count; // number of lines in the program
    int page_count; // number of pages in the program
    int duration;
    int pc;
    struct PCB *next;
    int* page_table; // each PCB has a page table
} PCB;

PCB* pcb_init(char* process_name);
void pcb_free(PCB* pcb);
int pcb_has_next_instruction(PCB* pcb);
int pcb_page_of_next_instruction(PCB* pcb);
int pcb_current_page_frame_loc(PCB* pcb);

PCB *create_process_from_FILE(FILE *script); // not used