#ifndef __MEMORY_ARGS__
#define __MEMORY_ARGS__

#include "../headers/globals.h"

#define FRAMES getFrames()
#define PROCESSES getMaxProcesses()
#define NUM_PAGES getNumPages()
#define WORKING_SET_LIMIT getWorkingSetLimit()
#define WAIT_TIME getWaitTime()
#define STOPPING_LIMIT getStoppingLimit()

extern int getFrames();
extern int getMaxProcesses();
extern int getNumPages();
extern int getWorkingSetLimit();
extern int getWaitTime();
extern int getStoppingLimit();
extern void readArgumentsFromConsole(int argc, char *argv[]);

#endif