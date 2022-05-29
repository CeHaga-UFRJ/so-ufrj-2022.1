#include "../headers/processes_input.h"

char* trim(char* str);
Process newProcess(int pid, int arrivalTime, int serviceTime, int numIO, IOQueueElement *IO);
Process* createProcessesFromFile(int *numProcesses, QueueCollection *queues);
Process* createProcessesFromKeyboard(int *numProcesses, QueueCollection *queues) ;
Process* createRandomProcesses(int *numProcesses, QueueCollection *queues);
Process* createProcesses(int readProcessesFrom, int *numProcesses, QueueCollection *queues);
int newIoInitialTime(IOQueueElement *IO, int currentNumberOfIO, int serviceTime);
int isSameInstant(IOQueueElement *IO, int initialTime, int currentNumberOfIO);
void sortProcess(Process* processes, int size);
void sortIO(IOQueueElement* IO, int size);


/*
 * Remove espacos iniciais e finais na string
 */
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

/*
 * Cria um novo processo dada suas informacoes
 */
Process newProcess(int pid, int arrivalTime, int serviceTime, int numIO, IOQueueElement *IO) {
    printf("=== Criando o processo %d ===\n", pid);
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

    printf("-> Tempo de chegada: %d \n-> Tempo de serviço: %d \n-> Quantidade de IOs: %d \n", arrivalTime, serviceTime, numIO);
    IOQueueElement *IOPtr = IO;

    for (int i = 0; i < numIO; i++) {
        printf("-> IO do tipo %s no instante %d\n", IOPtr->type, IOPtr->initialTime);
        IOPtr++;
    }

    printf("\n");

    return process;
}

/*
 * Cria os processos a partir do arquivo input.txt
 */
Process* createProcessesFromFile(int *numProcesses, QueueCollection *queues) {
    const char* filename = "../in/input.txt";
    FILE* ptr;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    // Array de processos
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);
    Process *processesPtr = processes;
 
    ptr = fopen(filename, "r");

    if (NULL == ptr) {
        exitProgram(FILE_ERROR, "Falha ao abrir o arquivo de entrada");
    }

    while ((read = getline(&line, &len, ptr)) != -1) {
        char *pt = strtok(line, ",");
        char *part;
        int pid, serviceTime, arrivalTime;

        // Leitura pid
        part = trim(pt);
        pt = strtok(NULL, ",");
        pid = atoi(part);

        // Leitura tempo de servico
        part = trim(pt);
        pt = strtok(NULL, ",");
        serviceTime = atoi(part);

        // Leitura tempo de chegada
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

            // Para a linha de IO, vamos percorrer ate chegar no maximo de IO permitido ou ate os IOS acabarem
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
                        element.deviceQueue = queues->diskQueue; 
                        element.type = "disco";
                        break;
                    case 'I':
                        element.deviceQueue = queues->printerQueue; 
                        element.type = "impressora";
                        break;
                    case 'F':
                        element.deviceQueue = queues->tapeQueue; 
                        element.type = "fita";
                        break;
                    default:
                        exitProgram(INVALID_OPTION, "Opcao invalida. Escolha uma das seguintes opcoes: 1, 2 ou 3.");
                }
                element.initialTime = IOInitialTime;

                if (isSameInstant(IO, IOInitialTime, numIO)) {
                    exitProgram(INVALID_ARGUMENT, "Argumento inválido. Duas operações de IO não podem começar ao mesmo tempo. Verifique o arquivo 'input.txt'.");
                }

                *IOPtr = element;
                IOPtr++;
            }
            sortIO(IO, numIO);
            if(pt) {
                printf("Existem mais entradas e saidas do que o maximo permitido. O processo %d sera executado com somente %d entradas e saidas.\n", pid, MAX_IO);
            }
        }
        (*numProcesses)++;
        if(*numProcesses > MAX_PROCESSES) {
            printf("Existem mais processos do que o maximo permitido. Somente %d processos serao executados.\n", MAX_PROCESSES);
            (*numProcesses)--;
            break;
        }

        *processesPtr = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    
    fclose(ptr);
    if (line) free(line);

    sortProcess(processes, *numProcesses);
    return processes;
}

/*
 * Cria os processos a partir da leitura da stdin
 */
