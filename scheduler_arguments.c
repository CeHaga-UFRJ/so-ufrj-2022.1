#include "scheduler_arguments.h"

int time_slice;
int disk_timer;
int tape_timer;
int printer_timer;

void readArgumentsFromConsole(int argc, char *argv[]){
    time_slice = 4;
    disk_timer = 3;
    tape_timer = 5;
    printer_timer = 8;

    for(int i = 1; i < argc; i++){
        char *arg = argv[i];
        if(arg[0] != '-' || arg[1] == '\0') exitProgram(INVALID_ARGUMENT);
        char flag = arg[1];
        arg += 2;
        
        switch(flag){
            case 'q':
                time_slice = handleParameter(arg);
                break;
            case 'd':
                disk_timer = handleParameter(arg);
                break;
            case 't':
                tape_timer = handleParameter(arg);
                break;
            case 'p':
                printer_timer = handleParameter(arg);
                break;
            case 'h':
                printf(HELP);
                exitProgram(NO_ERROR);
                break;
            default:
                exitProgram(INVALID_ARGUMENT);
        }
    }
}

int handleParameter(char *ps){
    if(ps[0] == '\0') exitProgram(INVALID_NUMBER);

    int value = 0;
    for(char *s = ps; *s != '\0'; s++){
        if(*s < '0' || *s > '9') exitProgram(INVALID_NUMBER);

        value = value * 10 + (*s - '0');
    } 
    return value;
}

int getTimeSlice(){
    return time_slice;
}

int getDiskTimer(){
    return disk_timer;
}

int getTapeTimer(){
    return tape_timer;
}

int getPrinterTimer(){
    return printer_timer;
}
