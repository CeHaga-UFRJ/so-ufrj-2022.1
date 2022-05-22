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
    int duration;
} Process;

typedef struct Device{
    Process *actualProcess;
    int remainingTime;
} Device;

typedef struct ProcessQueue{
    Process *process;
    struct ProcessQueue *next;
} ProcessQueue;

typedef struct ProcessQueueDescriptor{
    ProcessQueue *head;
    ProcessQueue *tail;
}

// HEADERS
Process* createProcesses();
Device* createDevice(int time);
void executeProcess(Device *device, ProcessQueue *inputQueue, ProcessQueue *returnQueue, int time);
void addQueue(ProcessQueueDescriptor *queue, Process *process);
Process* removeQueue(ProcessQueueDescriptor *queue);
void newProcess(int quantum, ProcessQueueDescriptor *queue);
int isEmpty();

int main(){
    Process *processes = createProcesses();

    Device *cpu = createDevice(TIME_SLICE);
    Device *disk = createDevice(DISK_TIMER);
    Device *tape = createDevice(TAPE_TIMER);
    Device *printer = createDevice(PRINTER_TIME);

    ProcessQueueDescriptor *highPriority = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    highPriority->head = highPriority->tail = NULL;

    ProcessQueueDescriptor *lowPriority = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    lowPriority->head = lowPriority->tail = NULL;

    ProcessQueueDescriptor *diskQueue = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    diskQueue->head = diskQueue->tail = NULL;

    ProcessQueueDescriptor *tapeQueue = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    tapeQueue->head = tapeQueue->tail = NULL;

    ProcessQueueDescriptor *printerQueue = (ProcessQueueDescriptor *)malloc(sizeof(ProcessQueueDescriptor));
    printerQueue->head = printerQueue->tail = NULL;

    int isOver = 0;

    for(int quantum = 0; !isOver; quantum++){
        newProcess(quantum, highPriority);
        executeProcess(disk, diskQueue, lowPriority, DISK_TIMER);
        executeProcess(tape, tapeQueue, highPriority, TAPE_TIMER);
        executeProcess(printer, printerQueue, highPriority, PRINTER_TIME);
        executeProcess(cpu, highPriority, lowPriority, TIME_SLICE);
        if(!cpu->actualProcess) executeProcess(cpu, lowPriority, lowPriority, TIME_SLICE);
        
        isOver = !processes && !cpu->actualProcess && !disk->actualProcess && !tape->actualProcess && !printer->actualProcess;
    }
    return 0;
}

Process* createProcesses(){
    return NULL;
}

Device* createDevice(int time){
    Device *device = (Device *)malloc(sizeof(Device));
    device->remainingTime = time;
    device->process = NULL;
    return device;
}

void executeProcess(Device *device, ProcessQueue *inputQueue, ProcessQueue *returnQueue, int time){
    if(device->process){
        device->remainingTime--; 
    }
    if(device->remainingTime == 0){
        addQueue(returnQueue, device->actualProcess);
        device->process = removeQueue(inputQueue);
        if(device->process) remainingTime = time;
    }
}

void addQueue(ProcessQueueDescriptor *queue, Process *process){
    if(!process) return;

    ProcessQueue processQueue = (ProcessQueue *)malloc(sizeof(ProcessQueue));
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

    return process;
}

void newProcess(int quantum, ProcessQueueDescriptor *queue){
    
}
