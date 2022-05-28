#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_IO 3

#define NO_ERROR 0
#define INVALID_NUMBER 3
#define INVALID_ARGUMENT 4
#define INVALID_OPTION 5
#define FILE_ERROR 6
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

#define READY 0
#define HIGH_PRIORITY 1
#define LOW_PRIORITY 2

/* Headers */
typedef struct Process Process;
typedef struct Device Device;
typedef struct ProcessQueueElement ProcessQueueElement;
typedef struct ProcessQueueDescriptor ProcessQueueDescriptor;
typedef struct IOQueueElement IOQueueElement;

Process* createProcesses(int readProcessesFrom, int *numProcesses, ProcessQueueDescriptor *highPriority, ProcessQueueDescriptor *lowPriority, ProcessQueueDescriptor *diskQueue, ProcessQueueDescriptor *tapeQueue, ProcessQueueDescriptor *printerQueue);
Device* createDevice(int time, char *name);
ProcessQueueDescriptor* createQueue();
void executeDevice(Device *device);
void executeCPU(Device *cpu, int *numProcesses);
void checkDeviceEnd(Device *device, ProcessQueueDescriptor *returnQueue);
void checkDeviceStart(Device *device, ProcessQueueDescriptor *inputQueue);
void addQueue(ProcessQueueDescriptor *queue, Process *process);
Process* removeQueue(ProcessQueueDescriptor *queue);
Process newProcess(int pid, int arrivalTime, int serviceTime, int numIO, IOQueueElement *IO);
void addNewProcessToQueue(int instant, ProcessQueueDescriptor *queue);
void killProcess(Device *cpu, int *numProcesses);
void checkProcessIO(Device *cpu);
int showMenu();
char* trim(char* str);
Process* createProcessesFromFile(int *numProcesses, ProcessQueueDescriptor *highPriority, ProcessQueueDescriptor *lowPriority, ProcessQueueDescriptor *diskQueue, ProcessQueueDescriptor *tapeQueue, ProcessQueueDescriptor *printerQueue);
Process* createProcessesFromKeyboard(int *numProcesses, ProcessQueueDescriptor *highPriority, ProcessQueueDescriptor *lowPriority, ProcessQueueDescriptor *diskQueue, ProcessQueueDescriptor *tapeQueue, ProcessQueueDescriptor *printerQueue);
Process* createRandomProcesses(int *numProcesses, ProcessQueueDescriptor *highPriority, ProcessQueueDescriptor *lowPriority, ProcessQueueDescriptor *diskQueue, ProcessQueueDescriptor *tapeQueue, ProcessQueueDescriptor *printerQueue);
int handleParameter(char *ps);
void exitProgram(int error);

/* Data structures */

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

    // Cria as filas de prioridade e dos dispositivos de IO
    ProcessQueueDescriptor *highPriority = createQueue();
    ProcessQueueDescriptor *lowPriority = createQueue();
    ProcessQueueDescriptor *diskQueue = createQueue();
    ProcessQueueDescriptor *tapeQueue = createQueue();
    ProcessQueueDescriptor *printerQueue = createQueue();

    int numProcesses;
    int readProcessesFrom = showMenu();

    // Vetor de processos
    Process *processes = createProcesses(readProcessesFrom, &numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue); // TODO

    printf("%d processos criados\n", numProcesses);

    for(int instant = 0; numProcesses; instant++){
        printf("=== Começando instante %d ===\n", instant);

        addNewProcessToQueue(instant, highPriority); // adicionar novo processo na fila de alta prioridade

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

int showMenu() {
    int choice;
    printf("Ola usuario, bem vindo ao simulador de escalonamento de processos! \nComo voce gostaria de realizar a criacao dos processos? \n 1 - Criar a partir do arquivo input.txt \n 2 - Criar a partir do teclado \n 3 - Criar com numeros aleatorios \n");
    printf("Sua escolha: "), 
    scanf("%i", &choice);
    return choice;
}

Process* createProcesses(
                    int readProcessesFrom,
                    int *numProcesses,
                    ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue) {
    *numProcesses = 0;
    switch (readProcessesFrom) {
        case 1:
            createProcessesFromFile(numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue);
            break;
        case 2:
            createProcessesFromKeyboard(numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue);
            break;
        case 3:
            createRandomProcesses(numProcesses, highPriority, lowPriority, diskQueue, tapeQueue, printerQueue);
            break;
        default:
            printf("Opcao invalida. Por favor, escolha uma das seguintes opcoes: 1, 2 ou 3.");
            exitProgram(INVALID_OPTION);
    }

    return NULL;
}

Process* createProcessesFromFile(
                    int *numProcesses,
                    ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue) {
    const char* filename = "input.txt";
    FILE* ptr;
    char * line = NULL;
    size_t len = 0;
    size_t read;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES); // crio array de processos
 
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        printf("Falha ao abrir o arquivo de entrada \n");
        exitProgram(FILE_ERROR);
    }

    while ((read = getline(&line, &len, ptr)) != -1) {
        // printf("%s", line);

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
        // printf("%s\n", IOLine);

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
                    exitProgram(INVALID_OPTION);
            }
            element.initialTime = IOInitialTime;

            *IO = element;
            IO++;

            // printf("%c\n", IOType);
            // printf("%d\n", IOInitialTime);
        }

        if(pt) {
            printf("Existem mais entradas e saidas do que o maximo permitido. O processo %d será executado com somente %d entradas e saidas.\n", pid, MAX_IO);
        }

        (*numProcesses)++;
        if(*numProcesses > MAX_PROCESSES) {
            printf("Existem mais processos do que o maximo permitido. Somente %d processos serão executados.\n", MAX_PROCESSES);
            break;
        }

        *processes = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        // printf("Id do processo = %d\n", processes->pid);
        processes++;
    }
    
    fclose(ptr);
    if (line) free(line);

    return processes;
}

