#pragma once
#include <stddef.h>
#include "queue.h"
int interpreter(char *command_args[], int args_size);
int help();

PCB *run_pcb_to_completion(PCB *pcb);
int run_pcb_for_n_steps(PCB *pcb, size_t n);

extern Queue *ready_queue;