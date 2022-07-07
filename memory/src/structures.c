#include "../headers/structures.h"

int readPageFromWorkingSet(Process *process, int pageNumber);
Process* createProcess(int pid);
RAM* createRam();
void addPageToWorkingSet(Process *process, int pageNumber, int address);
PageValues removeLeastUsedPage(Process *process);
void removePageFromRAM(RAM *ram, int page);
int addPageToRAM(RAM *ram);
int isRAMFull(RAM *ram);
int isWSEmpty(Process *process);
void printTLB(Process *process);

Process* createProcess(int pid){
    Process *process = (Process *)malloc(sizeof(Process));
    if(process == NULL){
        exit(PROCESS_CREATION_ERROR);
    }

    process->pid = pid;
    process->workingSet = (WS *)malloc(sizeof(WS));

    if(process->workingSet == NULL){
        exit(PROCESS_CREATION_ERROR);
    }

    process->workingSet->head = NULL;
    process->workingSet->tail = NULL;
    process->workingSet->remainingSlots = WORKING_SET_LIMIT;

    for(int i = 0; i < NUM_PAGES; i++){
        process->workingSet->rows[i] = -1;
    }
    // cleanWorkingSetList(process->workingSet);

    return process;
}

RAM* createRam(){
    RAM* ram = (RAM *)malloc(sizeof(RAM));

    if(!ram){
        exit(RAM_CREATION_ERROR);
    }

    ram->remainingSlots = FRAMES;
    for(int i = 0; i < FRAMES; i++){
        ram->addresses[i] = 0;
    }
    
    return ram;
}

void cleanWorkingSet(Process *process){
    process->workingSet->remainingSlots = WORKING_SET_LIMIT;
    PageElement *element = process->workingSet->head;

    while(element->next != NULL){
        element = element->next;
        free(element->prev);
    }
    free(element);

    process->workingSet->head = NULL;
    process->workingSet->tail = NULL;
    // cleanWorkingSetList(process->workingSet);
}

void cleanWorkingSetList(WS* workingSet) {
    workingSet->head = workingSet->tail = NULL;
}

/* 
    * Descricao: Remove a pagina menos recentemente usada do WS do processo 
    * Parametros: O processo 
*/
PageValues removeLeastUsedPage(Process *process){
    int page = process->workingSet->head->pageNumber;
    int address = process->workingSet->rows[page];

    PageValues pv;
    pv.address = address;
    pv.page = page;
    
    PageElement *aux = process->workingSet->head;
    process->workingSet->head = process->workingSet->head->next;
    free(aux);
    if(process->workingSet->head) process->workingSet->head->prev = NULL;
    process->workingSet->remainingSlots++;

    process->workingSet->rows[page] = -1;

    return pv;
}

// TO-DO
// Da ruim ao adicionar no inicio

/* 
    * Descricao: Adiciona uma pagina do working set do processo
    * Parametros: O processo e o numero da pagina desejada para adicao
    * Retorno: A pagina adicionada no WS do processo
*/
void addPageToWorkingSet(Process *process, int pageNumber, int address){
    if(process->workingSet->head == NULL){
        process->workingSet->head = (PageElement *)malloc(sizeof(PageElement));
        process->workingSet->tail = process->workingSet->head;
        process->workingSet->tail->next = NULL;
        process->workingSet->tail->prev = NULL;
    }else{
        process->workingSet->tail->next = (PageElement *)malloc(sizeof(PageElement));
        process->workingSet->tail->next->prev = process->workingSet->tail;
        process->workingSet->tail->next->next = NULL;
        process->workingSet->tail = process->workingSet->tail->next;
    }
    
    process->workingSet->remainingSlots--;
    process->workingSet->tail->pageNumber = pageNumber;

    process->workingSet->rows[pageNumber] = address;
}

// TODO
// Verificar se faz sentido nos casos extremos

/* 
    * Descricao: Le uma pagina do working set do processo
    * Parametros: O processo e o numero da pagina desejada para leitura
    * Retorno: Retorna a pagina caso ela exista no WS ou null caso contrario
*/
int readPageFromWorkingSet(Process *process, int pageNumber){
    PageElement *element = process->workingSet->head;
    while(element != NULL){
        if(element->pageNumber == pageNumber){
            // Retira elemento da lista
            if(element->prev) element->prev->next = element->next;
            else process->workingSet->head = element->next;

            if(element->next) element->next->prev = element->prev;
            else process->workingSet->tail = process->workingSet->tail->prev;

            // Adiciona no final
            process->workingSet->tail->next = element;
            element->prev = process->workingSet->tail;
            element->next = NULL;
            process->workingSet->tail = element;

            return process->workingSet->rows[pageNumber];
        }
        element = element->next;
    }
    return -1;
}

int addPageToRAM(RAM *ram){
    for(int i = 0; i < FRAMES; i++){
        if(ram->addresses[i] == 0){
            ram->addresses[i] = 1;
            ram->remainingSlots--;
            return i;
        }
    }
    return -1;
}

void removePageFromRAM(RAM *ram, int page){
    ram->addresses[page] = 0;
    ram->remainingSlots++;
}

int isRAMFull(RAM *ram){
    return ram->remainingSlots == 0;
}

int isWSEmpty(Process *process){
    return process->workingSet->head == NULL;
}

void printTLB(Process *process){
    printf("-------\n");
    printf("| P%-2d |\n",process->pid);
    printf("-------\n");
    for(int i = 0; i < NUM_PAGES; i++){
        int address = process->workingSet->rows[i];
        if(address == -1) continue;
        printf("|%02d|%02d|\n", i, address);
        printf("-------\n");
    }
}
