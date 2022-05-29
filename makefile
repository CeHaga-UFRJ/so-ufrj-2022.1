CMPL=cd src && gcc
WARN=-Wall
OBJS=a.out
HELP=-h
QUANTUM= -q
DISK= -d
TAPE= -t
PRINTER= -p
TARGET=*.c
QUANTUM_SIZE=4
DISK_OP_SIZE=3
TAPE_OP_SIZE=5
PRINTER_OP_SIZE=8

run:
	$(CMPL) $(TARGET) $(WARN) && ./$(OBJS) $(QUANTUM)$(QUANTUM_SIZE) $(DISK)$(DISK_OP_SIZE) $(TAPE)$(TAPE_OP_SIZE) $(PRINTER)$(PRINTER_OP_SIZE)

get-help:
	$(CMPL) $(TARGET) $(WARN) && ./$(OBJS) $(HELP)

# ===================== Testes =========================

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