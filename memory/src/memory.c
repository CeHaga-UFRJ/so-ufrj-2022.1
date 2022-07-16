#include "../headers/globals.h"
#include "../headers/memory_arguments.h"
#include "../headers/structures.h"

int requestPage();
void pageFault(Process *process, int pageNumber);
int getOldestProcess();

int activeProcesses = 0;
int lastProcessRemoved = 0;

Process* processes[MAX_PROCESSES];
RAM *ram;

int main(int argc, char *argv[]){
    srand(time(NULL));

    readArgumentsFromConsole(argc, argv);

    ram = createRam();
    int count = 0;
    while(1){
        if(STOPPING_LIMIT != -1 && count++ >= STOPPING_LIMIT) exitProgram(COUNTER_END, "Contador chegou ao final");
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
        if(activeProcesses < PROCESSES) {
            processes[activeProcesses] = createProcess(activeProcesses);

            int requestedPage = requestPage();
            printf("+ Processo %d foi criado e esta solicitando pagina %d\n", activeProcesses, requestedPage);

            pageFault(processes[activeProcesses], requestedPage);

            printTLB(processes[activeProcesses]);

            activeProcesses++;
        }
        sleep(WAIT_TIME);
    }
    return 0;
}

int requestPage(){
    return rand() % NUM_PAGES;
}

/**
 * @brief Realiza a adicao de pagina na memoria
 * 
 * @param process Processo que teve Page Fault
 * @param pageNumber Numero da pagina requisitada
 */
void pageFault(Process *process, int pageNumber){
    if(process->workingSet->remainingSlots == 0){
        PageValues pv = removeLeastUsedPage(process);
        removePageFromRAM(ram, pv.address);

        int address = addPageToRAM(ram);
        addPageToWorkingSet(process, pageNumber, address);

        printf("X Pagina %d removida. Pagina %d foi adicionada no endereco %d\n", pv.page, pageNumber, address);
    }else{
        if(isRAMFull(ram)){
            int oldestPid = getOldestProcess();
            Process *oldestProcess = processes[oldestPid];
            while(!isWSEmpty(oldestProcess)){
                PageValues pv = removeLeastUsedPage(oldestProcess);
                removePageFromRAM(ram, pv.address);
            }
            printf("! Memoria principal cheia, o processo %d foi removido\n", oldestPid);
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
