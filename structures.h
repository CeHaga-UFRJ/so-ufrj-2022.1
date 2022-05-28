#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "globals.h"
#include "scheduler_arguments.h"

typedef struct Process Process;
typedef struct Device Device;
typedef struct ProcessQueueElement ProcessQueueElement;
typedef struct ProcessQueueDescriptor ProcessQueueDescriptor;
typedef struct IOQueueElement IOQueueElement;
typedef struct QueueCollection QueueCollection;
typedef struct DeviceCollection DeviceCollection;
typedef struct StructureCollection StructureCollection;

struct Process{
    int pid;
    int ppid;
    int status;
    int priority;

    int arrivalTime;
    int processedTime;
    int serviceTime;

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

struct QueueCollection {
    ProcessQueueDescriptor *highPriority;
    ProcessQueueDescriptor *lowPriority;
    ProcessQueueDescriptor *diskQueue;
    ProcessQueueDescriptor *tapeQueue;
    ProcessQueueDescriptor *printerQueue;
};

struct DeviceCollection {
    Device *cpu;
    Device *disk;
    Device *tape;
    Device *printer;
};

struct StructureCollection{
    QueueCollection *queues;
    DeviceCollection *devices;

    int actualProcessIndex;
    int numProcesses;
    Process *processes;
};

#include "processes_input.h"


extern void addQueue(ProcessQueueDescriptor *queue, Process *process);
extern Process* removeQueue(ProcessQueueDescriptor *queue);
extern StructureCollection* createStructures(int readProcessesFrom);

#endif
