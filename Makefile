# default values for framesize and varmemsize if not defined during make
framesize ?= 18
varmemsize ?= 10

CC=gcc

# Default: release
CFLAGS=-DNDEBUG -Wall -DFRAME_STORE_SIZE=$(framesize) -DVAR_STORE_SIZE=$(varmemsize)

# Debug flags
DEBUG_FLAGS=-g -O0 -Wall -DFRAME_STORE_SIZE=$(framesize) -DVAR_STORE_SIZE=$(varmemsize)

# Target
mysh: shell.c interpreter.c shellmemory.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c pcb.c queue.c schedule_policy.c os_structures.c page.c linked_list.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o pcb.o queue.o schedule_policy.o os_structures.o page.o linked_list.o

# Debug build
debug: CFLAGS=$(DEBUG_FLAGS)
debug: clean mysh

clean: 
	rm -f mysh *.o