#pragma once
#include <stdio.h>
#include "pcb.h"
#include "page.h"
#include "linked_list.h"

// Here we define our demand paging structures and methods
// Note: This could have been put into shellmemory.c, but was separated
// for clarity

#ifndef FRAME_STORE_SIZE
#define FRAME_STORE_SIZE 18 // default value of frame store size if not set by Makefile
#endif

#define FRAME_STORE_SIZE_SLOTS (FRAME_STORE_SIZE / 3) // constant used for size of frame store array

extern Page* framestore[FRAME_STORE_SIZE_SLOTS]; // frame store - array of pages
extern LinkedList* LRU_list; // linked list used for LRU eviction

int fetch_page(char* process_name, int pageno);
int load_page(char* process_name, int pageno);
void update_pcb_pagetable(PCB* pcb, int pageno, int frame_loc);
int evict();
int evict_page(char* process_name, int pageno, int frame_loc);
