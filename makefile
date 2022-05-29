CMPL=cd src && gcc
HELP=-h
QUANTUM= -q
DISK_READ= -d
TAPE_READ= -t
PRINTER_READ= -p
TARGET=*.c
QNTM_SIZE=4
DREAD_SIZE=5
TREAD_SIZE=2
PREAD_SIZE=2
OBJS=a.out

help:
	$(CMPL) $(TARGET) && ./$(OBJS) $(HELP)

quantum:
	$(CMPL) $(TARGET) && ./$(OBJS) $(QUANTUM)$(QNTM_SIZE)

disk_readt:
	$(CMPL) $(TARGET) && ./$(OBJS) $(DISK_READ)$(DREAD_SIZE)

tape_readt:
	$(CMPL) $(TARGET) && ./$(OBJS) $(TAPE_READ)$(TREAD_SIZE)

printer_readt:
	$(CMPL) $(TARGET) && ./$(OBJS) $(PRINTER_READ)$(PREAD_SIZE)


#=====================TESTES=========================

#Teste de input inválido do menu
test1:
	$(CMPL) $(TARGET)
	echo w | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input por arquivo correto
test2:
	$(CMPL) $(TARGET)
	printf "1, 13, 0, D-2/D-4/I-9 \n3, 2, 1 \n2, 15, 13, F-1/D-7" > input.txt
	echo 1 | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input por arquivo com número de I/Os maior que o permitido
test3:
	$(CMPL) $(TARGET)
	printf "1, 13, 0, D-2/D-4/I-9/F-5 \n3, 2, 1 \n2, 15, 13, F-1/D-7" > input.txt
	echo 1 | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input por arquivo com número de processos maior que o permitido
test4:
	$(CMPL) $(TARGET)
	printf "1, 13, 0, D-2/D-4/I-9/F-5 \n3, 2, 1 \n2, 15, 2, F-1/D-7 \n2, 15, 3, F-1/D-7 \n2, 15, 4, F-1/D-7 \n2, 15, 5, F-1/D-7 \n2, 15, 6, F-1/D-7 \n2, 15, 7, F-1/D-7 \n2, 15, 8, F-1/D-7 \n2, 15, 9, F-1/D-7" > input.txt
	echo 1 | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input por teclado com opção inválida
test5:
	$(CMPL) $(TARGET)
	echo 2 D | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input por teclado com input válido
test6:
	$(CMPL) $(TARGET)
	echo 2 1 13 1 2 1 4 3 9 2 | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to

#Teste de input com números aleatórios
test7:
	$(CMPL) $(TARGET)
	echo 3 | ./$(OBJS) $(QUANTUM)5 $(DISK_READ)5 $(TAPE_READ)5 $(PRINTER_READ)5

#Expected to