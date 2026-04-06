#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "shellmemory.h"
#include "os_structures.h"

#ifndef FRAME_STORE_SIZE
#define FRAME_STORE_SIZE 100
#endif

#ifndef VAR_STORE_SIZE
#define VAR_STORE_SIZE 20
#endif

#define true 1
#define false 0

// Helper functions
int match(char *model, char *var) {
    int i, len = strlen(var), matchCount = 0;
    for (i = 0; i < len; i++) {
        if (model[i] == var[i])
            matchCount++;
    }
    if (matchCount == len) {
        return 1;
    } else
        return 0;
}

// for exec memory

struct program_line {
    int allocated; // for sanity-checking
    char *line;
};

struct program_line linememory[MEM_SIZE];
size_t next_free_line = 0;

void reset_linememory_allocator() {
    next_free_line = 0;
    // memset(loaded_programs, 0, sizeof(loaded_programs));
    assert_linememory_is_empty();
}

void assert_linememory_is_empty() {
    for (size_t i = 0; i < MEM_SIZE; ++i) {
        assert(!linememory[i].allocated);
        assert(linememory[i].line == NULL);
    }
}

void init_linemem() {
    for (size_t i = 0; i < MEM_SIZE; ++i) {
        linememory[i].allocated = false;
        linememory[i].line = NULL;
    }
}

size_t allocate_line(const char *line) {
    if (next_free_line >= MEM_SIZE) {
        // out of memory!
        return (size_t)(-1);
    }
    size_t index = next_free_line++;
    assert(!linememory[index].allocated);

    linememory[index].allocated = true;
    linememory[index].line = strdup(line);
    return index;
}

void free_line(size_t index) {
    free(linememory[index].line);
    linememory[index].allocated = false;
    linememory[index].line = NULL;
}

const char *get_line(size_t index) {
    assert(linememory[index].allocated);
    return linememory[index].line;
}

// Infrastructure for memory sharing between processes

// Struct for loaded programs - needed for sharing memory in exec

typedef struct {
    char *name;
    size_t line_base; // change later to backing store file?
    size_t line_count;
    int instances;
} loaded_program;

// table of unique loaded programs
// Note: Since array is so small no need for it to contain pointers to structs
loaded_program loaded_programs[3]; // maximum 3 unique programs at a time (no nested calls to exec)

// finds loaded program based on name
loaded_program *get_lp(char *name) {
    for (int i = 0; i < 3; i++) {
        if (loaded_programs[i].name && strcmp(loaded_programs[i].name, name) == 0) {
            return &loaded_programs[i];
        }
    }
    return NULL; // program doesn't exist
}

// adds loaded program (note: never called if loaded program with 'name' already exists)
int add_lp(char *name, size_t line_base, size_t line_count) {
    for (int i = 0; i < 3; i++) {
        if (!loaded_programs[i].name) { // available slot
            loaded_programs[i].name = strdup(name);
            loaded_programs[i].line_base = line_base;
            loaded_programs[i].line_count = line_count;
            loaded_programs[i].instances = 1;
            return 0;
        }
    }
    return 1;
}

// decrements instances field of a loaded program
// if instances == 0, don't do anything
// commented out for now
// int decrement_instances_lp(char *name) {
//     struct loaded_program *lp = get_lp(name);
//     if (!lp) return 1;
//     if (lp->instances > 0) {
//         lp->instances--;
//     }
//     return 0;
// }

// Shell memory functions -- VARIABLE STORE

struct memory_struct { // block or line
    char *var;
    char *value;
};

struct memory_struct shellmemory[VAR_STORE_SIZE];

void mem_init() {
    int i;
    for (i = 0; i < VAR_STORE_SIZE; i++) {
        shellmemory[i].var = "none";
        shellmemory[i].value = "none";
    }
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
    int i;

    for (i = 0; i < VAR_STORE_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    //Value does not exist, need to find a free spot.
    for (i = 0; i < VAR_STORE_SIZE; i++) {
        if (strcmp(shellmemory[i].var, "none") == 0) {
            shellmemory[i].var = strdup(var_in);
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    return;
}

//get value based on input key
char *mem_get_value(char *var_in) {
    int i;

    for (i = 0; i < VAR_STORE_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            return strdup(shellmemory[i].value);
        }
    }
    return NULL;
}
