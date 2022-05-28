#include "structures.h"

Device* createDevice(int time, char *name){
    Device *device = (Device *)malloc(sizeof(Device));
    device->remainingTime = device->duration = time;
    device->actualProcess = NULL;
    strcpy(device->name, name);

    return device;
}

ProcessQueueDescriptor* createQueue(){
    ProcessQueueDescriptor* queue = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    queue->head = queue->tail = NULL;

    return queue;
}

void addQueue(ProcessQueueDescriptor *queue, Process *process){
    if(!process) return;

    ProcessQueueElement *processQueue = (ProcessQueueElement *)malloc(sizeof(ProcessQueueElement));
    processQueue->process = process; 
    processQueue->next = NULL;

    if(!queue->head) queue->head = processQueue;

    if(queue->tail) queue->tail->next = processQueue;
    queue->tail = processQueue;
}

Process* removeQueue(ProcessQueueDescriptor *queue){
    if(!queue->head) return NULL;

    Process* process = queue->head->process;
    ProcessQueueElement* newHead = queue->head->next;

    free(queue->head);
    queue->head = newHead;

    if(!queue->head) queue->tail = NULL;

    return process;
}
