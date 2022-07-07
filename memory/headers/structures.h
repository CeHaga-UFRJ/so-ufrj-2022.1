#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include <stdlib.h>
#include <stdio.h>

#define FRAMES 64
#define MAX_PROCESSES 20
#define NUM_PAGES 50
#define WORKING_SET_LIMIT 4
#define WAIT_TIME 3
#define STOPPING_LIMIT 100

#define PROCESS_CREATION_ERROR 2
#define RAM_CREATION_ERROR 3

typedef struct PageElement PageElement;
typedef struct Process Process;
typedef struct WS WS;
typedef struct RAM RAM;
typedef struct PageValues PageValues;

struct PageElement{
    int pageNumber;
    struct PageElement *prev;
    struct PageElement *next;
};

struct Process{
    int pid;
    int ppid;
    int status;
    int priority;

    WS *workingSet;
};

struct WS{
    PageElement *head;
    PageElement *tail;
    int remainingSlots;
    int rows[NUM_PAGES];
};

struct RAM{
    int addresses[FRAMES];
    int remainingSlots;
};

struct PageValues{
    int page;
    int address;
};

extern int readPageFromWorkingSet(Process *process, int pageNumber);
extern Process* createProcess(int pid);
extern RAM* createRam();
extern void addPageToWorkingSet(Process *process, int pageNumber, int address);
extern PageValues removeLeastUsedPage(Process *process);
extern void removePageFromRAM(RAM *ram, int page);
extern int addPageToRAM(RAM *ram);
extern int isRAMFull(RAM *ram);
extern int isWSEmpty(Process *process);
extern void printTLB(Process *process);

#endif