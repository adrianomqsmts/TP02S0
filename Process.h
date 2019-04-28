#ifndef TP02SO_PROCESS_H
#define TP02SO_PROCESS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct EstadoProcesso{
    int inteiro;
    int contador; // Inicializar com 0
    int tamanho;
    char *programa;
} EstadoProcesso;

#endif //TP02SO_PROCESS_H
