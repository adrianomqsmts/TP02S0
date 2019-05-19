#ifndef TP02SO_PROCESSCOMMANDER_H
#define TP02SO_PROCESSCOMMANDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXTAM 100
#define BUFFER 256

typedef struct Instrucao {
    char instrucao[20];
} Instrucao;

typedef struct Programa {
    Instrucao instrucoes[20];
    int tamanho;
    int Primeiro, Ultimo;
} Programa;

void RunProcessCommander();
void FLVaziaPrograma(Programa *programa);
int VaziaPrograma(Programa *programa);
int InserePrograma(Programa *programa, char *instrucao);
int RetiraPrograma(Programa *programa, int indice, char *instrucao);
int PegarInstrucaoPrograma(Programa *programa, int indice, char *instrucao);

#endif //TP02SO_PROCESSCOMMANDER_H
