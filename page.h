#pragma once
#include <stdio.h>

// we decided to define a struct for a page and make our frame store
// an array of these structs

typedef struct {
    char* lines[3]; // array of 3 lines
} Page;

Page* init_page(char* process_name, int pageno);
void free_page(Page* page);