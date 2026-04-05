CC=gcc
#CFLAGS=-g -O0 -Wall #-DNDEBUG
CFLAGS=-DNDEBUG -Wall -D FRAME_STORE_SIZE=$(framesize) -D VAR_STORE_SIZE=$(varmemsize)

mysh: shell.c interpreter.c shellmemory.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c pcb.c queue.c schedule_policy.c os_structures.c utils/page.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o pcb.o queue.o schedule_policy.o os_structures.o page.o

clean: 
	rm mysh; rm *.o
