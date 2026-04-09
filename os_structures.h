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
// fetch desired page from disk
// insert into framestore and return position new page was inserted into in framestore

// map: process_name -> [array of pcb* to currently existing instances of that process]
int load_page(char* process_name, int pageno);
// int frame_loc = fetch_from_disk(pageno, process_name)
// look for free slot in frame_store
//  -> if found, insert into index
//  -> else, evict()
// find all currently existing pcbs for 'process_name' (using map outlined above)
// for each pcb found:
//  pcb->pagetable[pageno] = frame_loc


// ver 2
// void fetch_page(pageno, process_name);
// int frame_loc = fetch_from_disk(pageno, process_name)
// Idea: When a program fetches a page, go through the ready queue and find
// all other PCBs with the same program name
// Then update its page table: pcb->pagetable[pageno] = frame_loc


void update_pcb_pagetable(PCB* pcb, int pageno, int frame_loc);

int evict();
// uses LRU policy to evict page from frame_store by calling evict_page
// returns the index in framestore of evicted page


int evict_page(char* process_name, int pageno, int frame_loc);
// helper:
// find all currently existing pcbs for 'process_name' (using map outlined above)
// for each pcb found:
//  pcb->pagetable[pageno] = -1