Process* createProcessesFromKeyboard(
                    int *numProcesses,
                    ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue) {
    int choice, i, arrivalTime, serviceTime, numIO;
    int noMoreIO = 0;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);

    for(i = 0; i < MAX_PROCESSES; i++) {
        if (i != 0) {
            int choice;
            printf("Voce deseja continuar criando processos? Digite 1 para sim e 2 para não.\n");
            printf("Sua escolha: ");
            scanf("%d", &choice);
            if (choice == 2) break;
        }
        printf("=== Informacoes do %d° processo === \n", i+1);
        printf("Qual o tempo de chegada do processo? \n");
        scanf("%d", &arrivalTime);
        printf("Qual o tempo de serviço do processo? \n");
        scanf("%d", &serviceTime);

        IOQueueElement *IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // array de IO

        for(numIO = 0; numIO < MAX_IO; numIO++) {
            numIO == 0 ? printf("Algum dispositivo faz IO? Digite 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum \n") :  printf("Algum outro dispositivo faz IO? D para disco, 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum outro \n");
            scanf("%d", &choice);

            // Crio um elemento para a fila de IO
            IOQueueElement element;
            int IOInitialTime;
    		switch (choice) {
                case 1:
                    element.deviceQueue = diskQueue; // fila de disco
                    printf("-- Lendo as informacoes do IO tipo disco --\n");
                    break;
                case 2:
                    element.deviceQueue = tapeQueue; // fila de fita
                    printf("-- Lendo as informacoes do IO tipo fita --\n");
                    break;
                case 3:
                    element.deviceQueue = printerQueue; // fila de impressora
                    printf("-- Lendo as informacoes do IO tipo impressora --\n");
                    break;
                case 4:
                    noMoreIO = 1;
                    break;
                default:
                    printf("Opcao invalida. Escolha uma das seguintes opcoes: 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum.");
                    exitProgram(INVALID_OPTION);
    	    }
            
            if(noMoreIO) break;	
            printf("Qual o instante de inicio desse IO?\n");
            scanf("%d", &IOInitialTime);
            element.initialTime = IOInitialTime;
            *IO = element;
            IO++;			
        }
        *processes = newProcess(i+1, arrivalTime, serviceTime, numIO, IO);
        //printf("Id do processo = %d\n", processes->pid);
        processes++;
    }
    *numProcesses = i;
    return processes;
}

Process* createRandomProcesses(
                    int *numProcesses,
                    ProcessQueueDescriptor *highPriority,
                    ProcessQueueDescriptor *lowPriority,
                    ProcessQueueDescriptor *diskQueue,
                    ProcessQueueDescriptor *tapeQueue,
                    ProcessQueueDescriptor *printerQueue) {
    int i, arrivalTime, serviceTime, numIO, IOType, IOInitialTime, pid;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);

    srand((unsigned) time(NULL));

    *numProcesses = 1 + (rand() % MAX_PROCESSES);

    for(i = 0; i < *numProcesses; i++) {
        pid = i + 1;
        arrivalTime = rand() % 100;
        serviceTime = rand() % 100;
        numIO = rand() % MAX_IO;

        IOQueueElement *IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // array de IO

        for(int i = 0; i < numIO; i++) {
            // Crio um elemento para a fila de IO
            IOQueueElement element;
            IOType = 1 + (rand() % 3);
            IOInitialTime = 1 + (rand() % 3);
    		switch (IOType) {
                case 1:
                    element.deviceQueue = diskQueue; // fila de disco
                    printf("-- Processo %d tem IO do tipo disco --\n", pid);
                    break;
                case 2:
                    element.deviceQueue = tapeQueue; // fila de fita
                    printf("-- Processo %d tem IO do tipo fita --\n", pid);
                    break;
                case 3:
                    element.deviceQueue = printerQueue; // fila de impressora
                    printf("-- Processo %d tem IO do tipo impressora --\n", pid);
                    break;
                default:
                    printf("Opcao invalida. Erro ao gerar opcao de IO valida.");
                    exitProgram(INVALID_OPTION);
    	    }
            element.initialTime = IOInitialTime;
            *IO = element;
            IO++;			
        }
        if (numIO == 0) printf("-- Processo %d nao tem IO --\n", pid);
        *processes = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        processes++;
    }

    return processes;
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

Process newProcess(int pid, int arrivalTime, int serviceTime, int numIO, IOQueueElement *IO) {
    printf("Criando o processo %d\n", pid);
    Process process;
    process.pid = pid;
    process.status = READY; 
    process.priority = HIGH_PRIORITY;
    process.arrivalTime = arrivalTime;
    process.processedTime = 0;
    process.serviceTime = serviceTime;
    process.actualIO = 0;
    process.numIO = numIO;
    process.IO = IO;

    return process;
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

void addNewProcessToQueue(int instant, ProcessQueueDescriptor *queue){
    // print(Processo criado);
}

void killProcess(Device *cpu, int *numProcesses){
    if(cpu->actualProcess->processedTime == cpu->actualProcess->serviceTime){
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