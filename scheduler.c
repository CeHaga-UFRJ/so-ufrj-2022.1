#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define MAX_PROCESSES 10
#define TIME_SLICE 4

#define DISK_TIMER 5
#define TAPE_TIMER 8
#define PRINTER_TIME 10

/* Headers */
typedef struct Process Process;
typedef struct Device Device;
typedef struct ProcessQueueElement ProcessQueueElement;
typedef struct ProcessQueueDescriptor ProcessQueueDescriptor;
typedef struct IOQueueElement IOQueueElement;

Process* createProcesses(int *numProcesses, ProcessQueueDescriptor *highPriority, ProcessQueueDescriptor *lowPriority, ProcessQueueDescriptor *diskQueue, ProcessQueueDescriptor *tapeQueue, ProcessQueueDescriptor *printerQueue);
Device* createDevice(int time, char *name);
ProcessQueueDescriptor* createQueue();
void executeDevice(Device *device);
void checkDeviceEnd(Device *device, ProcessQueueElement *returnQueue);
void checkDeviceStart(Device *device, ProcessQueueElement *inputQueue);
void addQueue(ProcessQueueDescriptor *queue, Process *process);
Process* removeQueue(ProcessQueueDescriptor *queue);
void newProcess(int instant, ProcessQueueDescriptor *queue);
void killProcess(Process *process);
void showMenu();
void readProcessesFromFile();
void readProcessesFromKeyboard();
void createRandomProcesses();
char* trim(char* str);


/* Data structures */

struct Process{
    int pid;
    int ppid;
    int status;
    int priority;

    int startTime;
    int remainingTime;
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
    Device *deviceQueue;
    int initialTime;
};

int main(){
    showMenu();
    Device *cpu = createDevice(TIME_SLICE, "CPU");
    Device *disk = createDevice(DISK_TIMER, "Disco");
    Device *tape = createDevice(TAPE_TIMER, "Fita");
    Device *printer = createDevice(PRINTER_TIME, "Impressora");

    ProcessQueueDescriptor *highPriority = createQueue();
    ProcessQueueDescriptor *lowPriority = createQueue();
    ProcessQueueDescriptor *diskQueue = createQueue();
    ProcessQueueDescriptor *tapeQueue = createQueue();
    ProcessQueueDescriptor *printerQueue = createQueue();

    int numProcesses;
    // Vetor de processos
    Process *processes = createProcesses(&numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue); // TODO

    for(int instant = 0; numProcesses; instant++){
        printf("=== Começando instante %d ===\n", instant);

        newProcess(instant, highPriority);

        executeDevice(disk);
        executeDevice(tape);
        executeDevice(printer);
        executeDevice(cpu);

        Process *actualProcess = cpu->actualProcess;
        if(actualProcess){
            int remainingTime = actualProcess->remainingTime--;
            // IO
            if(remainingTime == 0){
                killProcess(actualProcess);
                numProcesses--;
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

void showMenu() {
    int choice;

    printf("Ola usuario! Como voce gostaria de realizar a criacao dos processos? \n 1 - Ler do arquivo input.txt \n 2 - Ler do teclado \n 3 - Criar processos com numeros aleatorios \n");
    scanf("%i", &choice);

    switch (choice) {
        case 1:
            readProcessesFromFile();
            break;
        case 2:
            readProcessesFromKeyboard();
            break;
        case 3:
            createRandomProcesses();
            break;
        default:
            printf("Opcao invalida. Escolha uma das seguintes opcoes: 1, 2 ou 3.");
    }
}

void readProcessesFromFile() {
    const char* filename = "input.txt";
    FILE* ptr;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
 
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        printf("Falha ao abrir o arquivo de input \n");
    }

    while ((read = getline(&line, &len, ptr)) != -1) {
        printf("%s", line);

        char *pt = strtok(line, ",");

        char *pid = trim(pt);
        pt = strtok(NULL, ",");

        char *serviceTime = trim(pt);
        pt = strtok(NULL, ",");

        char *arrivalTime = trim(pt);
        pt = strtok(NULL, ",");

        char *IO = trim(pt);
    }
    
    fclose(ptr);
    if (line) free(line);
}

char* trim(char* str) {
    static char str1[99];
    int count = 0, j, k;
  
    while (str[count] == ' ') {
        count++;
    }

    for (j = count, k = 0;
         str[j] != '\0'; j++, k++) {
        str1[k] = str[j];
    }
    str1[k] = '\0';
  
    return str1;
}

void readProcessesFromKeyboard() {
    // TODO
}

void createRandomProcesses() {
    // TODO
}

Process* createProcesses(
                    int *numProcesses,
                    ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue){

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

void checkDeviceEnd(Device *device, ProcessQueueElement *returnQueue){
    if(!device->actualProcess) return;

    if(device->remainingTime == 0){
        addQueue(returnQueue, device->actualProcess);
        printf("- Processo %d saiu do dispositivo %s\n", device->actualProcess->pid, device->name);
        device->actualProcess = NULL;
    }
}

void checkDeviceStart(Device *device, ProcessQueueElement *inputQueue){
    if(device->actualProcess) return;

    device->actualProcess = removeQueue(inputQueue);
    device->remainingTime = device->duration;

    if(device->actualProcess) 
        printf("+ Processo %d entrou no dispositivo %s\n", device->actualProcess->pid, device->name);
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

void newProcess(int instant, ProcessQueueDescriptor *queue){
    // print(Processo criado);
}

void killProcess(Process *process){
    printf("X Processo %d foi finalizado\n", process->pid);
    free(process);
}
