#include <stdio.h>

typedef struct {
    char* lines[3]; //length 3
} Page;

Page* init_page(char* process_name, int pageno);