Process* createProcessesFromKeyboard(int *numProcesses, QueueCollection *queues) {
    int i, numIO;
    long choice, arrivalTime, serviceTime;
    int noMoreIO = 0;
    Process *processes = (Process *) malloc(sizeof(Process) * MAX_PROCESSES);
    Process *processesPtr = processes;

    for(i = 0; i < MAX_PROCESSES; i++) {
        if (i != 0) {
            printf("Voce deseja continuar criando processos? Digite 1 para sim e 2 para não.\n");
            printf("Sua escolha: ");
            while(readNumberInRange(&choice, 1, 2)){
                printf("Opcao invalida, entre com uma das opcoes\n");
            }
            if (choice == 2) break;
        }
        printf("=== Informacoes do %d° processo === \n", i+1);
        printf("Qual o tempo de chegada do processo? (Minimo 0)\n");
        while(readNumberWithMin(&arrivalTime, 0)){
            printf("Entre com um numero\n");
        }
        printf("Qual o tempo de serviço do processo? (Minimo 1)\n");
        while(readNumberWithMin(&serviceTime, 1)){
            printf("Entre com um numero\n");
        }

        IOQueueElement *IO = (IOQueueElement *) malloc(sizeof(IOQueueElement) * MAX_IO); // array de IO
        IOQueueElement *IOPtr = IO;

        if(serviceTime > 1){
            for(numIO = 0; numIO < MAX_IO; numIO++) {
                numIO == 0 ? printf("Algum dispositivo faz IO? Digite 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum \n") :  printf("Algum outro dispositivo faz IO? D para disco, 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum outro \n");
                while(readNumberInRange(&choice, 1, 4)){
                    printf("Opcao invalida, entre com uma das opcoes\n");
                }

                // Crio um elemento para a fila de IO
                IOQueueElement element;
                long IOInitialTime;
                switch (choice) {
                    case 1:
                        element.deviceQueue = queues->diskQueue; 
                        element.type = "disco";
                        printf("-- Lendo as informacoes do IO tipo disco --\n");
                        break;
                    case 2:
                        element.deviceQueue = queues->tapeQueue;
                        element.type = "fita";
                        printf("-- Lendo as informacoes do IO tipo fita --\n");
                        break;
                    case 3:
                        element.deviceQueue = queues->printerQueue;
                        element.type = "impressora";
                        printf("-- Lendo as informacoes do IO tipo impressora --\n");
                        break;
                    case 4:
                        noMoreIO = 1;
                        break;
                    default:
                        exitProgram(INVALID_OPTION, "Opcao invalida. Escolha uma das seguintes opcoes: 1 para disco, 2 para fita, 3 para impressora, 4 para nenhum.");
                }
                
                if(noMoreIO) break;
                
                int sameTime;
                do {
                    printf("Qual o instante de inicio desse IO? (Minimo 1, Maximo %ld)\n", serviceTime - 1);
                    while(readNumberInRange(&IOInitialTime, 1, serviceTime - 1)){
                        printf("Entre com um numero\n");
                    }
                    sameTime = isSameInstant(IO, IOInitialTime, numIO);
                    if (sameTime) {
                        printf("Já existe outro IO com esse instante, digite outro\n");
                    }
                } while(sameTime);

                element.initialTime = IOInitialTime;

                

                *IOPtr = element;
                IOPtr++;			
            }
            sortIO(IO, numIO);
        }
        *processesPtr = newProcess(i+1, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    *numProcesses = i;

    sortProcess(processes, *numProcesses);
    return processes;
}

/*
 * Cria os processos a partir de numeros aleatorios
 */
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
            for(int currentNumberOfIO = 0; currentNumberOfIO < numIO; currentNumberOfIO++) {
                // Crio um elemento para a fila de IO
                IOQueueElement element;
                IOType = 1 + (rand() % 3);

                
                IOInitialTime = newIoInitialTime(IO, currentNumberOfIO, serviceTime);
                
                generatedInstants[currentNumberOfIO] = IOInitialTime;
                element.type = (char *) malloc(sizeof(char) * 10);

                switch (IOType) {
                    case 1:
                        element.deviceQueue = queues->diskQueue; 
                        element.type = "disco";
                        break;
                    case 2:
                        element.deviceQueue = queues->tapeQueue; 
                        element.type = "fita";
                        break;
                    case 3:
                        element.deviceQueue = queues->printerQueue;
                        element.type = "impressora";
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
        } else numIO = 0;

        *processesPtr = newProcess(pid, arrivalTime, serviceTime, numIO, IO);
        processesPtr++;
    }
    sortProcess(processes, *numProcesses);
    return processes;
}

/*
 * Menu de criacao de processos
 */
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

int newIoInitialTime(IOQueueElement *IO, int currentNumberOfIO, int serviceTime) {
    int IOInitialTime;
    // Um mesmo processo nao pode ter duas IO no mesmo instante
    do {
        IOInitialTime = 1 + (rand() % (serviceTime - 1));
    } while(isSameInstant(IO, IOInitialTime, currentNumberOfIO));
    return IOInitialTime;
}

int isSameInstant(IOQueueElement *IO, int initialTime, int currentNumberOfIO) {
    for (int j = 0; j < currentNumberOfIO; j++) 
        if (initialTime == (IO+j)->initialTime) 
            return 1;
            
    return 0;
}