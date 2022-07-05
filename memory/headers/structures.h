#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#define FRAMES 64

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
    Page page;
    struct PageElement *prev;
    struct PageElement *next;
};

struct Process{
    int pid;
    int ppid;
    int status;
    int priority;

    WS workingSet;
};

struct WS{
    PageElement *head;
    PageElement *tail;
    int totalPages;
};

struct TLB{
    Page* rows[FRAMES];
};

#endif