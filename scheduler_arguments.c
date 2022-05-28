#include "scheduler_arguments.h"

int time_slice;
int disk_timer;
int tape_timer;
int printer_timer;

void readArgumentsFromConsole(){
    time_slice = 4;
    disk_timer = 3;
    tape_timer = 5;
    printer_timer = 8;
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
