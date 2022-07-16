#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define MAX_FRAMES 128
#define MAX_PROCESSES 32
#define MAX_PAGES 64

#define HELP_TEXT \
"Simula o funcionamento de um gerenciador de memoria virtual com algoritmo LRU\n\
Para execuder rode:\n\
\t [executavel] [-f#] [-p#] [-v#] [-w#] [-s#] [-l#]\n\
\n\
\tf\t: Quantidade de frames na memoria principal\n\
\tp\t: Quantidade de processos\n\
\tv\t: Quantidade de paginas virtuais em cada processo\n\
\tw\t: Limite do working set\n\
\ts\t: Tempo de espera entre ciclos em segundos\n\
\tl\t: Quantidade maxima de ciclos\n\
"

#define PROCESS_CREATION_ERROR 2
#define RAM_CREATION_ERROR 3
#define INVALID_NUMBER 4
#define INVALID_ARGUMENT 5
#define OVER_MAX 6
#define COUNTER_END 7
#define HELP 8

extern void exitProgram(int error, char *errorMessage);

#endif