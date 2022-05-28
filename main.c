#include "main.h"

int handleParameter(char *ps){
    if(ps[0] == '\0') exitProgram(INVALID_NUMBER);

    int value = 0;
    for(char *s = ps; *s != '\0'; s++){
        if(*s < '0' || *s > '9') exitProgram(INVALID_NUMBER);

        value = value * 10 + (*s - '0');
    } 
    return value;
}

void exitProgram(int error){
    if(!error) return;
    printf("#Erro de codigo %d\n",error);
    exit(error);
}

int main(int argc, char *argv[]){
  
    return 0;
}
