#include "../headers/main.h"

int showMenu();

int main(int argc, char *argv[]){
    readArgumentsFromConsole(argc, argv);
    int readProcessesFrom = showMenu();
    StructureCollection *structures = createStructures(readProcessesFrom);
    scheduler(structures);
    destroyStructures(&structures);
    return NO_ERROR;
}

/*
 * Mostra o menu para selecao de entrada dos processos
 */
int showMenu() {
    long choice;
    printf("Ola usuario, bem vindo ao simulador de escalonamento de processos! \nComo voce gostaria de realizar a criacao dos processos? \n 1 - Criar a partir do arquivo input.txt \n 2 - Criar a partir do teclado \n 3 - Criar com numeros aleatorios \n");
    printf("Sua escolha: ");
    while(readNumberInRange(&choice, 1, 3)){
        printf("Opcao invalida, entre com uma das opcoes: 1, 2 ou 3.\n");
    }
    printf("\n");
    return choice;
}
