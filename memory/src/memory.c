#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/structures.h"

int requestPage();
void pageFault(Process *process, int pageNumber);
int getOldestProcess();

int activeProcesses = 0;
int lastProcessRemoved = 0;

Process* processes[MAX_PROCESSES];
RAM *ram;

int main(){
    srand(time(NULL));

    ram = createRam();
    
    while(1){
        for(int i = 0; i < activeProcesses; i++){
            int requestedPage = requestPage();
            printf("? Processo %d solicitando pagina %d\n", i, requestedPage);

            int address = readPageFromWorkingSet(processes[i], requestedPage);

            if(address != -1){
                printf("$ Pagina encontrada no endereco %d\n",address);
            }else{
                pageFault(processes[i], requestedPage);
            }
            printTLB(processes[i]);
        }
        if(activeProcesses < MAX_PROCESSES) {
            processes[activeProcesses] = createProcess(activeProcesses);

            int requestedPage = requestPage();
            printf("+ Processo %d foi criado e esta solicitando pagina %d\n", activeProcesses, requestedPage);

            pageFault(processes[activeProcesses], requestedPage);

            activeProcesses++;

            printTLB(processes[activeProcesses - 1]);
        }
        sleep(3);
    }
    return 0;
}

int requestPage(){
    return rand() % NUM_PAGES;
}

void pageFault(Process *process, int pageNumber){
    if(process->workingSet->remainingSlots == 0){
        int leastUsedPage = removeLeastUsedPage(process);
        removePageFromRAM(ram, leastUsedPage);

        int address = addPageToRAM(ram);
        addPageToWorkingSet(process, pageNumber, address);

        printf("X Pagina %d removida. Pagina %d sera adicionada no endereco %d\n", leastUsedPage, pageNumber, address);
    }else{
        if(isRAMFull(ram)){
            int oldestPid = getOldestProcess();
            Process *oldestProcess = processes[oldestPid];
            while(!isWSEmpty(oldestProcess)){
                int removePage = removeLeastUsedPage(oldestProcess);
                removePageFromRAM(ram, removePage);
            }
            printf("! Memoria principal cheia, o processo %d sera removido\n", oldestPid);
        }
        int address = addPageToRAM(ram);
        addPageToWorkingSet(process, pageNumber, address);
        printf("> Pagina %d adicionada no endereco %d\n", pageNumber, address);
    }
}

int getOldestProcess(){
    int oldestProcess = lastProcessRemoved;
    lastProcessRemoved = (lastProcessRemoved + 1) % activeProcesses;

    return oldestProcess;
}