CC=gcc
HELP=-h
QUANTUM= -q
DISK_READ= -d
TAPE_READ= -t
PRINTER_READ= -p
#FLAGS = 
TARGET=scheduler.c
QNTM_SIZE=4
DREAD_SIZE=5
TREAD_SIZE=2
PREAD_SIZE=2
OBJS=scheduler.o

help:
	$(CC) $(TARGET) -o $(OBJS) && ./$(OBJS) $(HELP)

quantum:
	$(CC) $(TARGET) -o $(OBJS) && ./$(OBJS) $(QUANTUM) $(QNTM_SIZE)

disk_readt:
	$(CC) $(TARGET) -o $(OBJS) && ./$(OBJS) $(DISK_READ) $(DREAD_SIZE)

tape_readt:
	$(CC) $(TARGET) -o $(OBJS) && ./$(OBJS) $(TAPE_READ) $(TREAD_SIZE)

printer_readt:
	$(CC) $(TARGET) -o $(OBJS) && ./$(OBJS) $(PRINTER_READ) $(PREAD_SIZE)	
