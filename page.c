#include <stdlib.h>
#include <string.h>

#include "page.h"
#include "shell.h" // MAX_USER_INPUT

// initializes page for process_name at page # pageno
Page* init_page(char* process_name, int pageno) {
    
    FILE* fptr = fopen(process_name, "rt");
    if (!fptr) return NULL;

    Page* pg = malloc(sizeof(Page));

    char linebuf[MAX_USER_INPUT];

    for (int i = 0; i < pageno * 3; i++) { // use fgets (pageno * 3) times to get to right offset in file
        if (fgets(linebuf, MAX_USER_INPUT, fptr)) {
            continue;
        }
    }

    for (int i = 0; i < 3; i++) { // read lines of page
        if (fgets(linebuf, MAX_USER_INPUT, fptr)) {
            pg->lines[i] = strdup(linebuf);
        } else { // no line to read - happens for pages at EOF
            pg->lines[i] = NULL;
        }
        
    }
    fclose(fptr);
    return pg;
}

// frees page from memory
void free_page(Page* page) {
    for (int i = 0; i < 3; i++) {
        free(page->lines[i]);
    }
    free(page);
}

