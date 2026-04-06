#pragma once
#include <stdio.h>

typedef size_t pid;

typedef struct PCB {
    pid pid;
    char *name;
    int line_count;
    int page_count;
    int duration;
    int pc;
    struct PCB *next;

    int* page_table;
} PCB;

PCB* pcb_init(char* process_name);
void pcb_free(PCB* pcb);
int pcb_has_next_instruction(PCB* pcb);
int pcb_page_of_next_instruction(PCB* pcb);

PCB *create_process_from_FILE(FILE *script);