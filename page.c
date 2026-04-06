#include <stdlib.h>
#include <string.h>

#include "page.h"
#include "shell.h" // MAX_USER_INPUT

Page* init_page(char* process_name, int pageno) {
    Page* pg = malloc(sizeof(Page));
    FILE* fptr = fopen(process_name, "rt");

    char linebuf[MAX_USER_INPUT];
    // use fgets (pageno * 3) times to get to right offset in file
    for (int i = 0; i < 3 * pageno; i++) {
        if (fgets(linebuf, MAX_USER_INPUT, fptr)) {
            continue;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (fgets(linebuf, MAX_USER_INPUT, fptr)) {
            pg->lines[i] = strdup(linebuf);
        }
        else {
            pg->lines[i] = NULL;
        }
        
    }

    fclose(fptr);
    return pg;
}

void free_page(Page* page) {
    for (int i = 0; i < 3; i++) {
        free(page->lines[i]);
    }
    free(page);
}

