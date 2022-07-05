#include "../headers/structures.h"
#include <stdlib.h>

// TO-DO
// Da ruim ao apagar no inicio ou final
void removeProcessFromTLB(TLB *table, int pid){
    for(int i = 0; i < FRAMES; i++){
        if(table->rows[i]->pid == pid){
            free(table->rows[i]);
        }
    }
}

void cleanWorkingSet(Process **process){
    (*process)->workingSet.totalPages = 0;
    PageElement *element = (*process)->workingSet.head;

    while(element != NULL){
        PageElement *aux = element;
        element = element->next;
        free(aux);
    }

    (*process)->workingSet.head = NULL;
    (*process)->workingSet.tail = NULL;
}

void removeLeastUsedPage(Process **process){
    (*process)->workingSet.head = (*process)->workingSet.head->next;
    (*process)->workingSet.head->prev = NULL;
    (*process)->workingSet.totalPages--;
}

// TO-DO
// Da ruim ao adicionar no inicio
void addPageToWorkingSet(Process **process, int pageNumber){
    (*process)->workingSet.tail->next = (PageElement *)malloc(sizeof(PageElement));
    (*process)->workingSet.tail->next->prev = (*process)->workingSet.tail;
    (*process)->workingSet.tail->next->next = NULL;

    (*process)->workingSet.tail = (*process)->workingSet.tail->next;
    (*process)->workingSet.totalPages++;

    (*process)->workingSet.tail->page.pid = (*process)->pid;
    (*process)->workingSet.tail->page.pageNumber = pageNumber;
}

// TODO
// Verificar se faz sentido nos casos extremos
int readPageFromWorkingSet(Process **process, int pageNumber){
    PageElement *element = (*process)->workingSet.head;
    while(element != NULL){
        if(element->page.pageNumber == pageNumber){
            if(element->prev) element->prev->next = element->next;
            if(element->next) element->next->prev = element->prev;

            element->next = NULL;
            element->prev = (*process)->workingSet.tail;
            (*process)->workingSet.tail->next = element;

            (*process)->workingSet.tail = element;

            return 0;
        }
    }
    return 1;
}

void addPageToTLB(TLB *table, Page *addedPage){
    for(int i = 0; i < FRAMES; i++){
        if(table->rows[i] == NULL){
            table->rows[i] = addedPage;
            break;
        }
    }
}

