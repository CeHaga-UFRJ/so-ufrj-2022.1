#include "../headers/globals.h"

/*
 * Finaliza o programa e imprime a mensagem de erro
 */
void exitProgram(int error, char *errorMessage){
    if(!error) return;
    printf("%s\n", errorMessage);
    exit(error);
}