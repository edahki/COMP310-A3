#pragma once
#include <stddef.h>
#include <stdio.h> 

typedef size_t pid;

struct PCB {
    pid pid;
    char *name;
    FILE* fptr;
    size_t duration;
    int pc;
    size_t line_count;
    size_t line_base;
    struct PCB *next;

    int pagetable[];
};


int pcb_has_next_instruction(struct PCB *pcb);
size_t pcb_next_instruction(struct PCB *pcb);
struct PCB *create_process(const char *filename);
struct PCB *create_process_from_FILE(FILE *f);
void free_pcb(struct PCB *pcb);

