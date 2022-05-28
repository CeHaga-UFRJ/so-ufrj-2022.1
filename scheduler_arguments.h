#ifndef __SCHEDULER_ARGS__
#define __SCHEDULER_ARGS__

#define MAX_PROCESSES 10
#define MAX_IO 3
#define TIME_SLICE getTimeSlice()
#define DISK_TIMER getDiskTimer()
#define TAPE_TIMER getTapeTimer()
#define PRINTER_TIME getPrinterTimer()

extern int getTimeSlice();
extern int getDiskTimer();
extern int getTapeTimer();
extern int getPrinterTimer();

#endif
