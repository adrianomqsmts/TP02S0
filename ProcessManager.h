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
#define BUFFER 256

typedef struct Time {
    int time; // Inicializar com 0
} Time;

typedef struct Cpu { // Rever os atributos
    int tempoCpu;
    int contadorAtual;
    int n;
    int tempoAtual;
} Cpu;

typedef struct Processo{
    pid_t pid;
    pid_t pid_pai;
    EstadoProcesso estadoProcesso;
    int prioridade;
    char estado[15];
    Time time;
    Cpu cpu;
} Processo;

typedef struct PcbTable {
    Processo vetor[MAXTAM];
    int Primeiro, Ultimo;
} PcbTable;

typedef struct ReadyState {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} ReadyState;

typedef struct BlockedState {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} BlockedState;

typedef struct RunningState {
    int iPcbTable;
} RunningState;

RunningState runningState;
ReadyState readyState;
BlockedState blockedState;
PcbTable pcbTable;
Cpu cpu;
Time timee;

void FFVaziaReady();
void FFVaziaBlocked();
void FLVazia();
void inicializarEstruturas();

#endif //TP02SO_PROCESSMANAGER_H
