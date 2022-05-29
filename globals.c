#include "headers/globals.h"

void exitProgram(int error, char *errorMessage);
int readNumber(long *number);
int readNumberWithMin(long *number, long min);
int readNumberInRange(long *number, long min, long max);

/*
 * Finaliza o programa e imprime a mensagem de erro
 */
void exitProgram(int error, char *errorMessage){
    if(!error) return;
    printf("%s\n", errorMessage);
    exit(error);
}

/*
 * Le um numero do teclado, fazendo a validacao de inteiro
 */
int readNumber(long *number){
    char buffer[256];
    char *ptr;
    fgets(buffer, 265, stdin);
    long value = strtol(buffer, &ptr, 10);
    *number = value;

    // Se for 0 e o buffer nao for "0\n", nao eh um numero
    if(*number == 0 && strcmp(buffer, "0\n")) return 1;

    return 0; 
}

/*
 * Le um numero do teclado com valor minimo, fazendo a validacao de inteiro
 */
int readNumberWithMin(long *number, long min){
    if(readNumber(number)) return 1;

    if(*number < min) return 1;

    return 0; 
}

/*
 * Le um numero do teclado com valor minimo e maximo, fazendo a validacao de inteiro
 */
int readNumberInRange(long *number, long min, long max){
    if(readNumberWithMin(number, min)) return 1;

    if(*number > max) return 1;

    return 0; 
}
