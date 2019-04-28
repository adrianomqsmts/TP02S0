#ifndef TP02SO_PROCESSMANAGER_H
#define TP02SO_PROCESSMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Process.h"

#define MAXTAM 100

typedef struct TIME {
    int time; // Inicializar com 0
} TIME;

typedef struct CPU {
    int tempoCpu;
    int contadorAtual;
    int n;
    int tempoAtual;
} CPU;

typedef struct Processo{
    pid_t pid;
    pid_t pid_pai;
    EstadoProcesso estadoProcesso;
    int prioridade;
    char estado[15];
    TIME time;
    CPU cpu;
} Processo;

typedef struct PC_TABLE {
    Processo vetor[MAXTAM];
    int Primeiro, Ultimo;
} PC_TABLE;

typedef struct READY_STATE {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} READY_STATE;

typedef struct BLOCKED_STATE {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} BLOCKED_STATE;

typedef struct RUNNING_STATE {
    Processo vetor[MAXTAM]; // Realmente precisa de um vetor?
    int Frente, Tras;
} RUNNING_STATE;


#endif //TP02SO_PROCESSMANAGER_H
