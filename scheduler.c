#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_IO 3

#define NO_ERROR 0
#define INVALID_NUMBER 3
#define INVALID_ARGUMENT 4
#define HELP \
"Simula o funcionamento de um escalonador de processos usando a estratégia Round Robin (ou Circular) com Feedback.\n\
Para executar rode:\n\
\t scheduler [-q#] [-d#] [-t#] [-p#]\n\
\n\
\tq\t: Tamanho do quantum (Time slice)\n\
\td\t: Tempo de leitura do disco\n\
\tt\t: Tempo de leitura da fita magnetica\n\
\tp\t: Tempo de leitura da impressora\n\
"

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
void executeCPU(Device *cpu, int *numProcesses);
void checkDeviceEnd(Device *device, ProcessQueueDescriptor *returnQueue);
void checkDeviceStart(Device *device, ProcessQueueDescriptor *inputQueue);
void addQueue(ProcessQueueDescriptor *queue, Process *process);
Process* removeQueue(ProcessQueueDescriptor *queue);
void newProcess(int instant, ProcessQueueDescriptor *queue);
void killProcess(Device *cpu, int *numProcesses);
void checkProcessIO(Device *cpu);
void showMenu();
char* trim(char* str);
void readProcessesFromFile();
void readProcessesFromKeyboard();
void createRandomProcesses();
int handleParameter(char *ps);
void exitProgram(int error);


/* Data structures */

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

int main(int argc, char *argv[]) {
    int TIME_SLICE = 4;
    int DISK_TIMER = 3;
    int TAPE_TIMER = 5;
    int PRINTER_TIME = 8;

    for(int i = 1; i < argc; i++){
        char *arg = argv[i];
        if(arg[0] != '-' || arg[1] == '\0') exitProgram(INVALID_ARGUMENT);
        char flag = arg[1];
        arg += 2;
        
        switch(flag){
            case 'q':
                TIME_SLICE = handleParameter(arg);
                break;
            case 'd':
                DISK_TIMER = handleParameter(arg);
                break;
            case 't':
                TAPE_TIMER = handleParameter(arg);
                break;
            case 'p':
                PRINTER_TIME = handleParameter(arg);
                break;
            case 'h':
                printf(HELP);
                exit(0);
                break;
            default:
                exitProgram(INVALID_ARGUMENT);
        }
    }

    // Cria estruturas que representam os dispositivos (CPU e IO)
    Device *cpu = createDevice(TIME_SLICE, "CPU");
    Device *disk = createDevice(DISK_TIMER, "Disco");
    Device *tape = createDevice(TAPE_TIMER, "Fita");
    Device *printer = createDevice(PRINTER_TIME, "Impressora");

    // Cria as filas
    ProcessQueueDescriptor *highPriority = createQueue();
    ProcessQueueDescriptor *lowPriority = createQueue();
    ProcessQueueDescriptor *diskQueue = createQueue();
    ProcessQueueDescriptor *tapeQueue = createQueue();
    ProcessQueueDescriptor *printerQueue = createQueue();

    int numProcesses;
    showMenu(&numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue);

    // Vetor de processos
    Process *processes = createProcesses(&numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue); // TODO

    for(int instant = 0; numProcesses; instant++){
        printf("=== Começando instante %d ===\n", instant);

        newProcess(instant, highPriority);

        checkDeviceStart(disk, diskQueue);
        checkDeviceStart(tape, tapeQueue);
        checkDeviceStart(printer, printerQueue);
        checkDeviceStart(cpu, highPriority);
        checkDeviceStart(cpu, lowPriority);

        executeDevice(disk);
        executeDevice(tape);
        executeDevice(printer);

        executeCPU(cpu, &numProcesses);

        checkDeviceEnd(disk, lowPriority);
        checkDeviceEnd(tape, highPriority);
        checkDeviceEnd(printer, highPriority);
        checkDeviceEnd(cpu, lowPriority);

        printf("\n\n");
    }
    return 0;
}

