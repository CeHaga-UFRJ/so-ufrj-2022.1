#include "main.h"

void exitProgram(int error){
    if(!error) return;
    printf("#Erro de codigo %d\n",error);
    exit(error);
}

int showMenu() {
    int choice;
    printf("Ola usuario, bem vindo ao simulador de escalonamento de processos! \nComo voce gostaria de realizar a criacao dos processos? \n 1 - Criar a partir do arquivo input.txt \n 2 - Criar a partir do teclado \n 3 - Criar com numeros aleatorios \n");
    printf("Sua escolha: "), 
    scanf("%i", &choice);
    return choice;
}

int main(int argc, char *argv[]){
    readArgumentsFromConsole(argc, argv);
    int readProcessesFrom = showMenu();
    return 0;
}
