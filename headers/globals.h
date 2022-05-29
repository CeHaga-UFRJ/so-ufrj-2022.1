#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define NO_ERROR 0
#define HELP 3
#define INVALID_NUMBER 4
#define INVALID_OPTION 5
#define FILE_ERROR 6
#define INVALID_ARGUMENT 7
#define HELP_TEXT \
"Simula o funcionamento de um escalonador de processos com escalonamento circular\n\
Para executar rode:\n\
\t scheduler [-q#] [-d#] [-t#] [-p#]\n\
\n\
\tq\t: Tamanho do quantum (Time slice)\n\
\td\t: Tempo de leitura do disco\n\
\tt\t: Tempo de leitura da fita magnetica\n\
\tp\t: Tempo de leitura da impressora\n\
"

#define READY 0
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern void exitProgram(int error, char *errorMessage);
extern int readNumber(long *number);
extern int readNumberWithMin(long *number, long min);
extern int readNumberInRange(long *number, long min, long max);

#endif
