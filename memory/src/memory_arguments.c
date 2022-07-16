#include "../headers/memory_arguments.h"

int frames;
int maxProcesses;
int numPages;
int workingSetLimit;
int waitTime;
int stoppingLimit;

/*
 * Verifica se o parametro e um numero
 */
int handleParameter(char *ps){
    if(ps[0] == '\0') exitProgram(INVALID_NUMBER, "Parametro nao possui numero");

    int value = 0;
    for(char *s = ps; *s != '\0'; s++){
        if(*s < '0' || *s > '9') exitProgram(INVALID_NUMBER, "Parametro nao possui um numero valido");

        value = value * 10 + (*s - '0');
    } 
    return value;
}

/*
 * Le os argumentos CLI
 */
void readArgumentsFromConsole(int argc, char *argv[]){
    frames = 64;
    maxProcesses = 20;
    numPages = 50;
    workingSetLimit = 4;
    waitTime = 3;
    stoppingLimit = -1;

    for(int i = 1; i < argc; i++){
        char *arg = argv[i];
        // Se nao for uma flag valida 
        if(arg[0] != '-' || arg[1] == '\0') exitProgram(INVALID_ARGUMENT, "Nao foi passado um argumento valido, use a opcao -h em caso de duvidas");
        char flag = arg[1];
        arg += 2;
        
        switch(flag){
            case 'f':
                frames = handleParameter(arg);
                if(frames > MAX_FRAMES) exitProgram(OVER_MAX, "Mais frames que permitido");
                break;
            case 'p':
                maxProcesses = handleParameter(arg);
                if(maxProcesses > MAX_PROCESSES) exitProgram(OVER_MAX, "Mais processos que permitido");
                break;
            case 'v':
                numPages = handleParameter(arg);
                if(numPages > MAX_PAGES) exitProgram(OVER_MAX, "Mais paginas que permitido");
                break;
            case 'w':
                workingSetLimit = handleParameter(arg);
                break;
            case 's':
                waitTime = handleParameter(arg);
                break;
            case 'l':
                stoppingLimit = handleParameter(arg);
                break;
            case 'h':
                exitProgram(HELP, HELP_TEXT);
                break;
            default:
                exitProgram(INVALID_ARGUMENT, "Nao foi passado um argumento valido, use a opcao -h em caso de duvidas");
        }
    }
}

/**
 * Getters para constantes
 */
int getFrames(){
    return frames;
}

int getMaxProcesses(){
    return maxProcesses;
}

int getNumPages(){
    return numPages;
}

int getWorkingSetLimit(){
    return workingSetLimit;
}

int getWaitTime(){
    return waitTime;
}

int getStoppingLimit(){
    return stoppingLimit;
}
