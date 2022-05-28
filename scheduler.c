#include "scheduler.h"

void scheduler(StructureCollection *structures);
void executeDevice(Device *device);
void checkDeviceEnd(Device *device, ProcessQueueDescriptor *returnQueue);
void checkDeviceStart(Device *device, ProcessQueueDescriptor *inputQueue);
void addNewProcessToQueue(int instant, ProcessQueueDescriptor *queue, StructureCollection *structures);
void killProcess(Device *cpu, int *killedProcesses);
void checkProcessIO(Device *cpu);
void executeCPU(Device *cpu, int *numProcesses);

void scheduler(StructureCollection *structures){
    int killedProcesses = 0;
    for(int instant = 0; killedProcesses < structures->numProcesses && instant < 50; instant++){
        printf("=== Começando instante %d ===\n", instant);
        printf("Atualmente tem %d processos totais, %d processos em execução e %d processos mortos\n", structures->numProcesses, structures->actualProcessIndex, killedProcesses);

        addNewProcessToQueue(instant, structures->queues->highPriority, structures); // adicionar novo processo na fila de alta prioridade

        checkDeviceStart(structures->devices->disk, structures->queues->diskQueue);
        checkDeviceStart(structures->devices->tape, structures->queues->tapeQueue);
        checkDeviceStart(structures->devices->printer, structures->queues->printerQueue);
        checkDeviceStart(structures->devices->cpu, structures->queues->highPriority);
        checkDeviceStart(structures->devices->cpu, structures->queues->lowPriority);

        executeDevice(structures->devices->disk);
        executeDevice(structures->devices->tape);
        executeDevice(structures->devices->printer);

        executeCPU(structures->devices->cpu, &killedProcesses);

        checkDeviceEnd(structures->devices->disk, structures->queues->lowPriority);
        checkDeviceEnd(structures->devices->tape, structures->queues->highPriority);
        checkDeviceEnd(structures->devices->printer, structures->queues->highPriority);
        checkDeviceEnd(structures->devices->cpu, structures->queues->lowPriority);

        printf("\n\n");
    }
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

void addNewProcessToQueue(int instant, ProcessQueueDescriptor *queue, StructureCollection *structures){
    while(structures->actualProcessIndex < structures->numProcesses && structures->processes[structures->actualProcessIndex].arrivalTime == instant){
        Process *actualProcess = &structures->processes[structures->actualProcessIndex];
        addQueue(queue, actualProcess);
        printf("Processo %d criado no instante %d\n", actualProcess->pid, instant);
        structures->actualProcessIndex++;
    }
}

void killProcess(Device *cpu, int *killedProcesses){
    if(cpu->actualProcess->processedTime == cpu->actualProcess->serviceTime){
        printf("X Processo %d foi finalizado\n", cpu->actualProcess->pid);
        cpu->actualProcess = NULL;
        *(killedProcesses) += 1;
    }
}

void checkProcessIO(Device *cpu){
    if(!cpu->actualProcess) return;

    if (cpu->actualProcess->IO->initialTime == cpu->actualProcess->processedTime) {
        ProcessQueueDescriptor *device = cpu->actualProcess->IO->deviceQueue;
        printf("- Processo %d saiu da CPU\n",cpu->actualProcess->pid);
        cpu->actualProcess->IO = (cpu->actualProcess->IO)+1;
        addQueue(device, cpu->actualProcess);
        cpu->actualProcess = NULL;
    } 
}

void executeCPU(Device *cpu, int *killedProcesses){
    if(cpu->actualProcess){
        cpu->remainingTime -= 1;
        cpu->actualProcess->processedTime += 1;

        killProcess(cpu, killedProcesses);
        
        checkProcessIO(cpu);
    }
}
