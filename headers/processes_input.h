#ifndef __PROCESSES_INPUT_H__
#define __PROCESSES_INPUT_H__

#include "globals.h"
#include "structures.h"

extern Process* createProcesses(int readProcessesFrom, int *numProcesses, QueueCollection *queues);

#endif