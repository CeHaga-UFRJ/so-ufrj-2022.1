#ifndef __MAIN_H__
#define __MAIN_H__

#define NO_ERROR 0
#define INVALID_NUMBER 3
#define INVALID_ARGUMENT 4
#define HELP \
"Simula o funcionamento de um escalonador de processos com escalonamento circular\n\
Para executar rode:\n\
\t scheduler [-q#] [-d#] [-t#] [-p#]\n\
\n\
\tq\t: Tamanho do quantum (Time slice)\n\
\td\t: Tempo de leitura do disco\n\
\tt\t: Tempo de leitura da fita magnetica\n\
\tp\t: Tempo de leitura da impressora\n\
"

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "scheduler_arguments.h"

#endif
