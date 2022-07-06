#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/structures.h"

int activeProcesses = 0;
int lastProcessRemoved = 0;

int main(){
    srand(time(NULL));

    Process* processes[MAX_PROCESSES];
    TLB* table = createTable();
    
    while(1){
        for(int i = 0; i < activeProcesses; i++){
            int requestedPage = requestPage();
            printf("? Processo %d solicitando pagina %d\n", i, requestedPage);

            Page *readPage = readPageFromWorkingSet(processes[i], requestedPage);

            if(readPage){
                printf("$ Pagina encontrada no WS\n");
            }else{
                pageFault(processes[i], table, requestedPage);
            }
        }
        if(activeProcesses < MAX_PROCESSES) {
            processes[activeProcesses] = createProcess(activeProcesses);

            int requestedPage = requestPage();
            printf("+ Processo %d foi criado e esta solicitando pagina %d\n", activeProcesses, requestedPage);


            activeProcesses++;
        }
        sleep(3);
    }
    return 0;
}

int requestPage(){
    return rand() % NUM_PAGES;
}

void pageFault(Process *process, TLB *table, int pageNumber){
    if(process->workingSet->remainingSlots == 0){
        removeLeastUsedPage(process);
        addPageToWorkingSet(process, pageNumber);
    }else{
        if(1){
            removeProcessFromTLB(table, getOldestProcess());
        }
        Page *addedPage = addPageToWorkingSet(process, pageNumber);
        addPageToTLB(table, addedPage);
    }
}

int getOldestProcess(){
    int oldestProcess = lastProcessRemoved;
    lastProcessRemoved = (lastProcessRemoved + 1) % activeProcesses;

    return oldestProcess;
}