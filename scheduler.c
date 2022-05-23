#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTES
#define MAX_PROCESSES 10
#define TIME_SLICE 4

#define DISK_TIMER 5
#define TAPE_TIMER 8
#define PRINTER_TIME 10

// STRUCTS
typedef struct Process{
    int pid;
    int priority;
    int startTime;
    int remainingTime;
} Process;

typedef struct Device{
    Process *actualProcess;
    int remainingTime;
    int duration;
    char name[16];
} Device;

typedef struct ProcessQueue{
    Process *process;
    struct ProcessQueue *next;
} ProcessQueue;

typedef struct ProcessQueueDescriptor{
    ProcessQueue *head;
    ProcessQueue *tail;
} ProcessQueueDescriptor;

// HEADERS

int main(){
    int numProcesses;
    Process *processes = createProcesses(&numProcesses);

    Device *cpu = createDevice(TIME_SLICE, "CPU");
    Device *disk = createDevice(DISK_TIMER, "Disco");
    Device *tape = createDevice(TAPE_TIMER, "Fita");
    Device *printer = createDevice(PRINTER_TIME, "Impressora");

    ProcessQueueDescriptor *highPriority = createQueue();
    ProcessQueueDescriptor *lowPriority = createQueue();
    ProcessQueueDescriptor *diskQueue = createQueue();
    ProcessQueueDescriptor *tapeQueue = createQueue();
    ProcessQueueDescriptor *printerQueue = createQueue();

    for(int quantum = 0; numProcesses; quantum++){
        printf("=== Começando instante %d ===\n", quantum);

        newProcess(quantum, highPriority);

        executeDevice(disk);
        executeDevice(tape);
        executeDevice(printer);
        executeDevice(cpu);

        Process *actualProcess = cpu->actualProcess;
        if(actualProcess){
            int remainingTime = actualProcess->remainingTime--;
            if(remainingTime == 0){
                killProcess(actualProcess);
            }
        }

        checkDeviceEnd(disk, lowPriority);
        checkDeviceEnd(tape, highPriority);
        checkDeviceEnd(printer, highPriority);
        checkDeviceEnd(cpu, lowPriority);

        checkDeviceStart(disk, diskQueue);
        checkDeviceStart(tape, tapeQueue);
        checkDeviceStart(printer, printerQueue);
        checkDeviceStart(cpu, highPriority);
        checkDeviceStart(cpu, lowPriority);
        
        printf("\n\n");
    }
    return 0;
}

Process* createProcesses(int *numProcesses){
    *numProcesses = 0;

    return NULL;
}

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

void executeDevice(Device *device){
    if(device->actualProcess){
        device->remainingTime--; 
    }
}

void checkDeviceEnd(Device *device, ProcessQueue *returnQueue){
    if(!device->actualProcess) return;

    if(device->remainingTime == 0){
        addQueue(returnQueue, device->actualProcess);
        printf("- Processo %d saiu do dispositivo %s\n", device->actualProcess->pid, device->name);
        device->actualProcess = NULL;
    }
}

void checkDeviceStart(Device *device, ProcessQueue *inputQueue){
    if(device->actualProcess) return;

    device->actualProcess = removeQueue(inputQueue);
    device->remainingTime = device->duration;

    if(device->actualProcess) 
        printf("+ Processo %d entrou no dispositivo %s\n", device->actualProcess->pid, device->name);
}    

void addQueue(ProcessQueueDescriptor *queue, Process *process){
    if(!process) return;

    ProcessQueue *processQueue = (ProcessQueue *)malloc(sizeof(ProcessQueue));
    processQueue->process = process; 
    processQueue->next = NULL;

    if(!queue->head) queue->head = processQueue;

    if(queue->tail) queue->tail->next = processQueue;
    queue->tail = processQueue;
}

Process* removeQueue(ProcessQueueDescriptor *queue){
    if(!queue->head) return NULL;

    Process* process = queue->head->process;
    ProcessQueue* newHead = queue->head->next;

    free(queue->head);
    queue->head = newHead;

    if(!queue->head) queue->tail = NULL;

    return process;
}

void newProcess(int quantum, ProcessQueueDescriptor *queue){
    // print(Processo criado);
}

void killProcess(Process *process){
    printf("X Processo %d foi finalizado\n", process->pid);
    free(process);
}
