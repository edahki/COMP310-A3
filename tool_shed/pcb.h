#pragma once
#include <stddef.h>
#include <stdio.h> 

typedef size_t pid;

PCB {
    pid pid;
    char *name;
    FILE* fptr;
    size_t duration;
    int pc;
    size_t line_count;
    size_t line_base;
    PCB *next;

    int pagetable[];
};


int pcb_has_next_instruction(PCB *pcb);
size_t pcb_page_of_next_instruction(PCB *pcb);
PCB *create_process(const char *filename);
PCB *create_process_from_FILE(FILE *f);
void free_pcb(PCB *pcb);