void showMenu(int *numProcesses, 
                ProcessQueueDescriptor *highPriority,
                ProcessQueueDescriptor *lowPriority,
                ProcessQueueDescriptor *diskQueue,
                ProcessQueueDescriptor *tapeQueue,
                ProcessQueueDescriptor *printerQueue) {
    int choice;

    printf("Ola usuario! Como voce gostaria de realizar a criacao dos processos? \n 1 - Ler do arquivo input.txt \n 2 - Ler do teclado \n 3 - Criar processos com numeros aleatorios \n");
    scanf("%i", &choice);

    // separar em outra função
    switch (choice) {
        case 1:
            readProcessesFromFile(highPriority, lowPriority, diskQueue, tapeQueue, printerQueue);
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

void readProcessesFromFile(ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue) {
    const char* filename = "input.txt";
    FILE* ptr;
    char * line = NULL;
    size_t len = 0;
    size_t read;
 
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        printf("Falha ao abrir o arquivo de input \n");
    }

    while ((read = getline(&line, &len, ptr)) != -1) {
        printf("%s", line);

        char *pt = strtok(line, ",");
        char *part;
        int pid, serviceTime, arrivalTime;

        part = trim(pt);
        pt = strtok(NULL, ",");
        pid = atoi(part);
        // printf("%d\n", pid);

        part = trim(pt);
        pt = strtok(NULL, ",");
        serviceTime = atoi(part);
        // printf("%d\n", serviceTime);

        part = trim(pt);
        pt = strtok(NULL, ",");
        arrivalTime = atoi(part);
        // printf("%d\n", arrivalTime);

        char *IOLine = trim(pt);
        printf("%s\n", IOLine);

        int numIO = 0;
        pt = strtok(IOLine, "/");

        IOQueueElement *IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // crio o array de elementos de fila de IO

        // Para a linha de IO, vamos percorrer até chegar no máximo de IO permitido ou até os IOS acabarem
        for (numIO = 0; numIO < MAX_IO; numIO++) {
            part = pt;
            if (part) {
                pt = strtok(NULL, "/");
            } else break;

            char IOType = part[0];
            int IOInitialTime = part[2] - '0';

            // Crio um elemento da fila de IO
            IOQueueElement element;
            switch (IOType) {
                case 'D':
                    element.deviceQueue = diskQueue; // fila de disco
                    break;
                case 'I':
                    element.deviceQueue = printerQueue; // fila de impressora
                    break;
                case 'F':
                    element.deviceQueue = tapeQueue; // fila de fita
                    break;
                default:
                    printf("Opcao invalida. Escolha uma das seguintes opcoes: 1, 2 ou 3.");
                    exit(1);
            }
            element.initialTime = IOInitialTime;

            *IO = element;
            IO++;

            printf("%c\n", IOType);
            printf("%d\n", IOInitialTime);
        }

        // Criando novo processo
        Process process;
        process.pid = pid;
        process.status = 0; // no início o processo nao esta pronto para ser executado
        process.priority = 1; // prioridade alta
        process.startTime = arrivalTime;
        process.processedTime = 0;
        process.totalTime = serviceTime;
        process.actualIO = 0;
        process.numIO = numIO;
        process.IO = IO;
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

void createNewProcess() {

}

void readProcessesFromKeyboard() {
    int arrivalTime;
    int serviceTime;
    char choice;
    int exit = 0;
    int requestTime;
    for(int i = 0; i<MAX_PROCESSES; i++) {
        printf("Criando %d° processo \n", i);
        printf("Qual o tempo de chegada do processo? \n");
        scanf("%d", &arrivalTime);
        printf("Qual o tempo de serviço do processo? \n");
        scanf("%d", &serviceTime);
        for(int j = 0; j<MAX_IO; j++) {
            printf("Qual dispositivo faz IO? D para disco, F para fita, I para impressora, N para nenhum \n");
            scanf("%c", &choice);
    		switch (choice) {
                case 'I':
                    //armazenar IO do tipo Impressora
                    break;
                case 'D':
                    //armazenar IO do tipo Disco
                    break;
                case 'F':
                    //armazenar IO do tipo Fita
                    break;
                case 'N':
                    exit = 1;
                    break;
                default:
                    printf("Opcao invalida. Escolha uma das seguintes opcoes: 1, 2 ou 3.");
                    if(exit){
                        break;
                    }
    	    }				
        }
    }
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
    Device *device = (Device *) malloc(sizeof(Device));
    device->remainingTime = device->duration = time;
    device->actualProcess = NULL;
    strcpy(device->name, name);

    return device;
}

ProcessQueueDescriptor* createQueue(){
    ProcessQueueDescriptor* queue = (ProcessQueueDescriptor *) malloc(sizeof(ProcessQueueDescriptor));
    queue->head = queue->tail = NULL;

    return queue;
}

void executeDevice(Device *device){
    if(device->actualProcess){
        device->remainingTime--;
    }
}

void checkDeviceEnd(Device *device, ProcessQueueDescriptor *returnQueue){
    if(!device->actualProcess) return;

    if(device->remainingTime == 0){
        addQueue(returnQueue, device->actualProcess);
        printf("- Processo %d saiu do dispositivo %s\n", device->actualProcess->pid, device->name);
        device->actualProcess = NULL;
    }
}

void checkDeviceStart(Device *device, ProcessQueueDescriptor *inputQueue){
    if(device->actualProcess) return;

    device->actualProcess = removeQueue(inputQueue);
    device->remainingTime = device->duration;

    if(device->actualProcess) 
        printf("+ Processo %d entrou no dispositivo %s\n", device->actualProcess->pid, device->name);
}    

void addQueue(ProcessQueueDescriptor *queue, Process *process){
    if(!process) return;

    ProcessQueueElement *processQueue = (ProcessQueueElement *) malloc(sizeof(ProcessQueueElement));
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

void killProcess(Device *cpu, int *numProcesses){
    if(cpu->actualProcess->processedTime == cpu->actualProcess->totalTime){
        *numProcesses -= 1;
        printf("X Processo %d foi finalizado\n", cpu->actualProcess->pid);
        free(cpu->actualProcess);
        cpu->actualProcess = NULL;
    }
}

void checkProcessIO(Device *cpu){
    if (cpu->actualProcess->IO->initialTime == cpu->actualProcess->processedTime) {
        ProcessQueueDescriptor *device = cpu->actualProcess->IO->deviceQueue;
        cpu->actualProcess->IO = (cpu->actualProcess->IO)+1;
        addQueue(device, cpu->actualProcess);
        cpu->actualProcess = NULL;
    } 
}

void executeCPU(Device *cpu, int *numProcesses){
    if(cpu->actualProcess){
        cpu->remainingTime -= 1;
        cpu->actualProcess->processedTime += 1;

        killProcess(cpu, numProcesses);
        
        checkProcessIO(cpu);
    }
}

int handleParameter(char *ps){
    if(ps[0] == '\0') exitProgram(INVALID_NUMBER);

    int value = 0;
    for(char *s = ps; *s != '\0'; s++){
        if(*s < '0' || *s > '9') exitProgram(INVALID_NUMBER);

        value = value * 10 + (*s - '0');
    } 
    return value;
}

void exitProgram(int error){
    if(!error) return;
    printf("#Erro de codigo %d\n",error);
    exit(error);
}