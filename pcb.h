#pragma once
#include <stddef.h>
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

