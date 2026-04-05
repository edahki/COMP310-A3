#include <stdio.h>
#ifndef
#define FRAME_STORE_SIZE 100
#endif

typedef struct {
    char** lines; // always array of size 3
} page;

page* framestore[FRAME_STORE_SIZE]; // length set by compile flag


int fetch_page(char* process_name, int pageno);
// fetch desired page from disk
// insert into framestore and return position new page was inserted into in framestore

// map: process_name -> [array of pcb* to currently existing instances of that process]
void load_page(char* process_name, int pageno);
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

int evict();
// uses LRU policy to evict page from frame_store by calling evict_page
// returns the index in framestore of evicted page

// helper :D
void evict_page(char* process_name, int pageno);
// find all currently existing pcbs for 'process_name' (using map outlined above)
// for each pcb found:
//  pcb->[agetable[pageno] = -1
