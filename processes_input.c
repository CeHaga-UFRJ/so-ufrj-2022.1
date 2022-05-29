#include "headers/processes_input.h"

char* trim(char* str);
Process newProcess(int pid, int arrivalTime, int serviceTime, int numIO, IOQueueElement *IO);
Process* createProcessesFromFile(int *numProcesses, QueueCollection *queues);
Process* createProcessesFromKeyboard(int *numProcesses, QueueCollection *queues) ;
Process* createRandomProcesses(int *numProcesses, QueueCollection *queues);
Process* createProcesses(int readProcessesFrom, int *numProcesses, QueueCollection *queues);
void sortProcess(Process* processes, int size);
void sortIO(IOQueueElement* IO, int size);

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

Process* createProcessesFromFile(int *numProcesses, QueueCollection *queues) {
    const char* filename = "input.txt";
    FILE* ptr;
    char * line = NULL;
    size_t len = 0;
    size_t read;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES); // crio array de processos
    Process *processesPtr = processes;
 
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        exitProgram(FILE_ERROR, "Falha ao abrir o arquivo de entrada");
    }

    while ((read = getline(&line, &len, ptr)) != -1) {
        char *pt = strtok(line, ",");
        char *part;
        int pid, serviceTime, arrivalTime;

        part = trim(pt);
        pt = strtok(NULL, ",");
        pid = atoi(part);

        part = trim(pt);
        pt = strtok(NULL, ",");
        serviceTime = atoi(part);

        part = trim(pt);
        pt = strtok(NULL, ",");
        arrivalTime = atoi(part);
        
        IOQueueElement *IO;
        int numIO = 0;

        // Se tiver IO
        if(pt){
            char *IOLine = trim(pt);
            pt = strtok(IOLine, "/");
            IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // crio o array de elementos de fila de IO
            IOQueueElement *IOPtr = IO;

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
                        element.deviceQueue = queues->diskQueue; // fila de disco
                        break;
                    case 'I':
                        element.deviceQueue = queues->printerQueue; // fila de impressora
                        break;
                    case 'F':
                        element.deviceQueue = queues->tapeQueue; // fila de fita
                        break;
                    default:
                        exitProgram(INVALID_OPTION, "Opcao invalida. Escolha uma das seguintes opcoes: 1, 2 ou 3.");
                }
                element.initialTime = IOInitialTime;

                *IOPtr = element;
                IOPtr++;
            }
            if(pt) {
                printf("Existem mais entradas e saidas do que o maximo permitido. O processo %d será executado com somente %d entradas e saidas.\n", pid, MAX_IO);
            }
        }
        (*numProcesses)++;
        if(*numProcesses > MAX_PROCESSES) {
            printf("Existem mais processos do que o maximo permitido. Somente %d processos serão executados.\n", MAX_PROCESSES);
            break;
        }

        *processesPtr = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    
    fclose(ptr);
    if (line) free(line);

    return processes;
}

Process* createProcessesFromKeyboard(int *numProcesses, QueueCollection *queues) {
    int choice, i, arrivalTime, serviceTime, numIO;
    int noMoreIO = 0;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);
    Process *processesPtr = processes;

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
        IOQueueElement *IOPtr = IO;

        for(numIO = 0; numIO < MAX_IO; numIO++) {
            numIO == 0 ? printf("Algum dispositivo faz IO? Digite 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum \n") :  printf("Algum outro dispositivo faz IO? D para disco, 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum outro \n");
            scanf("%d", &choice);

            // Crio um elemento para a fila de IO
            IOQueueElement element;
            int IOInitialTime;
    		switch (choice) {
                case 1:
                    element.deviceQueue = queues->diskQueue; // fila de disco
                    printf("-- Lendo as informacoes do IO tipo disco --\n");
                    break;
                case 2:
                    element.deviceQueue = queues->tapeQueue; // fila de fita
                    printf("-- Lendo as informacoes do IO tipo fita --\n");
                    break;
                case 3:
                    element.deviceQueue = queues->printerQueue; // fila de impressora
                    printf("-- Lendo as informacoes do IO tipo impressora --\n");
                    break;
                case 4:
                    noMoreIO = 1;
                    break;
                default:
                    exitProgram(INVALID_OPTION, "Opcao invalida. Escolha uma das seguintes opcoes: 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum.");
    	    }
            
            if(noMoreIO) break;	
            printf("Qual o instante de inicio desse IO?\n");
            scanf("%d", &IOInitialTime);
            element.initialTime = IOInitialTime;
            *IOPtr = element;
            IOPtr++;			
        }
        *processesPtr = newProcess(i+1, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    *numProcesses = i;

    return processes;
}

