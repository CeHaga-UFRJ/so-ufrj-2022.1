# Sistemas Operacionais | Trabalho 02 | 2022.1
*Simulador que implementa um gerenciador de memória virtual usando o algoritmo LRU*

## Tabela de Conteúdo

1. [Tecnologias utilizadas](#tecnologias-utilizadas)
2. [Estrutura do repositório](#estrutura-do-repositório)
3. [Como usar o programa?](#como-usar-o-programa)
4. [Autores](#autores)

## 🖥️ Tecnologias utilizadas
O projeto foi desenvolvido utilizando a linguagem C e para a compilação recomenda-se o uso do GCC (GNU Compiler Collection).

![Badge](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

## 📂 Estrutura do repositório
O repositório é composto pelos subdiretórios:
* **headers**: conjunto de headers do programa
* **src**: conjunto de códigos fonte do programa

## 🤔 Como usar o programa?
1.  Clone esse repositório

2. Utilize o comando abaixo do makefile para compilar o programa e executar o executável gerado:
```
  make run
```

Nessa opção, você não poderá escolhar o valor da *quantidade de frames*, *quantidade de processos*, *quantidade de páginas virtuais por processo*, *limite do working set*, *tempo de espera entre ciclos* e nem *quantidade máxima de ciclos*. Serão utilizados os valores default abaixo:
* **frames**: 64
* **processos**: 20
* **páginas virtuais**: 50
* **working set limit**: 4
* **tempo de espera**: 3

Caso queira alterar algum valor default, use os argumentos abaixo:
```
  make run QTD_FRAMES = a QTD_PROCESSES = b QTD_PAGES = c WS_LIMIT = d WAIT_TIME = e MAX_CICLES = f
```

## 👩‍💻 Autores
* Carlos Henrique Bravo Serrado
* Gabriele Jandres Cavalcanti
* Markson Viana Arguello
* Victor Wohlers Cardoso