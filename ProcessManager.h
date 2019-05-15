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
    int fatiaTempo;
    int fatiaTempoUsada;
} Cpu;

typedef struct Processo{ // Atributos na estrutura errada?
    pid_t pid;
    pid_t pid_pai;
    int prioridade;
    char estado[15];
    int timeInicio;
    int tempoCPU;
    EstadoProcesso estadoProcesso;
} Processo;

typedef struct Fila {
    char nome[30];
    int vetor[MAXTAM];
    int Frente, Tras;
} Fila;

typedef struct PcbTable {
    Processo vetor[MAXTAM];
    int Primeiro, Ultimo;
} PcbTable;

typedef struct ReadyState {
    Fila filaPrioridade0;
    Fila filaPrioridade1;
    Fila filaPrioridade2;
    Fila filaPrioridade3;
} ReadyState;

typedef struct BlockedState {
    Fila filaBlockedState;
} BlockedState;

typedef struct RunningState {
    int iPcbTable;
} RunningState;

void inicializarEstruturas(RunningState *runningState, ReadyState *readyState, BlockedState *blockedState,
                           PcbTable *pcbTable, Cpu *cpu, Time *time);
Processo criarPrimeiroSimulado(Programa *programa, Time *timee, int qtdeInstrucoes, int pidPai);
Processo criarProcessoSimulado(Time *timee, Processo *processoPai);
int colocarProcessoCPU(Cpu *cpu, ReadyState *readyState, RunningState *runningState, PcbTable *pcbTable);
void ImprimirCPU(Cpu *cpu);
int runCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, RunningState *runningState, BlockedState *blockedState,
           ReadyState *readyState);
void zerarCPU(Cpu *cpu);

void FFVazia(Fila *fila);
int EhVazia(Fila *fila);
void Enfileira(Fila *fila, int indiceProcesso);
int Desenfileira(Fila *fila);
void ImprimeFila(Fila *fila, PcbTable *pcbTable);
void AtualizaFila(Fila *fila, int indiceProcesso);
void RemoverProcessoFila(Fila *fila, int indiceProccesso);

void FLVaziaPcbTable(PcbTable *pcbTable);
int VaziaPcbTable(PcbTable *pcbTable);
int InserePcbTable(PcbTable *pcbTable, Processo processo);
int RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo);
void ImprimePcbTable(PcbTable *pcbTable);


#endif //TP02SO_PROCESSMANAGER_H
