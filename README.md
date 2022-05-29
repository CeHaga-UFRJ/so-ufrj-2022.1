# Sistemas Operacionais | Trabalho 01 | 2022.1
*Simulador que implementa o algoritmo de escalonamento de processos, usando a estratégia de seleção Round Robin (ou Circular) com Feedback.*

## Tabela de Conteúdo

1. [Tecnologias utilizadas](#tecnologias-utilizadas)
2. [Estrutura do repositório](#estrutura-do-repositório)
3. [Como usar o programa?](#como-usar-o-programa)
4. [Testes](#testes)
5. [Autores](#autores)

## 🖥️ Tecnologias utilizadas
O projeto foi desenvolvido utilizando a linguagem C e para a compilação recomenda-se o uso do GCC (GNU Compiler Collection).

![Badge](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

## 📂 Estrutura do repositório
O repositório é composto pelos subdiretórios:
* **in**: conjunto de arquivos de entrada
* **headers**: conjunto de headers do programa
* **src**: conjunto de códigos fonte do programa

## 🤔 Como usar o programa?
1.  Clone esse repositório

2. Utilize o comando abaixo do makefile para compilar o programa e executar o executável gerado:
```
  make run
```

Nessa opção, você não poderá escolhar o valor do *quantum*, *duração da operação da impressora*, *duração da operação do disco*, nem *duração da operação da fita magnética*. Serão utilizados os valores default abaixo:
* **quantum**: 4ut
* **disco**: 3ut
* **fita**: 5ut
* **impressora**: 8ut

Caso queira alterar algum valor default, use os argumentos abaixo:
```
  make run QUANTUM_SIZE = w DISK_OP_SIZE = x TAPE_OP_SIZE = y PRINTER_OP_SIZE = z
```

## 🧪 Testes
Alguns testes foram pré-configurados no makefile para testar o funcionamento do escalonador.

1. Para testar o funcionamento com base nos processos do arquivo input.txt:
```
  make test_create_processes_by_file
```

2. Para testar o funcionamento a partir do teclado:
```
  make test_create_processes_by_keyboard
```

3. Para testar o funcionamento com processos aleatórios:
```
  make test_create_processes_randomly
```

4. Para testar o funcionamento quando o número máximo de I/O por processo é ultrapassado:
```
  make test_max_io
```

5. Para testar o funcionamento quando o número máximo de processos é ultrapassado:
```
  make test_max_processes
```

## 👩‍💻 Autores
* Carlos Henrique Bravo Serrado
* Gabriele Jandres Cavalcanti
* Markson Viana Arguello
* Victor Wohlers Cardoso