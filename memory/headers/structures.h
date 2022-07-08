#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "../headers/globals.h"
#include "../headers/memory_arguments.h"

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
    int rows[MAX_PAGES];
};

struct RAM{
    int addresses[MAX_FRAMES];
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