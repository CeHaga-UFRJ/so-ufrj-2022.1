#include "headers/globals.h"

void exitProgram(int error, char *errorMessage);

/*
Finaliza o programa e imprime a mensagem de erro
*/
void exitProgram(int error, char *errorMessage){
    if(!error) return;
    printf("%s\n", errorMessage);
    exit(error);
}

int readNumber(long *number){
    char buffer[256];
    char *ptr;
    fgets(buffer, 265, stdin);
    long value = strtol(buffer, &ptr, 10);
    *number = value;

    if(*number == 0 && strcmp(buffer, "0\n")) return 1;

    return 0; 
}

int readNumberWithMin(long *number, long min){
    if(readNumber(number)) return 1;

    if(*number < min) return 1;

    return 0; 
}

int readNumberInRange(long *number, long min, long max){
    if(readNumberWithMin(number, min)) return 1;

    if(*number > max) return 1;

    return 0; 
}
