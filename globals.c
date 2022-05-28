#include "globals.h"

void exitProgram(int error, char *errorMessage);

/*
Finaliza o programa e imprime a mensagem de erro
*/
void exitProgram(int error, char *errorMessage){
    if(!error) return;
    printf("%s\n", errorMessage);
    exit(error);
}
