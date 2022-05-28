#include "headers/globals.h"

void exitProgram(int error);

void exitProgram(int error){
    if(!error) return;
    printf("#Erro de codigo %d\n",error);
    exit(error);
}