#include "../headers/structures.h"
#include <stdlib.h>

Page* readPageFromWorkingSet(Process *process, int pageNumber);
void removeProcessFromTLB(TLB *table, int pid);
Process* createProcess(int pid);
TLB* createTable();
Page* addPageToWorkingSet(Process *process, int pageNumber);
void removeLeastUsedPage(Process *process);
void addPageToTLB(TLB *table, Page *addedPage);

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

    //process->workingSet->head = NULL;
    //process->workingSet->tail = NULL;
    //process->workingSet->remainingSlots = WORKING_SET_LIMIT;
    cleanWorkingSetList(process->workingSet);

    return process;
}

TLB* createTable(){
    TLB* table = (TLB *)malloc(sizeof(TLB));

    if(!table){
        exit(TABLE_CREATION_ERROR);
    }

    table->remainingSlots = FRAMES;
    
    return table;
}

// TO-DO
// Da ruim ao apagar no inicio ou final
void removeProcessFromTLB(TLB *table, int pid){
    for(int i = 0; i < FRAMES; i++){
        if(table->rows[i]->pid == pid){
            free(table->rows[i]);
            table->remainingSlots++;
        }
    }
}

void cleanWorkingSet(Process *process){
    process->workingSet->remainingSlots = WORKING_SET_LIMIT;
    PageElement *element = process->workingSet->head;

    while(element != NULL){
        PageElement *aux = element;
        element = element->next;
        free(aux);
    }

    //process->workingSet->head = NULL;
    //process->workingSet->tail = NULL;
    cleanWorkingSetList(process->workingSet);
}

void cleanWorkingSetList(WS* workingSet) {
    workingSet->head = workingSet->tail = NULL;
}

/* 
    * Descricao: Remove a pagina menos recentemente usada do WS do processo 
    * Parametros: O processo 
*/
void removeLeastUsedPage(Process *process){
    process->workingSet->head = process->workingSet->head->next;
    process->workingSet->head->prev = NULL;
    process->workingSet->remainingSlots++;
}

// TO-DO
// Da ruim ao adicionar no inicio

/* 
    * Descricao: Adiciona uma pagina do working set do processo
    * Parametros: O processo e o numero da pagina desejada para adicao
    * Retorno: A pagina adicionada no WS do processo
*/
Page* addPageToWorkingSet(Process *process, int pageNumber){
    process->workingSet->tail->next = (PageElement *)malloc(sizeof(PageElement));
    process->workingSet->tail->next->prev = process->workingSet->tail;
    process->workingSet->tail->next->next = NULL;

    process->workingSet->tail = process->workingSet->tail->next;
    process->workingSet->remainingSlots--;

    Page *page = (Page *) malloc(sizeof(Page));
    page->pid = process->pid;
    page->pageNumber = pageNumber;
    process->workingSet->tail->page = page;

    return page;
}

// TODO
// Verificar se faz sentido nos casos extremos

/* 
    * Descricao: Le uma pagina do working set do processo
    * Parametros: O processo e o numero da pagina desejada para leitura
    * Retorno: Retorna a pagina caso ela exista no WS ou null caso contrario
*/
Page* readPageFromWorkingSet(Process *process, int pageNumber){
    PageElement *element = process->workingSet->head;
    while(element != NULL){
        if(element->page->pageNumber == pageNumber){
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

            return element;
        }
    }
    return NULL;
}

/* 
    * Descricao: Adiciona pagina na tabela de paginas virtuais 
    * Parametros: tabela de paginacao virtual e pagina a ser adicionada
*/
void addPageToTLB(TLB *table, Page *addedPage){
    for(int i = 0; i < FRAMES; i++){
        if(table->rows[i] == NULL){
            table->rows[i] = addedPage;
            table->remainingSlots--;
            break;
        }
    }
}

void printTLB(TLB *table){
    for(int row = 0; row < FRAMES; row++){
        for(int i = 0; i < 10; i++) printf("-");
        printf("|%02d|%02d|%02d|", row, table->rows[row]->pid, table->rows[row]->pageNumber)
    }
}
