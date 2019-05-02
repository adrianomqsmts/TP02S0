#ifndef TP02SO_PROCESSMANAGER_H
#define TP02SO_PROCESSMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ProcessCommander.h"

#define MAXTAM 100
#define BUFFER 256

typedef struct Time {
    int time; // Inicializar com 0
} Time;

typedef struct Cpu { // Rever os atributos
    Programa programa;
    int contadorProgramaAtual;
    int valorInteiro;
    float fatiaTempo;
    float fatiaTempoUsada;
} Cpu;

typedef struct Processo{ // Atributos na estrutura errada?
    pid_t pid;
    pid_t pid_pai;
    int prioridade;
    char estado[15];
    float timeInicio;
    float tempoCPU;
    EstadoProcesso estadoProcesso;
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

void inicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time);
Processo criarPrimeiroSimulado(Programa *programa, int qtdeInstrucoes);
void colocarProcessoCPU(Cpu *cpu, PcbTable *pcbTable, int qtdeInstrucoes);
void ImprimirCPU(Cpu *cpu);
void runCPU(Cpu *cpu, Time *time, PcbTable *pcbTable);
void FFVaziaReady(ReadyState *readyState);
void FFVaziaBlocked(BlockedState *blockedState);
int VaziaReady(ReadyState *readyState);
int VaziaBlocked(BlockedState *blockedState);
void EnfileiraReady(ReadyState *readyState, Processo processo);
void EnfileiraBlocked(BlockedState *blockedState, Processo processo);
void DesenfileiraReady(ReadyState *readyState, Processo *processo);
void DesenfileiraBlocked(BlockedState *blockedState, Processo *processo);
void ImprimeReady(ReadyState *readyState);
void ImprimeBlocked(BlockedState *blockedState);
void FLVaziaPcbTable(PcbTable *pcbTable);
int VaziaPcbTable(PcbTable *pcbTable);
void InserePcbTable(PcbTable *pcbTable, Processo processo);
void RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo);
void ImprimePcbTable(PcbTable *pcbTable);

#endif //TP02SO_PROCESSMANAGER_H
