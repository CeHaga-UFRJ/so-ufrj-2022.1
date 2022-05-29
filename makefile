CMPL = cd src && gcc
WARN = -Wall
OBJS = a.out
HELP = -h
QUANTUM = -q
DISK = -d
TAPE = -t
PRINTER = -p
TARGET = *.c
QUANTUM_SIZE = 4
DISK_OP_SIZE = 3
TAPE_OP_SIZE = 5
PRINTER_OP_SIZE = 8
EXE = ./$(OBJS) $(QUANTUM)$(QUANTUM_SIZE) $(DISK)$(DISK_OP_SIZE) $(TAPE)$(TAPE_OP_SIZE) $(PRINTER)$(PRINTER_OP_SIZE)

run:
	$(CMPL) $(TARGET) $(WARN) && $(EXE)

help:
	$(CMPL) $(TARGET) $(WARN) && ./$(OBJS) $(HELP) || true

# ===== Testes =====

# Teste de input por arquivo correto
test_create_processes_by_file:
	printf "1, 13, 0, D-2/D-4/I-9 \n2, 2, 1 \n3, 15, 13, F-1/D-7" > in/input.txt
	$(CMPL) $(TARGET) && echo 1 | $(EXE)

# Expected to

# Teste de input por teclado com input válido
test_create_processes_by_keyboard:
	$(CMPL) $(TARGET) && echo 2 1 13 1 2 1 4 3 9 2 | $(EXE)

# Expected to

# Teste de input com números aleatórios
test_create_processes_randomly:
	$(CMPL) $(TARGET) && echo 3 | $(EXE)

# Expected to

# Teste de input por arquivo com número de I/Os maior que o permitido
test_max_io:
	printf "1, 13, 0, D-2/D-4/I-9/F-5 \n2, 2, 1 \n3, 15, 13, F-1/D-7" > in/input.txt
	$(CMPL) $(TARGET) && echo 1 | $(EXE)

# Expected to

# Teste de input por arquivo com número de processos maior que o permitido
test_max_processes:
	printf "1, 13, 0, D-2/D-4/I-9 \n2, 2, 1 \n3, 15, 3, F-1/D-7 \n4, 15, 4, F-1 \n5, 15, 5, D-7 \n6, 15, 6, I-1 \n7, 15, 7" > in/input.txt
	$(CMPL) $(TARGET) && echo 1 | $(EXE)

# Expected to