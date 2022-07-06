#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#define FRAMES 64
#define MAX_PROCESSES 20
#define NUM_PAGES 50
#define WORKING_SET_LIMIT 4

#define PROCESS_CREATION_ERROR 2
#define TABLE_CREATION_ERROR 3

typedef struct Page Page;
typedef struct PageElement PageElement;
typedef struct TableElement TableElement;
typedef struct Process Process;
typedef struct WS WS;
typedef struct TLB TLB;

struct Page{
    int pageNumber;
    int pid;
};

struct PageElement{
    Page *page;
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
};

struct TLB{
    Page* rows[FRAMES];
    int remainingSlots;
};

extern Page* readPageFromWorkingSet(Process *process, int pageNumber);
extern void removeProcessFromTLB(TLB *table, int pid);
extern Process* createProcess(int pid);
extern TLB* createTable();
extern Page* addPageToWorkingSet(Process *process, int pageNumber);
extern void removeLeastUsedPage(Process *process);
extern void addPageToTLB(TLB *table, Page *addedPage);

#endif