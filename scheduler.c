#include "scheduler.h"

void scheduler(StructureCollection *structures){
    for(int instant = 0; instant < structures->numProcesses; instant++){
        printf("=== Começando instante %d ===\n", instant);

        addNewProcessToQueue(instant, structures->queues->highPriority); // adicionar novo processo na fila de alta prioridade

        checkDeviceStart(structures->devices->disk, structures->queues->diskQueue);
        checkDeviceStart(structures->devices->tape, structures->queues->tapeQueue);
        checkDeviceStart(structures->devices->printer, structures->queues->printerQueue);
        checkDeviceStart(structures->devices->cpu, structures->queues->highPriority);
        checkDeviceStart(structures->devices->cpu, structures->queues->lowPriority);

        executeDevice(structures->devices->disk);
        executeDevice(structures->devices->tape);
        executeDevice(structures->devices->printer);

        executeCPU(structures->devices->cpu, &structures->numProcesses);

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