Process* createRandomProcesses(int *numProcesses, QueueCollection *queues) {
    int i, arrivalTime, serviceTime, numIO, IOType, IOInitialTime, pid;
    int sameInstant = 0;
    int minIOServiceTime = MAX_IO + 2;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);
    Process *processesPtr = processes;

    srand((unsigned) time(NULL));

    *numProcesses = 1 + (rand() % MAX_PROCESSES);

    for(i = 0; i < *numProcesses; i++) {
        pid = i + 1;
        arrivalTime = i == 0 ? 0 : 1 + (rand() % 10);
        serviceTime = 1 + rand() % 10;
        numIO = (rand() % MAX_IO) % serviceTime;
        int *generatedInstants = (int *) calloc(MAX_IO, sizeof(int));

        IOQueueElement *IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // array de IO
        IOQueueElement *IOPtr = IO;

        if (serviceTime > minIOServiceTime) {
            for(int k = 0; k < numIO; k++) {
                // Crio um elemento para a fila de IO
                IOQueueElement element;
                IOType = 1 + (rand() % 3);

                // Um mesmo processo nao pode ter duas IO no mesmo instante
                do {
                    sameInstant = 0;
                    IOInitialTime = 1 + (rand() % (serviceTime - 1));
                    for (int j = 0; j < k; j++) {
                        if (IOInitialTime == generatedInstants[j]) {
                            sameInstant = 1;
                            break;
                        }
                    }
                } while(sameInstant == 1);
                
                generatedInstants[k] = IOInitialTime;

                switch (IOType) {
                    case 1:
                        element.deviceQueue = queues->diskQueue; // fila de disco
                        printf("-- Processo %d tem IO do tipo disco --\n", pid);
                        break;
                    case 2:
                        element.deviceQueue = queues->tapeQueue; // fila de fita
                        printf("-- Processo %d tem IO do tipo fita --\n", pid);
                        break;
                    case 3:
                        element.deviceQueue = queues->printerQueue; // fila de impressora
                        printf("-- Processo %d tem IO do tipo impressora --\n", pid);
                        break;
                    default:
                        printf("Opcao invalida. Erro ao gerar opcao de IO valida.");
                        exitProgram(INVALID_OPTION, "Opcao invalida. Erro ao gerar opcao de IO valida.");
                }
                element.initialTime = IOInitialTime;
                *IOPtr = element;
                IOPtr++;			
            }
            sortIO(IO, numIO);
        }

        if (numIO == 0 || serviceTime < minIOServiceTime) printf("-- Processo %d nao tem IO --\n", pid);
        *processesPtr = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    sortProcess(processes, *numProcesses);
    return processes;
}

Process* createProcesses(int readProcessesFrom, int *numProcesses, QueueCollection *queues) {
    *numProcesses = 0;
    switch (readProcessesFrom) {
        case 1:
            return createProcessesFromFile(numProcesses, queues);
        case 2:
            return createProcessesFromKeyboard(numProcesses, queues);
        case 3:
            return createRandomProcesses(numProcesses, queues);
    }
    exitProgram(INVALID_OPTION, "Opcao invalida. Por favor, escolha uma das seguintes opcoes: 1, 2 ou 3.");
    return NULL;
}

void sortProcess(Process* processes, int size) { 
    for (int i = 0; i < size; i++) { 
        for (int j = i+1; j < size; j++) { 
            if ((processes+i)->arrivalTime > (processes+j)->arrivalTime) { 
                Process aux = *(processes+j); 
                *(processes+j) = *(processes+i); 
                *(processes+i) = aux; 
            }      
        } 
    }   
}

void sortIO(IOQueueElement* IO, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i+1; j < size; j++) {
            if ((IO+i)->initialTime > (IO+j)->initialTime) {
                IOQueueElement aux = *(IO+j);
                *(IO+j) = *(IO+i);
                *(IO+i) = aux;
            }     
        }
    }  
}