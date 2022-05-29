#include "../headers/structures.h"

Device* createDevice(int time, char *name);
ProcessQueueDescriptor* createQueue();
void addQueue(ProcessQueueDescriptor *queue, Process *process);
Process* removeQueue(ProcessQueueDescriptor *queue);
StructureCollection* createStructures(int readProcessesFrom);
void destroyStructures(StructureCollection **collection);

/*
 * Cria um dispositivo
 */
Device* createDevice(int time, char *name){
    Device *device = (Device *)malloc(sizeof(Device));
    device->remainingTime = device->duration = time;
    device->actualProcess = NULL;
    strcpy(device->name, name);

    return device;
}

/*
 * Cria uma fila de processos
 */
ProcessQueueDescriptor* createQueue(){
    ProcessQueueDescriptor* queue = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    queue->head = queue->tail = NULL;

    return queue;
}

/*
 * Adiciona um processo na fila
 */
void addQueue(ProcessQueueDescriptor *queue, Process *process){
    if(!process) return;

    ProcessQueueElement *processQueue = (ProcessQueueElement *)malloc(sizeof(ProcessQueueElement));
    processQueue->process = process; 
    processQueue->next = NULL;

    if(!queue->head) queue->head = processQueue;

    if(queue->tail) queue->tail->next = processQueue;
    queue->tail = processQueue;
}

/*
 * Remove um processo da fila
 */
Process* removeQueue(ProcessQueueDescriptor *queue){
    if(!queue->head) return NULL;

    Process* process = queue->head->process;
    ProcessQueueElement* newHead = queue->head->next;

    free(queue->head);
    queue->head = newHead;

    if(!queue->head) queue->tail = NULL;

    return process;
}

/*
 * Cria as estruturas basicas
 */
StructureCollection* createStructures(int readProcessesFrom){
    StructureCollection *collection = (StructureCollection *)malloc(sizeof(StructureCollection));
    
    collection->devices = (DeviceCollection *)malloc(sizeof(DeviceCollection));
    collection->devices->cpu = createDevice(TIME_SLICE, "CPU");
    collection->devices->disk = createDevice(DISK_TIMER, "Disco");
    collection->devices->tape = createDevice(TAPE_TIMER, "Fita");
    collection->devices->printer = createDevice(PRINTER_TIME, "Impressora");

    collection->queues = (QueueCollection *)malloc(sizeof(QueueCollection));
    collection->queues->highPriority = createQueue();
    collection->queues->lowPriority = createQueue();
    collection->queues->diskQueue = createQueue();
    collection->queues->tapeQueue = createQueue();
    collection->queues->printerQueue = createQueue();

    collection->processes = createProcesses(readProcessesFrom, &collection->numProcesses, collection->queues);
    collection->actualProcessIndex = 0;
    printf("%d processo(s) criado(s) com sucesso\n\n", collection->numProcesses);

    return collection;
}

/*
 * Libera as estruturas
 */
void destroyStructures(StructureCollection **collection){
    free((*collection)->devices->cpu);
    free((*collection)->devices->disk);
    free((*collection)->devices->tape);
    free((*collection)->devices->printer);
    free((*collection)->devices);

    free((*collection)->queues->highPriority);
    free((*collection)->queues->lowPriority);
    free((*collection)->queues->diskQueue);
    free((*collection)->queues->tapeQueue);
    free((*collection)->queues->printerQueue);
    free((*collection)->queues);

    free((*collection)->processes);

    free((*collection));
}