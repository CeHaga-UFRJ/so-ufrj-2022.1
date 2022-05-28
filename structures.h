#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "main.h"

typedef struct Process Process;
typedef struct Device Device;
typedef struct ProcessQueueElement ProcessQueueElement;
typedef struct ProcessQueueDescriptor ProcessQueueDescriptor;
typedef struct IOQueueElement IOQueueElement;

struct Process{
    int pid;
    int ppid;
    int status;
    int priority;

    int startTime;
    int processedTime;
    int totalTime;

    int actualIO;
    int numIO;
    IOQueueElement *IO;
};

struct Device{
    Process *actualProcess;
    int remainingTime;
    int duration;
    char name[16];
};

struct ProcessQueueElement{
    Process *process;
    struct ProcessQueueElement *next;
};

struct ProcessQueueDescriptor{
    ProcessQueueElement *head;
    ProcessQueueElement *tail;
};

struct IOQueueElement{
    ProcessQueueDescriptor *deviceQueue;
    int initialTime;
};

extern Device* createDevice(int time, char *name);
extern ProcessQueueDescriptor* createQueue();
extern void addQueue(ProcessQueueDescriptor *queue, Process *process);
extern Process* removeQueue(ProcessQueueDescriptor *queue);

#endif
