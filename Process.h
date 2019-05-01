#ifndef TP02SO_PROCESS_H
#define TP02SO_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct TipoInstrucao {
    char instrucao[20];
} TipoInstrucao;

typedef struct EstadoProcesso{
    int inteiro;
    int contador; // Inicializar com 0
    int tamanho;
    TipoInstrucao programa[100]; // Armazenar as instruções com cada elemento do vetor contendo uma instrução..
} EstadoProcesso;

void executarInstrucao(EstadoProcesso *estadoProcesso, char instrucao[]);

#endif //TP02SO_PROCESS_H
